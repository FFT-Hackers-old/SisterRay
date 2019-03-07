#ifndef BATTLE_INVENTORY_H
#define BATTLE_INVENTORY_H

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
    SrBattleInventory() : SrResourceRegistry<BattleInventoryEntry>() {}
    void decrementInventoryEntry(u16 inventory_index, u8 stepSize);
    void incrementInventoryEntry(u16 inventory_index, u8 stepSize);
    u16 findItemInventoryIndex(u16 itemID);
    bool incrementInventoryByItemID(u16 item_id, u8 stepSize);
    bool decrementInventoryByItemID(u16 item_id, u8 stepSize);
    void setSlotsInUse(u16 slotsInUse);
};

SISTERRAY_API void InitBattleInventory();

/*utility for decrementing the quantity of an item at a particular inventory index*/

#endif

