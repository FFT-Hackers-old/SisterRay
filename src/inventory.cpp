#include <stdlib.h>
#include <string.h>
#include "impl.h"
#include "inventory.h"
#include <memory>

/*We want the inventory to construct itself with default (empty) values*/
SrItemInventory::SrItemInventory(i16 allocation_size): SrResourceRegistry<InventoryEntry>(allocation_size) {
    for (i32 i = 0; i < INVENTORY_SIZE; i++) {
        InventoryEntry entry = {
            0xFFFF,
            0
        };
        add_resource(entry);
    }
}

/*utility for decrementing the quantity of an item at a particular inventory index*/
void SrItemInventory::handle_decrement_inventory(u16 inventory_index, u8 decrement_quantity) {
    srLogWrite("decrementing inventory index %i", inventory_index);
    if (SrItemInventory::resource_registry[inventory_index].quantity + decrement_quantity > 1) {
        SrItemInventory::resource_registry[inventory_index].quantity = SrItemInventory::resource_registry[inventory_index].quantity - decrement_quantity;
    }
    else {
        SrItemInventory::resource_registry[inventory_index].item_id = 0xFFFF;
        SrItemInventory::resource_registry[inventory_index].quantity = 0;
    }
}

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

void SrBattleInventory::setSlotsInUse(u16 slotsInUse) {
    slots_in_use = slotsInUse;
}

void testFillInventory() {
    for (u16 item_id = 0; item_id < 320; item_id++) {
        updateInventoryEntry(item_id, 99);
    }
}

void updateInventoryEntry(u16 item_id, u8 quantity) {
    InventoryEntry entry = {
        item_id,
        quantity
    };
    gContext.inventory->update_resource(item_id, entry);
}

void addInventoryEntry(u16 item_id, u8 quantity) {
    InventoryEntry entry = {
        item_id,
        quantity
    };
    gContext.inventory->add_resource(entry);
}

/*Method to initialize the new inventories with an appropriate size*/
SISTERRAY_API void InitInventory()
{
    gContext.inventory = std::make_unique<SrItemInventory>(INVENTORY_SIZE);
    srLogWrite("sister ray: inventory initialized with capacity: %lu", (unsigned long)gContext.inventory->current_capacity());

}

SISTERRAY_API void InitBattleInventory()
{
    gContext.battle_inventory = std::make_unique<SrBattleInventory>(INVENTORY_SIZE);
    srLogWrite("sister ray: in_battle inventory initialized with capacity: %lu", (unsigned long)gContext.battle_inventory->current_capacity());
}

/*New sorting method, no longer sorts by index ranges, but instead items actually have associated "types"*/
SISTERRAY_API i16 sort_inventory(i32 sort_type) {
    return 0;
}

/*Initialize the item_type mapping*/
SISTERRAY_API void init_item_type_data() {
    gContext.item_type_data = SrItemTypeRegistry();
}

/*Probably should be moved inside the registry*/
void SrItemTypeRegistry::initialize_augmented_data(u8 item_type, u16 number_to_initialize) {
    ItemTypeData item_type_data;
    for (u16 i = 0; i < number_to_initialize; i++) {
        ItemTypeData item_type_data{ item_type, (u16)i };
        gContext.item_type_data.add_resource(item_type_data);
    }
}

/*Utility check if an item is usable on the menu*/
bool usable_in_inventory_menu(u16 item_id) {
    u16 restriction_mask = get_restriction_mask(item_id);
    if (restriction_mask & 4) {
        return true;
    }
    return false;
}

u16 get_restriction_mask(u16 item_id) {
    u8 item_type = gContext.item_type_data.get_resource(item_id).item_type;
    u16 relative_id = gContext.item_type_data.get_resource(item_id).type_relative_id;
    u16 restriction_mask;
    switch (item_type) {
    case 0: {
        restriction_mask = gContext.items.get_resource(relative_id).restriction_mask;
        break;
    }
    case 1: {
        restriction_mask = gContext.weapons.get_resource(relative_id).restriction_mask;
        break;
    }
    case 2: {
        restriction_mask = gContext.armors.get_resource(relative_id).restriction_mask;
        break;
    }
    case 3: {
        restriction_mask = gContext.accessories.get_resource(relative_id).restriction_mask;
        break;
    }
    default:
        restriction_mask = 0x00;
    }
    return restriction_mask;
}

u8 get_target_flags(u16 item_id) {
    u8 item_type = gContext.item_type_data.get_resource(item_id).item_type;
    u16 relative_id = gContext.item_type_data.get_resource(item_id).type_relative_id;
    u8 target_flags;
    switch (item_type) {
    case 0: {
        target_flags = gContext.items.get_resource(relative_id).target_flags;
        break;
    }
    case 1: {
        target_flags = gContext.weapons.get_resource(relative_id).target_flags;
        break;
    }
    case 2: {
        target_flags = 3;
        break;
    }
    case 3: {
        target_flags = 3;
        break;
    }
    default:
        target_flags = 0x00;
    }
    return target_flags;
}
