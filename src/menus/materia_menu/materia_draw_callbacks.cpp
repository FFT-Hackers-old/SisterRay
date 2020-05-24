#include "materia_draw_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"

using namespace MateriaWidgetNames;

void handleChangeMateriaCharacter(const MenuDrawEvent* event) {
    const char * fetchedName;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto topWidget = getChild(mainWidget, CHAR_DATA_WIDGET_NAME);
    updatePortraitPartyIndex(getChild(topWidget, PORTRAIT_WIDGET_NAME), *MAT_MENU_PARTY_INDEX);
    updateHPBarPartyIndex(getChild(topWidget, HPBAR_WIDGET_NAME), *MAT_MENU_PARTY_INDEX);
}

void handleUpdateMateriaDescription(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    auto materiaDescription = getChild(menuWidget, MATERIA_DESC_WIDGET_NAME);
    auto descriptionWidget = getChild(materiaDescription, MATERIA_DESC);

    auto& characterRecord = gContext.party.getActivePartyCharacter(*EQUIP_MENU_PARTY_INDEX);
    if (event->menuState != 1 && event->menuState != 2) {
        disableWidget(descriptionWidget);
        return;
    }

    auto slotChoice = getStateCursor(event->menu, 1)->context;
    u16 materiaID;
    if (event->menuState == 2) {
        auto materiaChoice = getStateCursor(event->menu, 2)->context;
        materiaID = gContext.materiaInventory->getResource(materiaChoice.baseRowIndex + materiaChoice.relativeRowIndex).materiaID;
    }
    else if (event->menuState == 1) {
        auto& gearSlot = characterRecord.equipment[slotChoice.relativeRowIndex];
        materiaID = gearSlot.materia[slotChoice.relativeColumnIndex].materiaID;
    }

    enableWidget(descriptionWidget);
    auto fetchedDescription = gContext.materias.getResource(materiaID).materiaDescription.str();
    updateText(descriptionWidget, fetchedDescription);
}

void handleUpdateMateriaView(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    auto gridWidget = getChild(menuWidget, MATERIA_GRID_WIDGET_NAME);
    auto slotChoice = getStateCursor(event->menu, 0)->context;
    if ((event->menuState == 0 || (event->menuState < 7 && event->menuState > 2)) && slotChoice.relativeRowIndex == 0) {
        disableWidget(gridWidget);
        return;
    }
    enableWidget(gridWidget);
    if (event->menuState == 1 || event->menuState == 2 || event->menuState == 8) {
        enableWidget(getChild(gridWidget, MATERIA_GRID));
        return;
    }
    disableWidget(getChild(gridWidget, MATERIA_GRID));
}

void handleUpdateMateriaData(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    auto& characterRecord = gContext.party.getActivePartyCharacter(*MAT_MENU_PARTY_INDEX);
    MateriaInventoryEntry materia;
    auto standardViewWidget = getChild(getChild(menuWidget, MATERIA_DATA_WIDGET_NAME), STANDARD_DISPLAY);
    if (event->menuState != 1 && event->menuState != 2) {
        disableWidget(standardViewWidget);
        return;
    }
    auto slotChoice = getStateCursor(event->menu, 1)->context;
    auto& gearSlot = characterRecord.equipment[slotChoice.relativeRowIndex];
    if (event->menuState == 2) {
        auto materiaChoice = getStateCursor(event->menu, 2)->context;
        materia = gContext.materiaInventory->getResource(materiaChoice.baseRowIndex + materiaChoice.relativeRowIndex);
    }
    else if (event->menuState == 1) {
        materia = gearSlot.materia[slotChoice.relativeColumnIndex];
    }

    if (materia.materiaID != 0xFFFF) {
        enableWidget(standardViewWidget);
        updateMateriaDisplay(standardViewWidget, materia);
        return;
    }
    disableWidget(standardViewWidget);
}

void updateMateriaDisplay(Widget* displayWidget, MateriaInventoryEntry materia) {
    auto materiaName = gContext.materias.getResource(materia.materiaID).materiaName.str();
    updateText(getChild(displayWidget, MATERIA_NAME), materiaName);
    /*last byte of status effect is element index; this is temporary until we have fully moved materia into sister ray*/
    auto elementIndex = gContext.materias.getResource(materia.materiaID).gameMateria.elementType;
    auto elementName = gContext.gameStrings.elementNames.get_string(elementIndex);
    updateText(getChild(displayWidget, MATERIA_ELEMENT), elementName);

    updateAssetType(getChild(displayWidget, MATERIA_SPHERE), getMateriaColorType(materia.materiaID));

    auto materiaType = getMateriaColorType(materia.materiaID);
    updateAssetType(getChild(displayWidget, MATERIA_SPHERE), materiaType);

    u8 maxLevel = 1;
    auto materiaLevel = getMateriaLevel(materia, &maxLevel); //returns the current level and sets the max level local via ptr arg
    if (materiaLevel < maxLevel) {
        enableWidget(getChild(displayWidget, CURRENT_AP));
        disableWidget(getChild(displayWidget, MASTERED));
        updateNumber(getChild(displayWidget, CURRENT_AP), materia.materiaAP);
        updateNumber(getChild(displayWidget, TO_LEVEL_AP), gContext.materias.getResource(materia.materiaID).gameMateria.apLevel[materiaLevel - 1] * 100 - materia.materiaAP);
    }
    else {
        disableWidget(getChild(displayWidget, CURRENT_AP));
        enableWidget(getChild(displayWidget, MASTERED));
        updateNumber(getChild(displayWidget, TO_LEVEL_AP), 0);
    }
    auto starsWidget = getChild(displayWidget, MATERIA_STARS);
    for (auto row = 0; row < 5; ++row) {
        auto widget = getChild(starsWidget, row);
        if (row + 1 > maxLevel) {
            disableWidget(widget);
            continue;
        }
        enableWidget(widget);
        updateAssetType(widget, materiaType);
        if (row + 1 > materiaLevel) {
            setStarShaded(widget, false);
            continue;
        }
        setStarShaded(widget, true);
        
    }
    //update the equip effect widget
    disableWidget(getChild(displayWidget, ABILITIES_LIST));
    disableWidget(getChild(displayWidget, EQUIP_EFFECTS));
    dispatchMateriaTypeHandlers(displayWidget, materia, materiaLevel);
}

void dispatchMateriaTypeHandlers(Widget* displayWidget, const MateriaInventoryEntry& materia, u8 materiaLevel) {
    DrawMateriaDataEvent event = { displayWidget, materia, materiaLevel };
    auto materiaType = getMateriaTopType(materia.materiaID);
    auto subtype = getMateriaSubType(materia.materiaID);

    auto topkey = getTopKey(materiaType);
    auto subkey = getSubKey(subtype);

    auto dispatchContext = std::vector<SrEventContext>({ topkey, subkey });
    gContext.eventBus.dispatch(DRAW_MATERIA_DATA, &event, dispatchContext);
}

/*Initializes the command view widget used */
void drawCommandViewWidget(const MenuDrawEvent* event) {
    auto viewChoiceCursor = getStateCursor(event->menu, 0);
    auto commandChoiceCursor = getStateCursor(event->menu, 3);
    auto commandViewWidget = getChild(event->menu->menuWidget, COMMAND_VIEW_WIDGET_NAME);
    if (!(event->menuState >= 3 && event->menuState < 7) && !(event->menuState == 0 && viewChoiceCursor->context.relativeColumnIndex == 1)) {
        disableWidget(commandViewWidget);
        return;
    }

    enableWidget(commandViewWidget);
    srLogWrite("Drawing widget for actor %i", *MAT_MENU_PARTY_INDEX);
    commandChoiceCursor->context.maxColumnBound = PARTY_STRUCT_ARRAY[*MAT_MENU_PARTY_INDEX].commandColumns;
    commandChoiceCursor->context.viewColumnBound = PARTY_STRUCT_ARRAY[*MAT_MENU_PARTY_INDEX].commandColumns;
    resizeBox(getChild(commandViewWidget, CMD_GRID_BOX), 65 * commandChoiceCursor->context.maxColumnBound + 0x98, 0x78);
}

void drawSpellsWidget(const MenuDrawEvent* event) {
    auto magicGrids = getChild(event->menu->menuWidget, SPELL_VIEW_WIDGET_NAME);
    auto spellBox = getChild(magicGrids, SPELL_VIEW_BOX);
    auto magicGrid = getChild(magicGrids, SPELL_GRID);
    if (event->menuState != 4) {
        disableWidget(magicGrid);
        disableWidget(spellBox);
    }
    else {
        enableWidget(magicGrid);
        enableWidget(spellBox);
        return;
    }

    auto summonGrid = getChild(magicGrids, SUMMON_GRID);
    if (event->menuState != 5) {
        disableWidget(summonGrid);
        disableWidget(spellBox);
    }
    else {
        enableWidget(summonGrid);
        enableWidget(spellBox);
        return;
    }

    auto eSkillGrid = getChild(magicGrids, ESKILL_GRID);
    if (event->menuState != 6) {
        disableWidget(eSkillGrid);
        disableWidget(spellBox);
    }
    else {
        enableWidget(eSkillGrid);
        enableWidget(spellBox);
        return;
    }
}



