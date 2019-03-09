#include "equip_menu.h"
#include "../impl.h"
#include "menu_utils.h"
#include "../party/party_utils.h"

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
            sub_6FA12F(316, 171, 324, 303);
            sideScrollerArguments* arguments = word_DCA490;
            setSideScrollerArguments(arguments, 8, equippableGearCount, cursorContextArray[1].baseRowIndex, 618, 171, 17, 303);
            renderMenuSideScroller(arguments, 0.2);
            sub_6FA347();
            break;
        }
    }

    //HP bars and the like
    sub_707891(53, *(i16*)(dword_DCA4A0 + 26) + 26); //Now we display the currently equipped weapons kernel data
    renderHPAndStatus(110, 17, *EQUIP_MENU_PARTY_INDEX, 1045220557);
    sub_6E6C5B(17, 9, *EQUIP_MENU_PARTY_INDEX, 1045220557);

    //Draw Menu Boxes -- this part can be data driven
    for (u32 boxDataIndex = 0; boxDataIndex < 2; ++boxDataIndex) //index into box data structs and draw dem boxes
        drawMenuBox((u16*)(dword_DCA4A0 + 8 * boxDataIndex), 1050253722);
    drawMenuBox((u16*)(dword_DCA4A0 + 16), 1053609165);
    drawMenuBox((u16*)(dword_DCA4A0 + 24), 1056964608);
    drawMenuBox((u16*)(dword_DCA4A0 + 32), 1058642330);
}

void displayMenuTexts(cursorContext* cursorContextArray, u16 menuState, u32 stateControlMask) {
    u16 kernelObjectID;
    u16 baseRowIndex;
    u16 equippableGearCount;
    char* fetchedName;
    char* fetchedDescription;
    u16 maxRowsInView;
    u8 characterRecordArrayIndex;


    characterRecordArrayIndex = (RECYCLE_SLOT_OFFSET_TABLE)[(CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX]];
    //Display state specific strings
    switch (menuState) {
        case 0:{
            break;
        }
        case 1:{
            equippableGearCount = gContext.gear_view_data->slots_in_use;
            if (equippableGearCount <= 8)
                maxRowsInView = equippableGearCount;
            else
                maxRowsInView = 8;
            if (*dword_DCA628)
                maxRowsInView++;

            baseRowIndex = cursorContextArray[1].baseRowIndex;
            for (i32 visibleRow = 0; visibleRow < maxRowsInView; ++visibleRow) {
                kernelObjectID = gContext.gear_view_data->get_resource(cursorContextArray[1].baseRowIndex + visibleRow);
                fetchedName = getNameFromRelativeID(kernelObjectID, cursorContextArray[0].relativeRowIndex + 1); //relative row here is offset by 1 from item_type
                displayTextAtLocation(427, 36 * visibleRow + 9 * baseRowIndex + 193, fetchedName, 7u, 1045220557);
            }
            break;
        }
        default: {
            break;
        }
    }

    for (i32 m = 0; m < 3; ++m)
        displayTextAtLocation(250, 34 * m + 13, &byte_9209A8[12 * m], 5u, 1045220557); // display "weapon" "armor" and "accessory"

    //display strings common to all states
    kernelObjectID = getEquippedGear(characterRecordArrayIndex, cursorContextArray[0].relativeRowIndex + 1);
    fetchedName = getNameFromRelativeID(kernelObjectID);
    switch (cursorContextArray[0].relativeRowIndex) { //Refactor later after constants moved into enums/data structures
        case 0: {
            displayTextAtLocation(303, 13, fetchedName, 7u, 1045220557);
            break;
        }
        case 1: {
            displayTextAtLocation(303, 47, fetchedName, 7u, 1045220557);
            break;
        }
        case 2: {
            displayTextAtLocation(303, 81, fetchedName, 7u, 1045220557);
            break;
        }
        default: {
            break;
        }
    }

    if (*EQUIP_MENU_STATE == 1) { // display the descritpion of the current item based on menu state
        kernelObjectID = gContext.gear_view_data->get_resource(cursorContextArray[1].baseRowIndex + cursorContextArray[1].relativeRowIndex).relative_item_id;
    }
    else {
        kernelObjectID = getEquippedGear(characterRecordArrayIndex, cursorContextArray[0].relativeRowIndex + 1);
    }
    fetchedDescription = getDescriptionFromRelativeID(kernelObjectID, cursorContextArray[0].relativeRowIndex + 1); //relative row here is offset by 1 from item_type
    displayTextAtLocation(24, *(u16*)(dword_DCA4A0 + 10) + 13, fetchedDescription, 7u, 1045220557);
}

void displayMenuCursors(cursorContext* cursorContextArray, u16 menuState, u32 stateControlMask) {
    switch (menuState) {
        case 0: {
            displayCursor(207, 36 * cursorContextArray[0].relativeRowIndex + 17, 0.101);
            break;
        }
        case 1: {
            if (stateControlMask & 2) //animate flashing cursor
                displayCursor(207, 36 * cursorContextArray[0].relativeRowIndex + 17, 0.101);
            displayCursor(385, 36 * cursorContextArray[1].relativeRowIndex + 197, 0.101);
            break;
        }
        default: {
            break;
        }
    }
}

void displayMateriaSlots(cursorContext* cursorContextArray, u16 menuState, u32 stateControlMask) {
    u8 characterRecordArrayIndex = (RECYCLE_SLOT_OFFSET_TABLE)[(CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX]];
    u16 kernelObjectID;
    u8 materiaGrowth;
    u8* materiaSlots;
    switch (menuState) {
        case 0:
            kernelObjectID = getEquippedGear(characterRecordArrayIndex, cursorContextArray[0].relativeRowIndex + 1);
            break;
        case 1:
            kernelObjectID = gContext.gear_view_data->get_resource(cursorContextArray[1].baseRowIndex + cursorContextArray[1].relativeRowIndex).relative_item_id);
            break;
        default: {
        }
    }

    if (cursorContextArray[0].relativeRowIndex != 2) {
        displayTextAtLocation(27, *(u16*)(dword_DCA4A0 + 18) + 21, a3lot_0, 5u, 1036966167);
        displayTextAtLocation(27, *(u16*)(dword_DCA4A0 + 18) + 64, aRowth, 5u, 1036966167);
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
        sub_70760F(153, *(i16*)(dword_DCA4A0 + 18) + 21, (i32)materiaSlots);
        if (materiaGrowth < 0 || materiaGrowth > 3) //display any invalid materia growth as "None"
            materiaGrowth = 4;
        v8 = *(u16*)(dword_DCA4A0 + 18) + 64;
        v9 = sub_6F54A2(&byte_9209A8[12 * (materiaGrowth + 19)]);
        displayTextAtLocation(243 - v9 / 2, v8, &byte_9209A8[12 * (materiaGrowth + 19)], 7u, 1045220557); //we do some materia growth text right here
    }
}

void handleEquipMenuInput(i32 updateStateMask) {
    cursorContext* cursorContextArray = (cursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;
    characterRecord* characterRecordArray = CHARACTER_RECORD_ARRAY;
    u32 equipSlotRelativeRow = cursorContextArray[0].relativeRowIndex;
    u32 equipMenuState = *EQUIP_MENU_STATE;
    i32 cursorViewBound = 0;
    u16 equippableGearCount = 0;
    u16 removedGearAbsoluteIndex;
    u16 removedGearRelativeIndex;
    u8 characterRecordArrayIndex;
    u8 equippedGearItemType;


    characterRecordArrayIndex = (RECYCLE_SLOT_OFFSET_TABLE)[(CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX]];
    handleCursorPositionUpdate((u32*)(&(cursorContextArray[equipMenuState])));
    if (equipMenuState == 1) {
        if (checkInputReceived(32)) {
            playMenuSound(447);
            *EQUIP_MENU_STATE = 0;
            u8 equippedGearRelativeIndex = gContext.gear_view_data->get_resource(cursorContextArray[1].baseRowIndex + cursorContextArray[1].relativeRowIndex).relative_item_id;

            switch (equipSlotRelativeRow) { //
                case 0: { //equip WEAPON
                    equippedGearItemType = 1;
                    handleEquipGear(characterRecordArray, characterRecordArrayIndex, equippedGearItemType, equippedGearRelativeIndex);
                    break;
                }
                case 1: { //equip ARMOR
                    equippedGearItemType = 2;
                    handleEquipGear(characterRecordArray, characterRecordArrayIndex, equippedGearItemType, equippedGearRelativeIndex);
                    break;
                }
                case 2: { //equip Accessory
                    equippedGearItemType = 3;
                    handleEquipGear(characterRecordArray, characterRecordArrayIndex, equippedGearItemType, equippedGearRelativeIndex);
                    break;
                }
                default: {
                }
            }
            recalculateBaseStats(*EQUIP_MENU_PARTY_INDEX);
            recalculateDerivedStats(*EQUIP_MENU_PARTY_INDEX);
            updateMiscPartyStats();
        }
        else if (checkInputReceived(64))
        {
            *EQUIP_MENU_STATE = 0;
        }
    }
    else {
        if (checkInputReceived(4)) {
            do {
                *EQUIP_MENU_PARTY_INDEX = (((*EQUIP_MENU_PARTY_INDEX) - 1) < 0) ? 2 : (*EQUIP_MENU_PARTY_INDEX) - 1;
            } while ((CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX] == 0xFF);
        }
        else if (checkInputReceived(8)) {
            do {
                *EQUIP_MENU_PARTY_INDEX = (((*EQUIP_MENU_PARTY_INDEX) - 1) > 2) ? 0 : (*EQUIP_MENU_PARTY_INDEX) + 1;
            } while ((CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX] == 0xFF);
        }
        if (checkInputReceived(64)) {
            if (*DID_MATERIA_GEAR_CHANGE && (*word_DD1BC0 || *dword_DC1290)) {
                playMenuSound(1);
                sub_6C9812(4, 3);
                sub_6C6AEE(3);
                *VIEW_PERSISTENT_ACTOR_INDEX = *EQUIP_MENU_PARTY_INDEX;
            }
            else {
                playMenuSound(4);
                sub_6C9812(5, 0);
                sub_6C6AEE(0);
            }
        }
        else if (checkInputReceived(32)) {
            gContext.gear_view_data->setSlotsInUse(setupGearMenu(equippedGearItemType));
            equippableGearCount = gContext.gear_view_data->slots_in_use;
            if (equippableGearCount && !(*byte_DC0B4B & 1)) {
                playMenuSound(1);
                *EQUIP_MENU_STATE = 1;
                if (equippableGearCount <= 8) //Sets the "single view" size
                    cursorViewBound = equippableGearCount;
                else
                    cursorViewBound = 8;
                setContextCursorData((cursorContext*)(&cursorContextArray[1]), 0, 0, 1, cursorViewBound, 0, 0, 1, equippableGearCount, 0, 0, 0, 0, 0, 1);
            }
            else {
                playMenuSound(3);
            }
        }
        else if (checkInputReceived(128) && (*word_DD1BC0 || *dword_DC1290)) { //When switching to the materia view
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
}

//There are originally 32 slots for the equipment view, this interfaces with a new global 
u16 setupGearMenu(u8 itemType) {
    return 0;
}

void handleEquipGear(characterRecord* characterRecordArray, u32 characterRecordArrayIndex, u8 gearType, u8 equippedGearRelativeIndex) {
    u8 removedGearRelativeID;
    u16 removedGearAbsoluteID;
    u16 equippedGearAbsoluteID;

    switch (gearType) {
    case 1: {
        removedGearRelativeID = characterRecordArray[characterRecordArrayIndex].equipped_weapon;
        removedGearAbsoluteID = gContext.itemTypeData.get_absolute_id(gearType, removedGearRelativeID);
        equippedGearAbsoluteID = gContext.itemTypeData.get_absolute_id(gearType, equippedGearRelativeIndex);
        characterRecordArray[characterRecordArrayIndex].equipped_weapon = equippedGearRelativeIndex;
        handleMateriaUpdate(characterRecordArray[characterRecordArrayIndex], gearType, equippedGearRelativeIndex);
    }
    case 2: {
        removedGearRelativeID = characterRecordArray[characterRecordArrayIndex].equipped_armor;
        removedGearAbsoluteID = gContext.itemTypeData.get_absolute_id(gearType, removedGearRelativeID);
        equippedGearAbsoluteID = gContext.itemTypeData.get_absolute_id(gearType, equippedGearRelativeIndex);
        characterRecordArray[characterRecordArrayIndex].equipped_armor = equippedGearRelativeIndex;
        handleMateriaUpdate(characterRecordArray[characterRecordArrayIndex], gearType, equippedGearRelativeIndex);
    }
    case 3: {
        removedGearRelativeID = characterRecordArray[characterRecordArrayIndex].equipped_accessory;
        removedGearAbsoluteID = gContext.itemTypeData.get_absolute_id(gearType, removedGearRelativeID);
        equippedGearAbsoluteID = gContext.itemTypeData.get_absolute_id(gearType, equippedGearRelativeIndex);
        characterRecordArray[characterRecordArrayIndex].equipped_accessory = equippedGearRelativeIndex;
    }
    }

    *DID_MATERIA_GEAR_CHANGE = 1;
    gContext.inventory->incrementInventoryByItemID(removedGearAbsoluteID, 1);
    gContext.inventory->decrementInventoryByItemID(equippedGearAbsoluteID, 1);
}

//Update Materia after new items are equipped
void handleMateriaUpdate(characterRecord& activeCharacterRecord, u8 gearType, u16 gearRelativeIndex) {
    WeaponData newWeaponData;
    ArmorData newArmorData;
    AccessoryData newAccessoryData;
    u32* equippedMateriaData;
    u8* materiaSlots;
    bool shouldRemove = false;

    for (i32 materiaSlotIndex = 0; materiaSlotIndex < 8; ++materiaSlotIndex) {
        switch (gearType) {
        case 0: {
            newWeaponData = gContext.weapons.get_resource(gearRelativeIndex);
            materiaSlots = &(newWeaponData.materia_slots[0]);
            equippedMateriaData = (u32*)&(activeCharacterRecord.weapon_materia_slots);
            shouldRemove = (!(materiaSlots[materiaSlotIndex] && (activeCharacterRecord.weapon_materia_slots[materiaSlotIndex] != 0xFFFFFFFF)));
            break;
        }
        case 1: {
            newArmorData = gContext.armors.get_resource(gearRelativeIndex);
            materiaSlots = &(newArmorData.materia_slots[0]);
            equippedMateriaData = (u32*)&(activeCharacterRecord.armor_materia_slots);
            shouldRemove = (!(materiaSlots[materiaSlotIndex] && (activeCharacterRecord.armor_materia_slots[materiaSlotIndex] != 0xFFFFFFFF)));
            break;
        }
        default:{}
        }

        if (shouldRemove) {
            *byte_DC1148 = 0;
            gContext.materia_inventory->insertIntoMateriaInventory((MateriaInventoryEntry*)equippedMateriaData[materiaSlotIndex]); //put any materia removed back on, needs to work with the SR materia
            *byte_DC1148 = 0;
            equippedMateriaData[materiaSlotIndex] = 0xFFFFFFFF;
        }
    }
}
