#include "inventory_utils.h"
#include "../impl.h"
#include "../gamedata/base_type_names.h"

/*Utility check if an item is usable on the menu*/
bool usableInInventoryMenu(u16 materiaID) {
    u16 restrictionMask = getRestrictionMask(materiaID);
    if (restrictionMask & 4) {
        return true;
    }
    return false;
}

void updateInventoryEntry(u16 materiaID, u8 quantity) {
    InventoryEntry entry = {
        materiaID,
        quantity
    };
    gContext.inventory->updateResource(materiaID, entry);
}

void addInventoryEntry(u16 materiaID, u8 quantity) {
    InventoryEntry entry = {
        materiaID,
        quantity
    };
    gContext.inventory->addResource(entry);
}

u16 getRestrictionMask(u16 absoluteID) {
    std::string itemType = gContext.baseItems.getResource(absoluteID).itemType;
    u16 relativeID = gContext.baseItems.getResource(absoluteID).typeRelativeID;
    u16 restrictionMask;
    if (itemType == ItemTypeNames::CONSUMABLE_TYPE) {
        restrictionMask = gContext.items.getResource(relativeID).gameItem.restrictionMask;
        return restrictionMask;
    }
    if (itemType == ItemTypeNames::WEAPON_TYPE){
        restrictionMask = gContext.weapons.getResource(relativeID).gameWeapon.restrictionMask;
        return restrictionMask;
    }
    if (itemType == ItemTypeNames::ARMOR_TYPE){
        restrictionMask = gContext.armors.getResource(relativeID).gameArmor.restrictionMask;
        return restrictionMask;
    }
    if (itemType == ItemTypeNames::ACCESSORY_TYPE) {
        restrictionMask = gContext.accessories.getResource(relativeID).gameAccessory.restrictionMask;
        return restrictionMask;
    }
    return 0;
}

u8 getTargetFlags(u16 absoluteItemID) {
    std::string itemType = gContext.baseItems.getResource(absoluteItemID).itemType;
    u16 relativeID = gContext.baseItems.getResource(absoluteItemID).typeRelativeID;
    u8 targetingFlags;
    if (itemType == ItemTypeNames::CONSUMABLE_TYPE) {
        targetingFlags = gContext.items.getResource(relativeID).gameItem.targetingFlags;
        return targetingFlags;
    }
    if (itemType == ItemTypeNames::WEAPON_TYPE) {
        targetingFlags = gContext.weapons.getResource(relativeID).gameWeapon.targetFlags;
        return targetingFlags;
    }
    if (itemType == ItemTypeNames::ARMOR_TYPE) {
        return 3;
    }
    if (itemType == ItemTypeNames::ACCESSORY_TYPE) {
        return 3;
    }
    return 0;
}

void testFillInventory() {
    for (u16 materiaID = 0; materiaID < 320; materiaID++) {
        updateInventoryEntry(materiaID, 99);
    }
}
