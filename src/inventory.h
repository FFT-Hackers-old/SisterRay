#ifndef INVENTORY_H
#define INVENTORY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>

#define INVENTORY_SIZE (i16)1024 //maximum allowable inventory size within the menu engine

/*This is the in-battle copy of the item inventory*/
typedef struct {
    u16 item_id;
    u8 quantity;
    u8 targetFlags;
    u8 restrictionMask;
    u8 padding;
} BattleInventoryEntry;

class SrBattleInventory : public SrResourceRegistry<BattleInventoryEntry> {
public:
    u16 slots_in_use;
    SrBattleInventory(i16 allocation_size);
    SrBattleInventory(): SrResourceRegistry<BattleInventoryEntry>(){}
    void setSlotsInUse(u16 slotsInUse);
};

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

void testFillInventory();
void addInventoryEntry(u16 item_id, u8 quantity);

class SrItemInventory : public SrResourceRegistry<InventoryEntry> {
public:
    SrItemInventory(i16 allocation_size);
    SrItemInventory(): SrResourceRegistry<InventoryEntry>(){}
    void handle_decrement_inventory(u16 inventory_index, u8 decrement_quantity);
};

// indexed by global "item_id"
typedef struct {
    u8 item_type; //00 = normal item, 01 = weapon, 02=armor, 03=accessory
    u16 type_relative_id;
} ItemTypeData;

class SrItemTypeRegistry : public SrResourceRegistry<ItemTypeData> {
public:
    SrItemTypeRegistry() : SrResourceRegistry<ItemTypeData>::SrResourceRegistry() {};
    void initialize_augmented_data(u8 item_type, u16 number_to_initialize);
};

SISTERRAY_API void InitInventory();
SISTERRAY_API void InitBattleInventory();
SISTERRAY_API void init_item_type_data();

/*utility for decrementing the quantity of an item at a particular inventory index*/
SISTERRAY_API i16 sort_inventory(i32 sort_type);
bool usableInInventoryMenu(u16 item_id);
u16 get_restriction_mask(u16 item_id);
u8 get_target_flags(u16 item_id);
void updateInventoryEntry(u16 item_id, u8 quantity);

#endif
