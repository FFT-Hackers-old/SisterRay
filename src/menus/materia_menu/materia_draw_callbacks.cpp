#include "materia_draw_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"

using namespace MateriaWidgetNames;

void handleChangeCharacter(const MateriaDrawEvent* event) {
    const char * fetchedName;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    u8 characterRecordArrayIndex = getCharacterRecordIndex(*MAT_MENU_PARTY_INDEX);

    auto topWidget = getChild(mainWidget, CHAR_DATA_WIDGET_NAME);
    updatePortraitPartyIndex(getChild(topWidget, PORTRAIT_WIDGET_NAME), *MAT_MENU_PARTY_INDEX);
    updateHPBarPartyIndex(getChild(topWidget, HPBAR_WIDGET_NAME), *MAT_MENU_PARTY_INDEX);

    std::vector<std::string> listNames = { EQUIPPED_WEAPON, EQUIPPED_ARMOR };
    for (u32 row = 0; row < listNames.size(); row++) {
        auto gear = getEquippedGear(characterRecordArrayIndex, row + 1);
        fetchedName = getNameFromRelativeID(gear, row + 1);
        updateText(getChild(topWidget, listNames[row]), fetchedName);
    }

    auto slotsPtr = &(gContext.weapons.get_resource(getEquippedGear(characterRecordArrayIndex, 1)).materia_slots[0]);
    updateMateriaSlots(getChild(topWidget, GEAR_1_SLOTS), slotsPtr);
    auto materiaPtr = gContext.characters.get_element(getCharacterName(characterRecordArrayIndex)).wpnMaterias.data();
    updateMateriaData(getChild(topWidget, GEAR_1_SLOTS), materiaPtr);

    slotsPtr = &(gContext.armors.get_resource(getEquippedGear(characterRecordArrayIndex, 2)).materia_slots[0]);
    updateMateriaSlots(getChild(topWidget, GEAR_2_SLOTS), slotsPtr);
    materiaPtr = gContext.characters.get_element(getCharacterName(characterRecordArrayIndex)).armMaterias.data();
    updateMateriaData(getChild(topWidget, GEAR_2_SLOTS), materiaPtr);
}

void handleUpdateMateriaDescription(const MateriaDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    u8 characterRecordArrayIndex = getCharacterRecordIndex(*MAT_MENU_PARTY_INDEX);
    u16 materiaID;
    const char * fetchedDescription;
    auto materiaDescription = getChild(menuWidget, MATERIA_DESC_WIDGET_NAME);
    auto descriptionWidget = getChild(materiaDescription, MATERIA_DESC);

    if (event->menuState != 1 && event->menuState != 2) {
        disableWidget(descriptionWidget);
        return;
    }

    auto slotChoice = getStateCursor(event->menu, 1)->context;

    if (event->menuState == 2) {
        auto materiaChoice = getStateCursor(event->menu, 2)->context;
        materiaID = gContext.materiaInventory->get_resource(materiaChoice.baseRowIndex + materiaChoice.relativeRowIndex).item_id;
    }
    else if (event->menuState == 1) {
        switch (slotChoice.relativeRowIndex) {
            case 0:
                materiaID = gContext.characters.get_element(getCharacterName(characterRecordArrayIndex)).wpnMaterias[slotChoice.relativeColumnIndex].item_id;
                break;
            case 1:
                materiaID = gContext.characters.get_element(getCharacterName(characterRecordArrayIndex)).armMaterias[slotChoice.relativeColumnIndex].item_id;
                break;
            default: {
                break;
            }
        }
    }
    enableWidget(descriptionWidget);
    fetchedDescription = gContext.gameStrings.materia_descriptions.get_string(materiaID);
    updateText(descriptionWidget, fetchedDescription);
}

void handleUpdateMateriaView(const MateriaDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    u8 characterRecordArrayIndex = getCharacterRecordIndex(*MAT_MENU_PARTY_INDEX);
    MateriaInventoryEntry materia;
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

void handleUpdateMateriaData(const MateriaDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    u8 characterRecordArrayIndex = getCharacterRecordIndex(*MAT_MENU_PARTY_INDEX);
    MateriaInventoryEntry materia;
    const char * fetchedText;
    auto standardViewWidget = getChild(getChild(menuWidget, MATERIA_DATA_WIDGET_NAME), STANDARD_DISPLAY);
    if (event->menuState != 1 && event->menuState != 2) {
        disableWidget(standardViewWidget);
        return;
    }
    auto slotChoice = getStateCursor(event->menu, 1)->context;

    if (event->menuState == 2) {
        auto materiaChoice = getStateCursor(event->menu, 2)->context;
        materia = gContext.materiaInventory->get_resource(materiaChoice.baseRowIndex + materiaChoice.relativeRowIndex);
    }
    else if (event->menuState == 1) {
        switch (slotChoice.relativeRowIndex) {
            case 0:
                materia = gContext.characters.get_element(getCharacterName(characterRecordArrayIndex)).wpnMaterias[slotChoice.relativeColumnIndex];
                break;
            case 1:
                materia = gContext.characters.get_element(getCharacterName(characterRecordArrayIndex)).armMaterias[slotChoice.relativeColumnIndex];
                break;
            default: {
                break;
            }
        }
    }

    if (materia.item_id != 0xFFFF) {
        enableWidget(standardViewWidget);
        updateMateriaDisplay(standardViewWidget, materia);
        return;
    }
    disableWidget(standardViewWidget);
}

void updateMateriaDisplay(Widget* displayWidget, MateriaInventoryEntry materia) {
    auto materiaName = gContext.gameStrings.materia_names.get_string(materia.item_id);
    updateText(getChild(displayWidget, MATERIA_NAME), materiaName);
    /*last byte of status effect is element index; this is temporary until we have fully moved materia into sister ray*/
    auto elementIndex = gContext.materias.get_resource(materia.item_id).elementType;
    auto elementName = gContext.gameStrings.elementNames.get_string(elementIndex);
    updateText(getChild(displayWidget, MATERIA_ELEMENT), elementName);

    updateAssetType(getChild(displayWidget, MATERIA_SPHERE), getMateriaColorType(materia.item_id));

    auto materiaType = getMateriaColorType(materia.item_id);
    updateAssetType(getChild(displayWidget, MATERIA_SPHERE), materiaType);

    u8 maxLevel = 1;
    auto materiaLevel = getMateriaLevel(materia, &maxLevel); //returns the current level and sets the max level local via ptr arg
    if (materiaLevel < maxLevel) {
        enableWidget(getChild(displayWidget, CURRENT_AP));
        disableWidget(getChild(displayWidget, MASTERED));
        updateNumber(getChild(displayWidget, CURRENT_AP), materia.materia_ap);
        updateNumber(getChild(displayWidget, TO_LEVEL_AP), gContext.materias.get_resource(materia.item_id).apLevel[materiaLevel - 1] * 100 - materia.materia_ap);
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
    auto materiaType = getMateriaTopType(materia.item_id);
    auto subtype = getMateriaSubType(materia.item_id);

    auto topkey = getTopKey(materiaType);
    auto subkey = getSubKey(subtype);

    auto dispatchContext = std::vector<SrEventContext>({ topkey, subkey });
    gContext.eventBus.dispatch(DRAW_MATERIA_DATA, &event, dispatchContext);
}

/*Initializes the command view widget used */
void drawCommandViewWidget(const MateriaDrawEvent* event) {
    auto viewChoiceCursor = getStateCursor(event->menu, 0);
    auto commandChoiceCursor = getStateCursor(event->menu, 3);
    auto commandViewWidget = getChild(event->menu->menuWidget, COMMAND_VIEW_WIDGET_NAME);
    if (!(event->menuState >= 3 && event->menuState < 7) && !(event->menuState == 0 && viewChoiceCursor->context.relativeRowIndex == 0)) {
        disableWidget(commandViewWidget);
        return;
    }

    enableWidget(commandViewWidget);
    srLogWrite("Drawing widget for actor %i", *MAT_MENU_PARTY_INDEX);
    commandChoiceCursor->context.maxColumnBound = PARTY_STRUCT_ARRAY[*MAT_MENU_PARTY_INDEX].commandColumns;
    commandChoiceCursor->context.viewColumnBound = PARTY_STRUCT_ARRAY[*MAT_MENU_PARTY_INDEX].commandColumns;
    resizeBox(getChild(commandViewWidget, CMD_GRID_BOX), 0x2F, 0xD6, 65 * commandChoiceCursor->context.maxColumnBound + 0x98, 0x78);
}

void drawSpellsWidget(const MateriaDrawEvent* event) {
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
    }

    auto summonGrid = getChild(magicGrids, SUMMON_GRID);
    if (event->menuState != 5) {
        disableWidget(summonGrid);
        disableWidget(spellBox);
    }
    else {
        enableWidget(summonGrid);
        enableWidget(spellBox);
    }

    auto eSkillGrid = getChild(magicGrids, ESKILL_GRID);
    if (event->menuState != 6) {
        disableWidget(eSkillGrid);
        disableWidget(spellBox);
    }
    else {
        enableWidget(eSkillGrid);
        enableWidget(spellBox);
    }
}



