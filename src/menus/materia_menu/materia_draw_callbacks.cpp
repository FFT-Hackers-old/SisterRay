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

    std::vector <std::string> slotNames = { GEAR_1_SLOTS, GEAR_2_SLOTS };
    for (int row = 0; row < slotNames.size(); row++) {
        auto slotsPtr = &(gContext.weapons.get_resource(getEquippedGear(characterRecordArrayIndex, 0)).materia_slots[0]);
        updateMateriaSlots(getChild(topWidget, slotNames[row]), slotsPtr);
        auto materiaPtr = gContext.characters.get_element(getCharacterName(characterRecordArrayIndex)).wpnMaterias.data();
        updateMateriaData(getChild(topWidget, slotNames[row]), materiaPtr);
    }
}

void handleUpdateMateriaDescription(const MateriaDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    u8 characterRecordArrayIndex = getCharacterRecordIndex(*MAT_MENU_PARTY_INDEX);
    u16 materiaID;
    const char * fetchedDescription;
    if (event->menuState != 1 || event->menuState != 2)
        return;

    auto materiaDescription = getChild(menuWidget, MATERIA_DESC_WIDGET_NAME);
    auto descrptionWidget = getChild(materiaDescription, MATERIA_DESC);
    auto slotChoice = getStateCursor(event->menu, 1)->context;

    if (event->menuState == 2) {
        auto materiaChoice = getStateCursor(event->menu, 2)->context;
        materiaID = gContext.materiaInventory->get_resource(materiaChoice.baseRowIndex + materiaChoice.relativeRowIndex).item_id;
    }
    else if (event->menuState == 1) {
        switch (slotChoice.relativeRowIndex) {
            case 0:
                materiaID = gContext.characters.get_element(getCharacterName(characterRecordArrayIndex)).wpnMaterias[slotChoice.relativeRowIndex].item_id;
                break;
            case 1:
                materiaID = gContext.characters.get_element(getCharacterName(characterRecordArrayIndex)).armMaterias[slotChoice.relativeRowIndex].item_id;
                break;
            default: {
                break;
            }
        }
    }
    fetchedDescription = gContext.gameStrings.materia_descriptions.get_string(materiaID);
    updateText(descrptionWidget, fetchedDescription);
}

void handleUpdateMateriaData(const MateriaDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    u8 characterRecordArrayIndex = getCharacterRecordIndex(*MAT_MENU_PARTY_INDEX);
    MateriaInventoryEntry materia;
    const char * fetchedText;
    if (event->menuState != 1 || event->menuState != 2)
        return;

    auto materiaDataWidget = getChild(menuWidget, MATERIA_DATA_WIDGET_NAME);
    auto slotChoice = getStateCursor(event->menu, 1)->context;

    if (event->menuState == 2) {
        auto materiaChoice = getStateCursor(event->menu, 2)->context;
        materia = gContext.materiaInventory->get_resource(materiaChoice.baseRowIndex + materiaChoice.relativeRowIndex);
    }
    else if (event->menuState == 1) {
        switch (slotChoice.relativeRowIndex) {
            case 0:
                materia = gContext.characters.get_element(getCharacterName(characterRecordArrayIndex)).wpnMaterias[slotChoice.relativeRowIndex];
                break;
            case 1:
                materia = gContext.characters.get_element(getCharacterName(characterRecordArrayIndex)).armMaterias[slotChoice.relativeRowIndex];
                break;
            default: {
                break;
            }
        }
    }
    updateMateriaDisplay(getChild(materiaDataWidget, STANDARD_DISPLAY), materia);
}

void updateMateriaDisplay(Widget* displayWidget, MateriaInventoryEntry materia) {
    auto materiaName = gContext.gameStrings.materia_names.get_string(materia.item_id);
    updateText(getChild(displayWidget, MATERIA_NAME), materiaName);
    /*last byte of status effect is element index; this is temporary until we have fully moved materia into sister ray*/
    auto elementIndex = gContext.materias.get_resource(materia.item_id).elementType;
    auto elementName = gContext.gameStrings.elementNames.get_string(elementIndex);
    updateText(getChild(displayWidget, MATERIA_NAME), elementName);

    auto materiaType = getMateriaType(materia.item_id);
    updateAssetType(getChild(displayWidget, MATERIA_SPHERE), materiaType);

    u8 maxLevel = 1;
    auto materiaLevel = getMateriaLevel(materia, &maxLevel); //returns the current level and sets the max level local via ptr arg
    if (materiaLevel < maxLevel) {
        enableWidget(getChild(displayWidget, CURRENT_AP));
        disableWidget(getChild(displayWidget, MASTERED));
        updateNumber(getChild(displayWidget, CURRENT_AP), materia.materia_ap);
        updateNumber(getChild(displayWidget, TO_LEVEL_AP), gContext.materias.get_resource(materia.item_id).apLevel[materiaLevel - 1] - materia.materia_ap);
    }
    else {
        disableWidget(getChild(displayWidget, CURRENT_AP));
        enableWidget(getChild(displayWidget, MASTERED));
        updateNumber(getChild(displayWidget, TO_LEVEL_AP), 0);
    }
    //Update the STAR widget
    //update the equip effect widget
    updateEquipEffect(getChild(displayWidget, EQUIP_EFFECTS), materia);
    //update the ability list widget
    updateAbilityListWidget(getChild(displayWidget, ABILITIES_LIST), materia, materiaLevel);
}

void updateEquipEffect(Widget* equipEffectWidget, MateriaInventoryEntry materia) {
    auto equipEffect = gContext.materias.get_resource(materia.item_id).equipEffect;
    return;
}

//REFACTOR WHEN WE WANT THE ABILITY TO ADD NEW MATERIA TYPES: Create a callback table for each type of materia out of these case statements
void updateAbilityListWidget(Widget* abilityListWidget, MateriaInventoryEntry materia, u8 level) {
    auto materiaType = getMateriaType(materia.item_id);
    switch (materiaType) {
        case 9: { //Display for basic master materia
            auto data = gContext.materias.get_resource(materia.item_id).data; //These are bytes, they will need to be words instead after all materia facing logic is relocated
            for (auto idx = 0; idx < 5; ++idx) {
                auto widget = getChild(abilityListWidget, idx);
                if (data[idx] = 0xFF) {
                    disableWidget(widget);
                    continue;
                }
                enableWidget(widget);
                auto magicIndex = data[idx];
                auto abilityName = gContext.gameStrings.magic_names.get_string(magicIndex);
                updateText(widget, abilityName);
                if (idx <= level - 1) {
                    updateTextColor(widget, COLOR_WHITE);
                    return;
                }
                updateTextColor(widget, COLOR_GRAY);
            }
        }
        default: {

        }
    }
}
