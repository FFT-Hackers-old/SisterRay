#include "equip_menu.h"
#include "../impl.h"
#include "menu_utils.h"

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
            default: {}
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
            equippableGearCount = setupGearMenu(equippedGearItemType);
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
