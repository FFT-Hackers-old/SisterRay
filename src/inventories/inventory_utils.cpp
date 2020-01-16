#include "inventory_utils.h"
#include "../impl.h"

/*Utility check if an item is usable on the menu*/
bool usableInInventoryMenu(u16 item_id) {
    u16 restriction_mask = getRestrictionMask(item_id);
    if (restriction_mask & 4) {
        return true;
    }
    return false;
}

void updateInventoryEntry(u16 item_id, u8 quantity) {
    InventoryEntry entry = {
        item_id,
        quantity
    };
    gContext.inventory->updateResource(item_id, entry);
}

void addInventoryEntry(u16 item_id, u8 quantity) {
    InventoryEntry entry = {
        item_id,
        quantity
    };
    gContext.inventory->addResource(entry);
}

u16 getCharacterRestrictionMask(u16 absoluteItemID) {
    u8 item_type = gContext.itemTypeData.getResource(absoluteItemID).itemType;
    u16 relativeItemID = gContext.itemTypeData.getResource(absoluteItemID).typeRelativeID;
    u16 restriction_mask;
    switch (item_type) {
    case 0: {
        restriction_mask = gContext.items.getResource(relativeItemID).auxData.characterRestrictionMask;
        break;
    }
    case 1: {
        restriction_mask = gContext.weapons.getResource(relativeItemID).gameWeapon.equip_mask;
        break;
    }
    case 2: {
        restriction_mask = gContext.armors.getResource(relativeItemID).gameArmor.equip_mask;
        break;
    }
    case 3: {
        restriction_mask = gContext.accessories.getResource(relativeItemID).gameAccessory.equip_mask;
        break;
    }
    default:
        restriction_mask = 0x00;
    }
    return restriction_mask;
}

u16 getRestrictionMask(u16 absoluteID) {
    u8 itemType = gContext.itemTypeData.getResource(absoluteID).itemType;
    u16 relativeID = gContext.itemTypeData.getResource(absoluteID).typeRelativeID;
    u16 restrictionMask;
    switch (itemType) {
    case 0: {
        restrictionMask = gContext.items.getResource(relativeID).gameItem.restrictionMask;
        break;
    }
    case 1: {
        restrictionMask = gContext.weapons.getResource(relativeID).gameWeapon.restriction_mask;
        break;
    }
    case 2: {
        restrictionMask = gContext.armors.getResource(relativeID).gameArmor.restriction_mask;
        break;
    }
    case 3: {
        restrictionMask = gContext.accessories.getResource(relativeID).gameAccessory.restriction_mask;
        break;
    }
    default:
        restrictionMask = 0x00;
    }
    return restrictionMask;
}

u8 getTargetFlags(u16 absoluteItemID) {
    u8 item_type = gContext.itemTypeData.getResource(absoluteItemID).itemType;
    u16 relativeID = gContext.itemTypeData.getResource(absoluteItemID).typeRelativeID;
    u8 targetingFlags;
    switch (item_type) {
    case 0: {
        targetingFlags = gContext.items.getResource(relativeID).gameItem.targetingFlags;
        break;
    }
    case 1: {
        targetingFlags = gContext.weapons.getResource(relativeID).gameWeapon.targetFlags;
        break;
    }
    case 2: {
        targetingFlags = 3;
        break;
    }
    case 3: {
        targetingFlags = 3;
        break;
    }
    default:
        targetingFlags = 0x00;
    }
    return targetingFlags;
}

void testFillInventory() {
    for (u16 item_id = 0; item_id < 320; item_id++) {
        updateInventoryEntry(item_id, 99);
    }
}
