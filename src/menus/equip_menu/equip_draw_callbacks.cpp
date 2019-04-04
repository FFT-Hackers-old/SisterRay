#include "equip_draw_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"

//-----------------------------------------------------Begin Widget Draw Callbacks---------------------------------------------------------//
using namespace EquipWidgetNames;

void handleChangeCharacter(const EquipDrawEventParams* params) {
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

void handleUpdateDescription(const EquipDrawEventParams* params) {
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

void handleUpdateGearSlotsWidget(const EquipDrawEventParams* params) {
    auto menuWidget = params->equipMenuWidget;
    cursorContext* cursorContextArray = (cursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;
    u8 characterRecordArrayIndex = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];
    u8 kernelObjectID;
    u8* materiaSlots;

    auto gearSlotsWidget = getChild(menuWidget, GEAR_SLOTS_WIDGET_NAME);
    auto materiaSlotsWidget = getChild(gearSlotsWidget, GEAR_SLOTS);
    auto growthWidget = getChild(gearSlotsWidget, GEAR_GROWTH);

    if (*EQUIP_MENU_STATE == 1) { // display the descritpion of the current item based on menu state
        kernelObjectID = gContext.gearViewData.get_resource(cursorContextArray[1].baseRowIndex + cursorContextArray[1].relativeRowIndex).relative_item_id;
    }
    else {
        kernelObjectID = getEquippedGear(characterRecordArrayIndex, cursorContextArray[0].relativeRowIndex + 1);
    }

    //Display current weapon slots & growth by default -- overridden in handlers to behave correctly
    materiaSlots = &(gContext.weapons.get_resource(kernelObjectID).materia_slots[0]);
    updateMateriaSlots(materiaSlotsWidget, materiaSlots);

    auto materiaGrowth = gContext.weapons.get_resource(kernelObjectID).materia_growth;
    if (materiaGrowth < 0 || materiaGrowth > 3) //display any invalid materia growth as "None"
        materiaGrowth = 4;
    char * menuText = gContext.game_strings.equipMenuTexts.get_string(materiaGrowth + 4);
    updateText(growthWidget, menuText);
}

void handleUpdateStatMenuWidget(const EquipDrawEventParams* params) {
    auto menuWidget = params->equipMenuWidget;
    cursorContext* cursorContextArray = (cursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;

}
//--------------------------------------------------End Widget Draw Callbacks---------------------------------------------------------//
