#ifndef INVENTORY_H
#define INVENTORY_H

#include "sr_inventory_template.h"

#define INVENTORY_SIZE (i16)1024 //maximum allowable inventory size within the menu engine

/*This does not match the original format of an FFVII item
  In order to enable more items, a new format is being introduced and
  all routines which touch the inventory are being rewritten */
#pragma pack(push, 1)
typedef struct InventoryEntry{
    u16 item_id;
    u8 quantity;
    InventoryEntry(): item_id(0xFFFF), quantity(0) {
    };
    InventoryEntry(u16 item_id, u8 quantity): item_id(item_id), quantity(quantity) {
    };
} InventoryEntry;
#pragma pack(pop)

class SrItemInventory : public SrInventory<InventoryEntry> {
public:
    SrItemInventory(i16 allocation_size);
    SrItemInventory(): SrInventory<InventoryEntry>(){}
};

//The data structure stores information
typedef struct {
    u16 relative_item_id;
} GearViewData;

class SrGearViewData : public SrResourceRegistry<GearViewData> {
public:
    u8 itemType;
    u16 slots_in_use;
    SrGearViewData(i16 allocation_size, u8 itemType);
    SrGearViewData() : SrResourceRegistry<GearViewData>() {}
    void setSlotsInUse(u16 slotsInUse);
    void setItemType(u8 itemType);
    u8 getItemType();
};

SISTERRAY_API void initInventory()

#endif
