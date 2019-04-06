#include "equip_input_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
//-----------------------------------------------------Input Handler Callbacks-------------------------------------------------------------//

/*Handlers for "OK" inputs, one per menu State*/
void equipGearHandler(const EquipInputEventParams* params) {
    cursorContext* cursorContextArray = (cursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;
    characterRecord* characterRecordArray = CHARACTER_RECORD_ARRAY;
    u8 characterRecordArrayIndex;
    u8 equippedGearItemType;

    if (*EQUIP_MENU_STATE != 1)
        return;

    auto menuWidget = params->equipMenuWidget;

    playMenuSound(447);
    *EQUIP_MENU_STATE = 0;
    u8 equippedGearRelativeIndex = gContext.gearViewData.get_resource(cursorContextArray[1].baseRowIndex + cursorContextArray[1].relativeRowIndex).relative_item_id;

    switch (cursorContextArray[0].relativeRowIndex) { //
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

void selectGearHandler(const EquipInputEventParams* params) {
    cursorContext* cursorContextArray = (cursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;
    characterRecord* characterRecordArray = CHARACTER_RECORD_ARRAY;
    u32 equipMenuState = *EQUIP_MENU_STATE;
    i32 cursorViewBound = 0;
    u16 equippableGearCount = 0;

    if (*EQUIP_MENU_STATE != 0)
        return;

    auto menuWidget = params->equipMenuWidget;

    equippableGearCount = setupGearMenu(cursorContextArray[0].relativeRowIndex + 1);
    if (equippableGearCount && !(*byte_DC0B4B & 1)) {
        playMenuSound(1);
        *EQUIP_MENU_STATE = 1;
        if (equippableGearCount <= 8) //Sets the "single view" size
            cursorViewBound = equippableGearCount;
        else
            cursorViewBound = 8;
        setContextCursorData((cursorContext*)(&cursorContextArray[1]), 0, 0, 1, cursorViewBound, 0, 0, 1, equippableGearCount, 0, 0, 0, 0, 0, 1);
        // add the list widget (this widget updates it's own elements based on a cursorContext*)
        // 
    }
    else {
        playMenuSound(3);
    }

}

/*Handlers for "Cancel" inputs, one per menu State*/
void exitEquipViewListener(const EquipInputEventParams* params) {

    auto menuWidget = params->equipMenuWidget;
    
    *EQUIP_MENU_STATE = 0;
    //Remove Equip Menu Widgets for showing the List of Items
    //Reset stat diffs, descriptions, etc to currently equipped values
}

void exitMenuListener(const EquipInputEventParams* params) {
    if (*EQUIP_MENU_STATE != 0)
        return;

    auto menuWidget = params->equipMenuWidget;

    if (*DID_MATERIA_GEAR_CHANGE && (*word_DD1BC0 || *dword_DC1290)) {
        playMenuSound(1);
        sub_6C9812(4, 3);
        setActiveMenu(3);
        *VIEW_PERSISTENT_ACTOR_INDEX = *EQUIP_MENU_PARTY_INDEX;
    }
    else {
        playMenuSound(4);
        sub_6C9812(5, 0);
        setActiveMenu(0);
    }
}

/*Handlers for L1/R1 "switching" inputs, for states where they function*/
void changeCharLeft(const EquipInputEventParams* params) {

    auto menuWidget = params->equipMenuWidget;
 
    do {
        *EQUIP_MENU_PARTY_INDEX = (((i32)(*EQUIP_MENU_PARTY_INDEX) - 1) < 0) ? 2 : ((*EQUIP_MENU_PARTY_INDEX) - 1);
    } while ((CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX] == 0xFF);

    //update displayed character Data in the Widget
}

void changeCharRight(const EquipInputEventParams* params) {
    auto menuWidget = params->equipMenuWidget;

    do {
        *EQUIP_MENU_PARTY_INDEX = (((*EQUIP_MENU_PARTY_INDEX) + 1) > 2) ? 0 : ((*EQUIP_MENU_PARTY_INDEX) + 1);
    } while ((CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX] == 0xFF);

    //update displayed character Data in the Widget
}

//-----------------------------------------------------End Input Handler Callbacks-------------------------------------------------------------//


//-----------------------------------------------------End Widget Mutating Callbacks-----------------------------------------------------------//

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
        default: {}
        }

        if (shouldRemove) {
            *byte_DC1148 = 0;
            gContext.materia_inventory->insertIntoMateriaInventory((MateriaInventoryEntry*)equippedMateriaData[materiaSlotIndex]); //put any materia removed back on, needs to work with the SR materia
            *byte_DC1148 = 0;
            equippedMateriaData[materiaSlotIndex] = 0xFFFFFFFF;
        }
    }
}

/*There are originally 32 slots for the equipment view, this interfaces with a new global. We construct
  We do this by just constructing a new object and binding it to the gContext, since this is an emphemeral struct*/
u16 setupGearMenu(u8 itemType) {
    u8 characterID = (CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX];
    u16 equippableGearCount = 0;
    gContext.gearViewData = SrGearViewData();
    for (i32 inventoryEntry = 0; inventoryEntry < gContext.inventory->current_capacity(); inventoryEntry++) {
        if (gContext.itemTypeData.get_resource(inventoryEntry).item_type != itemType) {
            continue;
        }
        if (characterCanEquipItem(characterID, gContext.inventory->get_resource(inventoryEntry).item_id)) {
            GearViewData data = { gContext.itemTypeData.get_resource(inventoryEntry).type_relative_id };
            gContext.gearViewData.add_resource(data);
            equippableGearCount++;
        }
    }
    gContext.gearViewData.setItemType(itemType);
    gContext.gearViewData.setSlotsInUse(equippableGearCount);
    return equippableGearCount;
}
