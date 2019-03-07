#include <stdlib.h>
#include <string.h>
#include "../impl.h"
#include "battle_inventory.h"
#include <memory>

/*We want the Battle Inventory to construct itself with default (empty) values*/
SrBattleInventory::SrBattleInventory(i16 allocation_size) : SrResourceRegistry<BattleInventoryEntry>(allocation_size) {
    for (i32 i = 0; i < INVENTORY_SIZE; i++) {
        BattleInventoryEntry entry = {
            0xFFFF,
            0,
            0,
            0,
            0
        };
        add_resource(entry);
    }
}

/*utility for decrementing the quantity of an item at a particular inventory index*/
void SrBattleInventory::decrementInventoryEntry(u16 inventory_index, u8 stepSize) {
    if (resource_registry[inventory_index].quantity + stepSize > 1) {
        resource_registry[inventory_index].quantity = resource_registry[inventory_index].quantity - stepSize;
    }
    else {
        resource_registry[inventory_index].item_id = 0xFFFF;
        resource_registry[inventory_index].quantity = 0;
    }
}


void SrBattleInventory::incrementInventoryEntry(u16 inventory_index, u8 stepSize) {
    if (resource_registry[inventory_index].quantity + stepSize < 99) {
        resource_registry[inventory_index].quantity = resource_registry[inventory_index].quantity + stepSize;
    }
    else {
        resource_registry[inventory_index].quantity = 99;
    }
}

u16 SrBattleInventory::findItemInventoryIndex(u16 itemID) {
    u16 inventoryIndex;

    for (auto it = begin(resource_registry); it != end(resource_registry); ++it) {
        if ((*it).item_id == itemID) {
            return distance(resource_registry.begin(), it);
        }
    }
    return 0xFFFF;
}

//Utility function for incrementing an item from its absolute item ID
bool SrBattleInventory::incrementInventoryByItemID(u16 absoluteID, u8 stepSize) {
    u16 inventoryIndex = findItemInventoryIndex(absoluteID);
    if (inventoryIndex != 0xFFFF) {
        incrementInventoryEntry(inventoryIndex, stepSize);
        return true;
    }
    return false;
}

bool SrBattleInventory::decrementInventoryByItemID(u16 absoluteID, u8 stepSize) {
    u16 inventoryIndex = findItemInventoryIndex(absoluteID);
    if (inventoryIndex != 0xFFFF) {
        decrementInventoryEntry(inventoryIndex, stepSize);
        return true;
    }
    return false;
}

void SrBattleInventory::setSlotsInUse(u16 slotsInUse) {
    slots_in_use = slotsInUse;
}

SISTERRAY_API void InitBattleInventory()
{
    gContext.battle_inventory = std::make_unique<SrBattleInventory>(INVENTORY_SIZE);
    srLogWrite("sister ray: in_battle inventory initialized with capacity: %lu", (unsigned long)gContext.battle_inventory->current_capacity());
}
