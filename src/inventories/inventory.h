#ifndef INVENTORY_H
#define INVENTORY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "sr_inventory_template.h"
#include <vector>

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

// indexed by global "item_id"
typedef struct {
    u8 item_type; //00 = normal item, 01 = weapon, 02=armor, 03=accessory
    u16 type_relative_id;
    u8 itemIconType; //00 = normal item, 01 = sword 02= glove, etc
} ItemTypeData;

class SrItemTypeRegistry : public SrResourceRegistry<ItemTypeData> {
public:
    std::vector<i16> reverseItemRegistry;
    std::vector<i16> reverseArmorRegistry;
    std::vector<i16> reverseAccessoryRegistry;
    std::vector<i16> reverseWeaponRegistry;
    SrItemTypeRegistry() : SrResourceRegistry<ItemTypeData>::SrResourceRegistry() {};
    void initialize_augmented_data(u8 itemType, u16 numberToInitialize);
    u16 get_absolute_id(u8 itemType, u8 relativeIndex);
};

u8 getIconTypeFromItemType(u8 itemType, u16 typeRelativeIndex);

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

SISTERRAY_API void initInventory();
SISTERRAY_API void initItemTypeData();

#endif
