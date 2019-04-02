#include "equip_menu.h"
#include "../impl.h"
#include "../party/party_utils.h"
#include <memory>


SISTERRAY_API void equipMenuUpdateHandler(i32 updateStateMask) {
    displayEquipMenuViews(updateStateMask);
    if (!is_input_handling_enabled()) {
        handleEquipMenuInput(updateStateMask);
    }
}



//TODO: All of the magic numbers need to be pulled out and data driven
void displayEquipMenuViews(i32 stateControlMask) {
    cursorContext* cursorContextArray = (cursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;
    characterRecord* characterRecordArray = CHARACTER_RECORD_ARRAY;
    u8 characterRecordArrayIndex;

    sub_6C98A6();
    displayMenuObjects(cursorContextArray, *EQUIP_MENU_STATE, stateControlMask);
    displayMenuCursors(cursorContextArray, *EQUIP_MENU_STATE, stateControlMask);
    displayMateriaSlots(cursorContextArray, *EQUIP_MENU_STATE, stateControlMask);
    displayMenuTexts(cursorContextArray, *EQUIP_MENU_STATE, stateControlMask);
    displayEquipGearStats(cursorContextArray, *EQUIP_MENU_STATE, stateControlMask);
}

//This function can be registered to a particular menu state to modularize modding the menus
void displayMenuObjects(cursorContext* cursorContextArray, u32 menuState, i32 stateControlMask) {
    u16 kernelObjectID;
    u16 baseRowIndex;
    u16 equippableGearCount;
    char* fetchedName;
    char* fetchedDescription;
    u16 maxRowsInView;

    switch (menuState) {
        case 1: {
            //sub_6FA12F(316, 171, 324, 303); //I can't figure out what this does...
            equippableGearCount = gContext.gearViewData.slots_in_use;
            drawScrollerParams* arguments = (drawScrollerParams*)word_DCA490;
            setSideScrollerArguments(arguments, 8, equippableGearCount, cursorContextArray[1].baseRowIndex, 618, 171, 17, 303);
            renderMenuSideScroller(arguments, 0.2);
            //sub_6FA347();
            break;
        }
    }

    //HP bars and the like
    renderHPAndStatus(110, 17, *EQUIP_MENU_PARTY_INDEX, 1045220557); //HP bar
    displayPortrait(17, 9, *EQUIP_MENU_PARTY_INDEX, 1045220557); //Character Portrait

    //Draw Menu Boxes -- this part can be data driven
    for (u32 boxDataIndex = 0; boxDataIndex < 2; ++boxDataIndex) //index into box data structs and draw dem boxes
        gameDrawBox((i16*)(&(equipMenuWindowConfig)[boxDataIndex]), .3f);
    gameDrawBox((i16*)(&(equipMenuWindowConfig)[2]), .4f);
    gameDrawBox((i16*)(&(equipMenuWindowConfig)[3]), .5f);
    gameDrawBox((i16*)(&(equipMenuWindowConfig)[4]), .6f);
}

void displayMenuTexts(cursorContext* cursorContextArray, u16 menuState, u32 stateControlMask) {
    u16 kernelObjectID;
    u16 baseRowIndex;
    u16 baseColumnIndex;
    u16 equippableGearCount;
    char *menuText;
    char* fetchedName;
    char* fetchedDescription;
    u16 maxRowsInView;
    u8 characterID;


    characterID = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];
    //Display state specific strings
    switch (menuState) {
        case 0:{
            break;
        }
        case 1:{
            equippableGearCount = gContext.gearViewData.slots_in_use;
            if (equippableGearCount <= 8)
                maxRowsInView = equippableGearCount;
            else
                maxRowsInView = 8;
            if (*dword_DCA628)
                maxRowsInView++;

            baseRowIndex = cursorContextArray[1].baseRowIndex;
            baseColumnIndex = cursorContextArray[1].baseColumnIndex;
            for (i32 visibleRow = 0; visibleRow < maxRowsInView; ++visibleRow) {
                kernelObjectID = gContext.gearViewData.get_resource(cursorContextArray[1].baseRowIndex + visibleRow).relative_item_id;
                fetchedName = getNameFromRelativeID(kernelObjectID, cursorContextArray[0].relativeRowIndex + 1); //relative row here is offset by 1 from item_type
                displayTextAtLocation(427, 36 * visibleRow + 9 * baseColumnIndex + 193, fetchedName, COLOR_WHITE, 0.2f);
            }
            break;
        }
        default: {
            break;
        }
    }

    //Display weapon, armor, and accessory
    for (i32 menuTextIndex = 0; menuTextIndex < 3; ++menuTextIndex) {
        menuText = gContext.game_strings.equip_menu_texts.get_string(menuTextIndex);
        displayTextAtLocation(250, 34 * menuTextIndex + 13, menuText, COLOR_TEAL, 0.2f);
    }

    kernelObjectID = getEquippedGear(characterID, 1);
    fetchedName = getNameFromRelativeID(kernelObjectID, 1);
    displayTextAtLocation(303, 13, fetchedName, COLOR_WHITE, 0.2f);

    kernelObjectID = getEquippedGear(characterID, 2);
    fetchedName = getNameFromRelativeID(kernelObjectID, 2);
    displayTextAtLocation(303, 47, fetchedName, COLOR_WHITE, 0.2f);

    kernelObjectID = getEquippedGear(characterID, 3);
    fetchedName = getNameFromRelativeID(kernelObjectID, 3);
    displayTextAtLocation(303, 81, fetchedName, COLOR_WHITE, 0.2f);
    // End Texts for current Equip Widget  


    if (*EQUIP_MENU_STATE == 1) { // display the descritpion of the current item based on menu state
        kernelObjectID = gContext.gearViewData.get_resource(cursorContextArray[1].baseRowIndex + cursorContextArray[1].relativeRowIndex).relative_item_id;
    }
    else {
        kernelObjectID = getEquippedGear(characterID, cursorContextArray[0].relativeRowIndex + 1);
    }
    fetchedDescription = getDescriptionFromRelativeID(kernelObjectID, cursorContextArray[0].relativeRowIndex + 1); //relative row here is offset by 1 from item_type
    displayTextAtLocation(24, equipMenuWindowConfig[1].drawDistance2 + 13, fetchedDescription, COLOR_WHITE, 0.2f);
}

void displayMenuCursors(cursorContext* cursorContextArray, u16 menuState, u32 stateControlMask) {
    switch (menuState) {
        case 0: {
            displayCursor(207, 36 * cursorContextArray[0].relativeRowIndex + 17, 0.1f);
            break;
        }
        case 1: {
            if (stateControlMask & 2) //animate flashing cursor
                displayCursor(207, 36 * cursorContextArray[0].relativeRowIndex + 17, 0.1f);
            displayCursor(385, 36 * cursorContextArray[1].relativeRowIndex + 197, 0.1f);
            break;
        }
        default: {
            break;
        }
    }
}

void displayMateriaSlots(cursorContext* cursorContextArray, u16 menuState, u32 stateControlMask) {
    u8 characterRecordArrayIndex = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];
    u16 kernelObjectID;
    u8 materiaGrowth;
    u8* materiaSlots;
    char* menuText;

    switch (menuState) {
        case 0:
            kernelObjectID = getEquippedGear(characterRecordArrayIndex, cursorContextArray[0].relativeRowIndex + 1);
            break;
        case 1:
            kernelObjectID = gContext.gearViewData.get_resource(cursorContextArray[1].baseRowIndex + cursorContextArray[1].relativeRowIndex).relative_item_id;
            break;
        default: {
        }
    }

    if (cursorContextArray[0].relativeRowIndex != 2) {
        menuText = gContext.game_strings.equip_menu_texts.get_string(4);
        displayTextAtLocation(27, equipMenuWindowConfig[2].drawDistance2 + 21, menuText, COLOR_TEAL, 0.1f);
        menuText = gContext.game_strings.equip_menu_texts.get_string(5);
        displayTextAtLocation(27, equipMenuWindowConfig[2].drawDistance2 + 64, menuText, COLOR_TEAL, 0.1f);
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
        renderGearMateriaSlots(153, equipMenuWindowConfig[2].drawDistance2 + 21, materiaSlots); //render the materia slots, convert into a shared reusable utility and merge with materia handler
        if (materiaGrowth < 0 || materiaGrowth > 3) //display any invalid materia growth as "None"
            materiaGrowth = 4;
        menuText = gContext.game_strings.equip_menu_texts.get_string(materiaGrowth + 4);

        i32 growthTypeY = equipMenuWindowConfig[2].drawDistance2 + 64;
        i32 growthTypeX = sub_6F54A2((u8*)menuText);

        displayTextAtLocation(243 - growthTypeX / 2, growthTypeY, menuText, COLOR_WHITE, 0.2f); //we do some materia growth text right here
    }
}

//This is a wrapper function, the method that this points add will be rewritten and split up in the futre
void displayEquipGearStats(cursorContext* cursorContextArray, u16 menuState, u16 stateControlMask) {
    u8 characterID = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];
    u16 equippedWeaponID = (CHARACTER_RECORD_ARRAY)[characterID].equipped_weapon;
    u16 equippedArmorID = (CHARACTER_RECORD_ARRAY)[characterID].equipped_armor;
    u16 equippedAccessoryID = (CHARACTER_RECORD_ARRAY)[characterID].equipped_accessory;
    u16 toEquipWeaponID;
    u16 toEquipArmorID;
    u16 toEquipAccessoryID;
    u8 equippedStats[8];
    u8 statToEquip[8];
    u8 statDisplayColor;
    u16 windowTop = equipMenuWindowConfig[3].drawDistance2 + 26;
    char* menuText;

    for (i32 i = 0; i < 7; ++i) {
        menuText = gContext.game_strings.equip_menu_texts.get_string(5 + i);
        displayTextAtLocation(53, windowTop + 26 * i - 6, menuText, COLOR_TEAL, 0.2f);
    }
    for (i32 j = 0; j < 7; ++j)
        gameDrawAsset(53 + 194, 26 * j + windowTop, 0xDAu, 5u, 0.2f); //This function draws the arrows

    equippedStats[0] = gContext.weapons.get_resource(equippedWeaponID).weapon_strength;
    gameDrawNumbers(200, windowTop, equippedStats[0], 3, COLOR_WHITE, 0.2f);
    equippedStats[1] = gContext.weapons.get_resource(equippedWeaponID).weapon_hit_rate;
    gameDrawNumbers(200, windowTop + 26, equippedStats[1], 3, COLOR_WHITE, 0.2f);
    equippedStats[2] = gContext.armors.get_resource(equippedArmorID).defense;
    gameDrawNumbers(200, windowTop + 52, equippedStats[2], 3, COLOR_WHITE, 0.2f);
    equippedStats[3] = gContext.armors.get_resource(equippedArmorID).evade;
    gameDrawNumbers(200, windowTop + 78, equippedStats[3], 3, COLOR_WHITE, 0.2f);
    equippedStats[4] = 0;
    gameDrawNumbers(200, windowTop + 104, 0, 3, COLOR_WHITE, 0.2f);
    equippedStats[5] = gContext.armors.get_resource(equippedArmorID).magic_defense;
    gameDrawNumbers(200, windowTop + 130, equippedStats[4], 3, COLOR_WHITE, 0.2f);
    equippedStats[6] = gContext.armors.get_resource(equippedArmorID).magic_evade;
    gameDrawNumbers(200, windowTop + 154, equippedStats[5], 3, COLOR_WHITE, 0.2f);

    if (menuState == 1) {
        switch (cursorContextArray[0].relativeRowIndex) {
            case 0: {
                toEquipWeaponID = gContext.gearViewData.get_resource(cursorContextArray[1].baseRowIndex + cursorContextArray[1].relativeRowIndex).relative_item_id;
                statToEquip[0] = gContext.weapons.get_resource(toEquipWeaponID).weapon_strength;
                statToEquip[1] = gContext.weapons.get_resource(toEquipWeaponID).weapon_hit_rate;
                statToEquip[2] = equippedStats[2];
                statToEquip[3] = equippedStats[3];
                statToEquip[4] = 0;
                statToEquip[5] = equippedStats[4];
                statToEquip[6] = equippedStats[5];
                break;
            }
            case 1: {
                toEquipArmorID = gContext.gearViewData.get_resource(cursorContextArray[1].baseRowIndex + cursorContextArray[1].relativeRowIndex).relative_item_id;
                statToEquip[0] = equippedStats[0];
                statToEquip[1] = equippedStats[1];
                statToEquip[2] = gContext.armors.get_resource(toEquipArmorID).defense;
                statToEquip[3] = gContext.armors.get_resource(toEquipArmorID).evade;
                statToEquip[4] = 0;
                statToEquip[5] = gContext.armors.get_resource(toEquipArmorID).magic_defense;
                statToEquip[6] = gContext.armors.get_resource(toEquipArmorID).magic_evade;
                break;
            }
            default: {
                break;
            }
        }
        for (i32 stat = 0; stat < 7; stat++) {
            statDisplayColor = getStatDisplayColor(equippedStats[stat], statToEquip[stat]);
            gameDrawNumbers(270, windowTop + (26 * stat), statToEquip[stat], 3, statDisplayColor, 0.2f);
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
}

void handleEquipMenuInput(i32 updateStateMask) {
    cursorContext* cursorContextArray = (cursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;
    characterRecord* characterRecordArray = CHARACTER_RECORD_ARRAY;
    u32 equipMenuState = *EQUIP_MENU_STATE;
    i32 cursorViewBound = 0;
    u16 equippableGearCount = 0;
    u16 removedGearAbsoluteIndex;
    u16 removedGearRelativeIndex;
    u8 characterRecordArrayIndex;
    u8 equippedGearItemType;


    characterRecordArrayIndex = (RECYCLE_SLOT_OFFSET_TABLE)[((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX]];
    handleCursorPositionUpdate((u32*)(&(cursorContextArray[equipMenuState])));
    if (checkInputReceived(32)) {
        gContext.eventBus.dispatch(EQUIP_MENU_INPUT_OK, nullptr);
    }
    else if (checkInputReceived(64)) {
        gContext.eventBus.dispatch(EQUIP_MENU_INPUT_CANCEL, nullptr);
    }
    else if (checkInputReceived(4)) {
        gContext.eventBus.dispatch(EQUIP_MENU_INPUT_L1, nullptr);
    }
    else if (checkInputReceived(8)) {
        gContext.eventBus.dispatch(EQUIP_MENU_INPUT_R1, nullptr);
    }
    else if (checkInputReceived(128) && (*word_DD1BC0 || *dword_DC1290)) { //When switching to the materia view, square
        playMenuSound(1);
        sub_6C9812(4, 3);
        sub_6C6AEE(3);
        *VIEW_PERSISTENT_ACTOR_INDEX = *EQUIP_MENU_PARTY_INDEX;
    }
    else if (checkInputReceived(16) && !(*byte_DC0B4B & 1) && *dword_DCA5C4 == 2) { //unequip accessory
        playMenuSound(4);
        if (characterRecordArray[characterRecordArrayIndex].equipped_accessory != 0xFF) {
            removedGearRelativeIndex = characterRecordArray[characterRecordArrayIndex].equipped_accessory;
            removedGearAbsoluteIndex = gContext.itemTypeData.get_absolute_id(3, removedGearRelativeIndex);
            gContext.inventory->incrementInventoryByItemID(removedGearAbsoluteIndex, 1); //can only unequip
        }
        characterRecordArray[characterRecordArrayIndex].equipped_accessory = 0xFF;
        recalculateBaseStats(*EQUIP_MENU_PARTY_INDEX);
        recalculateDerivedStats(*EQUIP_MENU_PARTY_INDEX);
        updateMiscPartyStats();
    }
}
