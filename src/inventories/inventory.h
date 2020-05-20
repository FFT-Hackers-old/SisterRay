#ifndef INVENTORY_H
#define INVENTORY_H

#include "sr_inventory_template.h"

#define INVENTORY_SIZE (i16)1024 //maximum allowable inventory size within the menu engine

/*This does not match the original format of an FFVII item
  In order to enable more items, a new format is being introduced and
  all routines which touch the inventory are being rewritten */
#pragma pack(push, 1)
typedef struct InventoryEntry{
    u16 materiaID;
    u8 quantity;
    InventoryEntry(): materiaID(0xFFFF), quantity(0) {};
    InventoryEntry(u16 materiaID, u8 quantity): materiaID(materiaID), quantity(quantity) {};
} InventoryEntry;
#pragma pack(pop)

class SrItemInventory : public SrInventory<InventoryEntry> {
public:
    SrItemInventory(i16 allocation_size);
    SrItemInventory(): SrInventory<InventoryEntry>(){}
};

//The data structure stores information
typedef struct {
    u16 relativeItemID;
} GearViewData;

class SrGearViewData : public SrResourceRegistry<GearViewData> {
public:
    SrGameGearType itemType;
    u16 slotsInUse;
    SrGearViewData(i16 allocation_size, SrGameGearType itemType);
    SrGearViewData() : SrResourceRegistry<GearViewData>() {}
    void setSlotsInUse(u16 slotsInUse);
    void setItemType(SrGameGearType itemType);
    SrGameGearType getItemType();
};

SISTERRAY_API void initInventory();

#endif
