#ifndef BATTLE_INVENTORY_H
#define BATTLE_INVENTORY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>

#define BATTLE_INVENTORY_SIZE (i16)1024 //maximum allowable inventory size within the menu engine

/*This is the in-battle copy of the item inventory*/
typedef struct {
    u16 item_id;
    u8 quantity;
    u8 targetFlags;
    u8 restrictionMask;
    u8 padding;
} BattleInventoryEntry;

class SrBattleInventory : public SrInventory<BattleInventoryEntry> {
public:
    u16 slots_in_use;
    SrBattleInventory(i16 allocation_size);
    SrBattleInventory() : SrInventory<BattleInventoryEntry>() {}
    void setSlotsInUse(u16 slotsInUse);
};

SISTERRAY_API void InitBattleInventory();

/*utility for decrementing the quantity of an item at a particular inventory index*/

#endif

