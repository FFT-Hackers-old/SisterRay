#include "equip_draw_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"

//-----------------------------------------------------Begin Widget Draw Callbacks---------------------------------------------------------//
using namespace EquipWidgetNames;

void handleChangeCharacter(const EquipDrawEvent* params) {
    auto menuWidget = params->equipMenuWidget;
    u8 kernelObjectID;
    char* fetchedName;

    u8 characterRecordArrayIndex = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];

    auto topWidget = getChild(menuWidget, CHAR_DATA_WIDGET_NAME);
    updatePortraitPartyIndex(getChild(topWidget, PORTRAIT_WIDGET_NAME), *EQUIP_MENU_PARTY_INDEX);
    updateHPBarPartyIndex(getChild(topWidget, HPBAR_WIDGET_NAME), *EQUIP_MENU_PARTY_INDEX);

    std::vector<std::string> listNames = { EQUIPPED_WEAPON, EQUIPPED_ARMOR, EQUIPPED_ACC };
    for (int row = 0; row < 0; row++) {
        kernelObjectID = getEquippedGear(characterRecordArrayIndex, row + 1);
        fetchedName = getNameFromRelativeID(kernelObjectID, row + 1);
        updateText(getChild(topWidget, listNames[row]), fetchedName);
    }
}

void handleUpdateDescription(const EquipDrawEvent* params) {
    auto menuWidget = params->equipMenuWidget;
    cursorContext* cursorContextArray = (cursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;
    u8 characterRecordArrayIndex = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];
    u8 kernelObjectID;
    char * fetchedDescription;

    auto gearDescWidget = getChild(menuWidget, GEAR_DESC_WIDGET_NAME);
    auto descrptionWidget = getChild(gearDescWidget, GEAR_DESCRIPTION);

    if (*EQUIP_MENU_STATE == 1) { // display the descritpion of the current item based on menu state
        kernelObjectID = gContext.gearViewData.get_resource(cursorContextArray[1].baseRowIndex + cursorContextArray[1].relativeRowIndex).relative_item_id;
    }
    else {
        kernelObjectID = getEquippedGear(characterRecordArrayIndex, cursorContextArray[0].relativeRowIndex + 1);
    }

    fetchedDescription = getDescriptionFromRelativeID(kernelObjectID, cursorContextArray[0].relativeRowIndex + 1); //relative row here is offset by 1 from item_type
    updateText(descrptionWidget, fetchedDescription);
}

void handleUpdateGearSlotsWidget(const EquipDrawEvent* params) {
    auto menuWidget = params->equipMenuWidget;
    cursorContext* cursorContextArray = (cursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;
    u8 characterRecordArrayIndex = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];
    u8 kernelObjectID;
    u8* materiaSlots;
    u8 materiaGrowth;

    auto gearSlotsWidget = getChild(menuWidget, GEAR_SLOTS_WIDGET_NAME);
    auto materiaSlotsWidget = getChild(gearSlotsWidget, GEAR_SLOTS);
    auto growthWidget = getChild(gearSlotsWidget, GEAR_GROWTH);

    if (*EQUIP_MENU_STATE == 1) {
        kernelObjectID = gContext.gearViewData.get_resource(cursorContextArray[1].baseRowIndex + cursorContextArray[1].relativeRowIndex).relative_item_id;
    }
    else {
        kernelObjectID = getEquippedGear(characterRecordArrayIndex, cursorContextArray[0].relativeRowIndex + 1);
    }

    switch (cursorContextArray[0].relativeRowIndex) {
        case 0: {
            materiaSlots = &(gContext.weapons.get_resource(kernelObjectID).materia_slots[0]);
            materiaGrowth = gContext.weapons.get_resource(kernelObjectID).materia_growth;
            break;
        }
        case 1: {
            materiaSlots = &(gContext.armors.get_resource(kernelObjectID).materia_slots[0]);
            materiaGrowth = gContext.armors.get_resource(kernelObjectID).materia_growth;
            break;
        }
        default: {
        }
    }

    updateMateriaSlots(materiaSlotsWidget, materiaSlots);

    if (materiaGrowth < 0 || materiaGrowth > 3) //display any invalid materia growth as "None"
        materiaGrowth = 4;
    char * menuText = gContext.game_strings.equipMenuTexts.get_string(materiaGrowth + 4);
    i32 growthTypeY = equipMenuWindowConfig[2].drawDistance2 + 64;
    i32 growthTypeX = sub_6F54A2((u8*)menuText);
    moveWidget(growthWidget, 243 - growthTypeX / 2, growthTypeY);
    updateText(growthWidget, menuText);
}

void handleUpdateStatMenuWidget(const EquipDrawEvent* params) {
    auto menuWidget = params->equipMenuWidget;
    cursorContext* cursorContextArray = (cursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;
    u8 characterRecordArrayIndex = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];
    u8 statsToDisplay[8];
    auto statDiffWidget = getChild(menuWidget, STAT_DIFF_WIDGET_NAME);

    if (*EQUIP_MENU_STATE == 1) {
        switch (cursorContextArray[0].relativeRowIndex) {
            case 0: {
                auto toEquipWeaponID = gContext.gearViewData.get_resource(cursorContextArray[1].baseRowIndex + cursorContextArray[1].relativeRowIndex).relative_item_id;
                statsToDisplay[0] = gContext.weapons.get_resource(toEquipWeaponID).weapon_strength;
                statsToDisplay[1] = gContext.weapons.get_resource(toEquipWeaponID).weapon_hit_rate;
                std::vector<std::string> listNames = { NEW_STAT_VALUE_1, NEW_STAT_VALUE_2 };
                for (int row = 0; row < 2; row++) {
                    updateNumber(getChild(statDiffWidget, listNames[row]), statsToDisplay[row]);
                }
                break;
            }
            case 1: {
                auto toEquipArmorID = gContext.gearViewData.get_resource(cursorContextArray[1].baseRowIndex + cursorContextArray[1].relativeRowIndex).relative_item_id;
                statsToDisplay[2] = gContext.armors.get_resource(toEquipArmorID).defense;
                statsToDisplay[3] = gContext.armors.get_resource(toEquipArmorID).evade;
                statsToDisplay[4] = 0;
                statsToDisplay[5] = gContext.armors.get_resource(toEquipArmorID).magic_defense;
                statsToDisplay[6] = gContext.armors.get_resource(toEquipArmorID).magic_evade;
                std::vector<std::string> listNames = { NEW_STAT_VALUE_3, NEW_STAT_VALUE_4, NEW_STAT_VALUE_5, NEW_STAT_VALUE_6, NEW_STAT_VALUE_7 };
                for (int row = 2; row < 7; row++) {
                    updateNumber(getChild(statDiffWidget, listNames[row]), statsToDisplay[row]);
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
    statsToDisplay[0] = gContext.weapons.get_resource(equippedWeaponID).weapon_strength;
    statsToDisplay[1] = gContext.weapons.get_resource(equippedWeaponID).weapon_hit_rate;
    statsToDisplay[2] = gContext.armors.get_resource(equippedArmorID).defense;
    statsToDisplay[3] = gContext.armors.get_resource(equippedArmorID).evade;
    statsToDisplay[4] = 0;
    statsToDisplay[5] = gContext.armors.get_resource(equippedArmorID).magic_defense;
    statsToDisplay[6] = gContext.armors.get_resource(equippedArmorID).magic_evade;
    for (int row = 0; row < 0; row++) {
        updateNumber(getChild(statDiffWidget, listNames[row]), statsToDisplay[row]);
    }

}
//--------------------------------------------------End Widget Draw Callbacks---------------------------------------------------------//
