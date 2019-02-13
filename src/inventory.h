#ifndef INVENTORY_H
#define INVENTORY_H

#include <SisterRay/types.h>
#include <SisterRay/data_addresses.h>

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

SISTERRAY_API void InitInventory();
SISTERRAY_API void InitBattleInventory();

/*utility for decrementing the quantity of an item at a particular inventory index*/
void handle_decrement_inventory(u16 inventory_index, u8 decrement_quantity);

#endif
