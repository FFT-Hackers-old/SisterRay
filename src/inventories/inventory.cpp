#include <stdlib.h>
#include <string.h>
#include "../impl.h"
#include "inventory.h"
#include <memory>

/*We want the inventory to construct itself with default (empty) values*/
SrItemInventory::SrItemInventory(i16 allocation_size): SrInventory<InventoryEntry>(allocation_size) {
    for (i32 i = 0; i < INVENTORY_SIZE; i++) {
        InventoryEntry entry = {
            0xFFFF,
            0
        };
        add_resource(entry);
    }
}

/*Construct with default values*/
SrGearViewData::SrGearViewData(i16 allocation_size) : SrResourceRegistry<GearViewData>(allocation_size) {
    for (i32 i = 0; i < INVENTORY_SIZE; i++) {
        GearViewData entry = {
            0
        };
        add_resource(entry);
    }
}

void SrGearViewData::setSlotsInUse(u16 slotsInUse) {
    slots_in_use = slotsInUse;
}

void initGearViewStorage() {
    gContext.gear_view_data = std::make_unique<SrGearViewData>((INVENTORY_SIZE)/2);
    srLogWrite("sister ray: initialized equip view with size: %lu", (unsigned long)gContext.gear_view_data->current_capacity());
}

/*Method to initialize the new inventories with an appropriate size*/
SISTERRAY_API void InitInventory()
{
    gContext.inventory = std::make_unique<SrItemInventory>(INVENTORY_SIZE);
    srLogWrite("sister ray: inventory initialized with capacity: %lu", (unsigned long)gContext.inventory->current_capacity());
}


/*New sorting method, no longer sorts by index ranges, but instead items actually have associated "types"*/
SISTERRAY_API i16 sort_inventory(i32 sort_type) {
    return 0;
}

/*Initialize the item_type mapping*/
SISTERRAY_API void init_item_type_data() {
    gContext.itemTypeData = SrItemTypeRegistry();
}

/*Probably should be moved inside the registry*/
void SrItemTypeRegistry::initialize_augmented_data(u8 item_type, u16 number_to_initialize) {
    ItemTypeData item_type_data;
    for (u16 i = 0; i < number_to_initialize; i++) {
        ItemTypeData item_type_data{ item_type, (u16)i };
        gContext.itemTypeData.add_resource(item_type_data);
    }
}
