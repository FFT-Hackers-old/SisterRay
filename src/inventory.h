#ifndef INVENTORY_H
#define INVENTORY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>

#define INVENTORY_SIZE 1024

/*This is the in-battle copy of the item inventory*/
typedef struct {
    u16 item_id;
    u8 quantity;
    u8 throwable;
    u8 restrictionMask;
    u8 padding;
} BattleInventoryEntry;

typedef struct {
    size_t          count;
    size_t          capacity;
    BattleInventoryEntry* data;
} SrBattleInventory;

/*This does not match the original format of an FFVII item
  In order to enable more items, a new format is being introduced and
  all routines which touch the inventory are being rewritten */
#pragma pack(push, 1)
typedef struct {
    u16 item_id;
    u8 quantity;
} InventoryEntry;
#pragma pack(pop)

typedef struct {
    size_t          capacity;
    InventoryEntry*    data;
} SrItemInventory;

// indexed by global "item_id"
typedef struct {
    u8 item_type; //00 = normal item, 01 = weapon, 02=armor, 03=accessory
    u16 type_relative_id;
} ItemTypeData;

class SrItemTypeRegistry : SrResourceRegistry<ItemTypeData> {
public:
    SrItemTypeRegistry() : SrResourceRegistry<ItemTypeData>::SrResourceRegistry() {};
};

SISTERRAY_API void InitInventory();
SISTERRAY_API void InitBattleInventory();
SISTERRAY_API void init_item_type_data();

/*utility for decrementing the quantity of an item at a particular inventory index*/
void handle_decrement_inventory(u16 inventory_index, u8 decrement_quantity);
SISTERRAY_API i16 sort_inventory(i32 sort_type);
static void initialize_augmented_data(u8 item_type);

#endif
