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
    gContext.gearViewData = SrGearViewData();
    srLogWrite("sister ray: initialized equip view with size: %lu", (unsigned long)gContext.gearViewData.current_capacity());
}

/*Method to initialize the new inventories with an appropriate size*/
SISTERRAY_API void initInventory()
{
    gContext.inventory = std::make_unique<SrItemInventory>(INVENTORY_SIZE);
    srLogWrite("sister ray: inventory initialized with capacity: %lu", (unsigned long)gContext.inventory->current_capacity());
}


/*New sorting method, no longer sorts by index ranges, but instead items actually have associated "types"*/
SISTERRAY_API i16 sort_inventory(i32 sort_type) {
    return 0;
}

/*Initialize the item_type mapping*/
SISTERRAY_API void initItemTypeData() {
    gContext.itemTypeData = SrItemTypeRegistry();
}

/*Probably should be moved inside the registry*/
void SrItemTypeRegistry::initialize_augmented_data(u8 itemType, u16 numberToInitialize) {
    ItemTypeData item_type_data;
    u16 absoluteBase = resource_count() - 1;
    u16 absoluteIndex;

    for (u16 i = 0; i < numberToInitialize; i++) {
        ItemTypeData item_type_data{ itemType, (u16)i };
        gContext.itemTypeData.add_resource(item_type_data);

        absoluteIndex = absoluteBase + i;
        switch (itemType) {
        case 0:
            reverseItemRegistry.push_back(absoluteIndex);
        case 1:
            reverseWeaponRegistry.push_back(absoluteIndex);
        case 2:
            reverseArmorRegistry.push_back(absoluteIndex);
        case 3:
            reverseAccessoryRegistry.push_back(absoluteIndex);
        }
    }
}

u16 SrItemTypeRegistry::get_absolute_id(u8 itemType, u8 relativeIndex) {
    switch (itemType) {
    case 0:
        return reverseItemRegistry.at(relativeIndex);
    case 1:
        return reverseWeaponRegistry.at(relativeIndex);
    case 2:
        return reverseArmorRegistry.at(relativeIndex);
    case 3:
        return reverseAccessoryRegistry.at(relativeIndex);
    }
}
