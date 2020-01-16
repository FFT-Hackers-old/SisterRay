#include "equip_draw_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"

using namespace EquipWidgetNames;

void handleChangeCharacter(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    u16 kernelObjectID;
    const char* fetchedName;

    u8 characterRecordArrayIndex = getCharacterRecordIndex(*EQUIP_MENU_PARTY_INDEX);

    auto topWidget = getChild(menuWidget, CHAR_DATA_WIDGET_NAME);
    updatePortraitPartyIndex(getChild(topWidget, PORTRAIT_WIDGET_NAME), *EQUIP_MENU_PARTY_INDEX);
    updateHPBarPartyIndex(getChild(topWidget, HPBAR_WIDGET_NAME), *EQUIP_MENU_PARTY_INDEX);

    std::vector<std::string> listNames = { EQUIPPED_WEAPON, EQUIPPED_ARMOR, EQUIPPED_ACC };
    for (u32 row = 0; row < listNames.size(); row++) {
        kernelObjectID = getEquippedGear(characterRecordArrayIndex, row + 1);
        fetchedName = getNameFromRelativeID(kernelObjectID, row + 1);
        updateText(getChild(topWidget, listNames[row]), fetchedName);
    }
}

void handleUpdateEquipDescription(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    u8 characterRecordArrayIndex = getCharacterRecordIndex(*EQUIP_MENU_PARTY_INDEX);
    u16 kernelObjectID;
    const char * fetchedDescription;

    auto gearDescWidget = getChild(menuWidget, GEAR_DESC_WIDGET_NAME);
    auto descrptionWidget = getChild(gearDescWidget, GEAR_DESCRIPTION);


    auto slotChoice = getStateCursor(event->menu, 0)->context;
    if (event->menuState == 1) {
        auto gearChoice = getStateCursor(event->menu, 1)->context;
        kernelObjectID = gContext.gearViewData.getResource(gearChoice.baseRowIndex + gearChoice.relativeRowIndex).relative_item_id;
    }
    else {
        kernelObjectID = getEquippedGear(characterRecordArrayIndex, slotChoice.relativeRowIndex + 1);
    }

    fetchedDescription = getDescriptionFromRelativeID(kernelObjectID, slotChoice.relativeRowIndex + 1); 
    updateText(descrptionWidget, fetchedDescription);
}

void handleUpdateGearSlotsWidget(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    u8 characterRecordArrayIndex = getCharacterRecordIndex(*EQUIP_MENU_PARTY_INDEX);
    u16 kernelObjectID;
    u8* materiaSlots;
    u8 materiaGrowth;

    auto gearSlotsWidget = getChild(menuWidget, GEAR_SLOTS_WIDGET_NAME);
    auto materiaSlotsWidget = getChild(gearSlotsWidget, GEAR_SLOTS);
    auto growthWidget = getChild(gearSlotsWidget, GEAR_GROWTH);


    auto slotChoice = getStateCursor(event->menu, 0)->context;
    if (event->menuState == 1) {
        auto gearChoice = getStateCursor(event->menu, 1)->context;
        kernelObjectID = gContext.gearViewData.getResource(gearChoice.baseRowIndex + gearChoice.relativeRowIndex).relative_item_id;
    }
    else {
        kernelObjectID = getEquippedGear(characterRecordArrayIndex, slotChoice.relativeRowIndex + 1);
    }

    switch (slotChoice.relativeRowIndex) {
        case 0: {
            materiaSlots = &(gContext.weapons.getResource(kernelObjectID).gameWeapon.materia_slots[0]);
            materiaGrowth = gContext.weapons.getResource(kernelObjectID).gameWeapon.materia_growth;
            break;
        }
        case 1: {
            materiaSlots = &(gContext.armors.getResource(kernelObjectID).gameArmor.materia_slots[0]);
            materiaGrowth = gContext.armors.getResource(kernelObjectID).gameArmor.materia_growth;
            break;
        }
        default: {
            materiaSlots = nullptr;
            materiaGrowth = 4;
        }
    }

    updateMateriaSlots(materiaSlotsWidget, materiaSlots);

    if (materiaGrowth < 0 || materiaGrowth > 3) //display any invalid materia growth as "None"
        materiaGrowth = 4;

    const char * menuText = gContext.gameStrings.equipMenuTexts.get_string(materiaGrowth + 11);
    i32 growthTypeY = equipMenuWindowConfig[2].drawDistance2 + 64;
    i32 growthTypeX = sub_6F54A2((u8*)menuText);
    moveWidget(growthWidget, 243 - growthTypeX / 2, growthTypeY);
    updateText(growthWidget, menuText);
}

void handleUpdateStatMenuWidget(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    u8 characterRecordArrayIndex = getCharacterRecordIndex(*EQUIP_MENU_PARTY_INDEX);
    u8 statsToDisplay[7];
    auto statDiffWidget = getChild(menuWidget, STAT_DIFF_WIDGET_NAME);

    if (event->menuState == 1) {
        auto slotChoice = getStateCursor(event->menu, 0)->context;
        auto gearChoice = getStateCursor(event->menu, 1)->context;
        switch (slotChoice.relativeRowIndex) {
            case 0: {
                auto toEquipWeaponID = gContext.gearViewData.getResource(gearChoice.baseRowIndex + gearChoice.relativeRowIndex).relative_item_id;
                statsToDisplay[0] = gContext.weapons.getResource(toEquipWeaponID).gameWeapon.weapon_strength;
                statsToDisplay[1] = gContext.weapons.getResource(toEquipWeaponID).gameWeapon.weaponHitRate;
                std::vector<std::string> listNames = { NEW_STAT_VALUE_1, NEW_STAT_VALUE_2 };
                std::vector<std::string> currentGearNames = { STAT_VALUE_1, STAT_VALUE_2 };
                for (u32 row = 0; row < listNames.size(); row++) {
                    //auto currentStat = getNumber(getChild(statDiffWidget, currentGearNames[row]));
                    //updateNumberColor(getChild(statDiffWidget, listNames[row]), getStatDisplayColor(currentStat, statsToDisplay[row]));
                    updateNumber(getChild(statDiffWidget, listNames[row]), statsToDisplay[row]);
                }
                break;
            }
            case 1: {
                auto toEquipArmorID = gContext.gearViewData.getResource(gearChoice.baseRowIndex + gearChoice.relativeRowIndex).relative_item_id;
                statsToDisplay[2] = gContext.armors.getResource(toEquipArmorID).gameArmor.defense;
                statsToDisplay[3] = gContext.armors.getResource(toEquipArmorID).gameArmor.evade;
                statsToDisplay[4] = 0;
                statsToDisplay[5] = gContext.armors.getResource(toEquipArmorID).gameArmor.magic_defense;
                statsToDisplay[6] = gContext.armors.getResource(toEquipArmorID).gameArmor.magic_evade;
                std::vector<std::string> listNames = { NEW_STAT_VALUE_3, NEW_STAT_VALUE_4, NEW_STAT_VALUE_5, NEW_STAT_VALUE_6, NEW_STAT_VALUE_7 };
                std::vector<std::string> currentGearNames = { STAT_VALUE_3, STAT_VALUE_4, STAT_VALUE_5, STAT_VALUE_6, STAT_VALUE_7 };
                for (u32 row = 0; row < listNames.size(); row++) {
                    //auto currentStat = getNumber(getChild(statDiffWidget, currentGearNames[row]));
                    //updateNumberColor(getChild(statDiffWidget, listNames[row]), getStatDisplayColor(currentStat, statsToDisplay[row]));
                    updateNumber(getChild(statDiffWidget, listNames[row]), statsToDisplay[row + 2]);
                }
                break;
            }
            default: {
                break;
            }
        }
        return;
    }

    u16 equippedArmorID = (CHARACTER_RECORD_ARRAY)[characterRecordArrayIndex].equipped_armor;
    u16 equippedWeaponID = (CHARACTER_RECORD_ARRAY)[characterRecordArrayIndex].equipped_weapon;

    std::vector<std::string> listNames = { NEW_STAT_VALUE_1, NEW_STAT_VALUE_2, NEW_STAT_VALUE_3, NEW_STAT_VALUE_4, NEW_STAT_VALUE_5, NEW_STAT_VALUE_6, NEW_STAT_VALUE_7 };
    std::vector<std::string> baseNames = { STAT_VALUE_1, STAT_VALUE_2, STAT_VALUE_3, STAT_VALUE_4, STAT_VALUE_5, STAT_VALUE_6, STAT_VALUE_7 };
    statsToDisplay[0] = gContext.weapons.getResource(equippedWeaponID).gameWeapon.weapon_strength;
    statsToDisplay[1] = gContext.weapons.getResource(equippedWeaponID).gameWeapon.weaponHitRate;
    statsToDisplay[2] = gContext.armors.getResource(equippedArmorID).gameArmor.defense;
    statsToDisplay[3] = gContext.armors.getResource(equippedArmorID).gameArmor.evade;
    statsToDisplay[4] = 0;
    statsToDisplay[5] = gContext.armors.getResource(equippedArmorID).gameArmor.magic_defense;
    statsToDisplay[6] = gContext.armors.getResource(equippedArmorID).gameArmor.magic_evade;
    for (u32 row = 0; row < listNames.size(); row++) {
        updateNumber(getChild(statDiffWidget, listNames[row]), statsToDisplay[row]);
        updateNumber(getChild(statDiffWidget, baseNames[row]), statsToDisplay[row]);
    }

}

void enableListWidget(const MenuDrawEvent* event) {
    auto widget = getChild(getChild(event->menu->menuWidget, GEAR_LIST_WIDGET_NAME), EQUIP_LIST);
    switch (event->menuState) {
        case 0:
            disableWidget(widget);
            break;
        case 1:
            enableWidget(widget);
            break;
        default: {
        }
    }
}

color getStatDisplayColor(u8 equippedStat, u8 toEquipStat) {
    if (toEquipStat == equippedStat) {
        return COLOR_WHITE;
    }
    if (toEquipStat > equippedStat) {
        return COLOR_GREEN;
    }
    if (toEquipStat < equippedStat) {
        return COLOR_RED;
    }
    return COLOR_WHITE;
}

