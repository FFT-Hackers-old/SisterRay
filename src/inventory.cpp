#include <stdlib.h>
#include <string.h>
#include "impl.h"
#include "inventory.h"

/*utility for decrementing the quantity of an item at a particular inventory index*/
void SrItemInventory::handle_decrement_inventory(u16 inventory_index, u8 decrement_quantity) {
    if (SrItemInventory::resource_registry[inventory_index].quantity + decrement_quantity > 1) {
        SrItemInventory::resource_registry[inventory_index].quantity = SrItemInventory::resource_registry[inventory_index].quantity - decrement_quantity;
    }
    else {
        SrItemInventory::resource_registry[inventory_index].item_id = 0xFFFF;
        SrItemInventory::resource_registry[inventory_index].quantity = 0;
    }
}

/*Method to initialize the new inventories with an appropriate size*/
SISTERRAY_API void InitInventory()
{
    gContext.inventory = SrItemInventory(INVENTORY_SIZE);
    srLogWrite("sister ray: inventory initialized with size: ", (unsigned long)gContext.inventory.resource_count());
}

SISTERRAY_API void InitBattleInventory()
{
    gContext.battle_inventory = SrBattleInventory(INVENTORY_SIZE);
    srLogWrite("sister ray: in_battle inventory initialized with size: ", (unsigned long)gContext.battle_inventory.resource_count());
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
static void initialize_augmented_data(u8 item_type) {
    ItemTypeData item_type_data;
    for (int i = 0; i < gContext.accessories.count; i++) {
        ItemTypeData item_type_data{ item_type, (u16)i };
        gContext.item_type_data.add_resource(item_type_data);
    }
}
