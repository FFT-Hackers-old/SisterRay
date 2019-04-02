#include "equip_menu_listeners.h"
#include "../impl.h"
#include "../party/party_utils.h"

//Handler for MENU_STATE = 1, inject any on equipEffects similarly
void equipGearHandler(const void* params) {
    cursorContext* cursorContextArray = (cursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;
    characterRecord* characterRecordArray = CHARACTER_RECORD_ARRAY;
    u32 equipMenuState = *EQUIP_MENU_STATE;
    u8 characterRecordArrayIndex;
    u8 equippedGearItemType;

    if (equipMenuState != 1)
        return;

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
