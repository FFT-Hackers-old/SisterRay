#ifndef BATTLE_INVENTORY_H
#define BATTLE_INVENTORY_H

#include "sr_inventory_template.h"

#define BATTLE_INVENTORY_SIZE (i16)1024 //maximum allowable inventory size within the menu engine

/*This is the in-battle copy of the item inventory*/
typedef struct {
    u16 materiaID;
    u8 quantity;
    u8 targetFlags;
    u8 restrictionMask;
    u8 padding;
} BattleInventoryEntry;

class SrBattleInventory : public SrInventory<BattleInventoryEntry> {
public:
    u16 slotsInUse;
    SrBattleInventory(i16 allocation_size);
    SrBattleInventory() : SrInventory<BattleInventoryEntry>() {}
    void setSlotsInUse(u16 slotsInUse);
};

SISTERRAY_API void initBattleInventory();

#endif

