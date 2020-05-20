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
        addResource(entry);
    }
}

/*Construct with default values*/
SrGearViewData::SrGearViewData(i16 allocation_size, SrGameGearType type) : SrResourceRegistry<GearViewData>(allocation_size) {
    for (i32 i = 0; i < allocation_size; i++) {
        GearViewData entry = {
            0
        };
        addResource(entry);
    }
    itemType = type;

}

void SrGearViewData::setSlotsInUse(u16 slotsInUse) {
    slotsInUse = slotsInUse;
}

void SrGearViewData::setItemType(SrGameGearType type) {
    itemType = type;
}

SrGameGearType SrGearViewData::getItemType() {
    return itemType;
}

void initGearViewStorage() {
    gContext.gearViewData = SrGearViewData();
    srLogWrite("sister ray: initialized equip view with size: %lu", (unsigned long)gContext.gearViewData.currentCapacity());
}

/*Method to initialize the new inventories with an appropriate size*/
SISTERRAY_API void initInventory()
{
    gContext.inventory = std::make_unique<SrItemInventory>(INVENTORY_SIZE);
    srLogWrite("sister ray: inventory initialized with capacity: %lu", (unsigned long)gContext.inventory->currentCapacity());
}


/*New sorting method, no longer sorts by index ranges, but instead items actually have associated "types"*/
i16 sortInventory(i32 sortType) {
    return 0;
}
