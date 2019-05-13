#include <stdlib.h>
#include <string.h>
#include "../impl.h"
#include "battle_inventory.h"
#include <memory>

/*We want the Battle Inventory to construct itself with default (empty) values*/
SrBattleInventory::SrBattleInventory(i16 allocation_size) : SrInventory<BattleInventoryEntry>(allocation_size) {
    for (i32 i = 0; i < INVENTORY_SIZE; i++) {
        BattleInventoryEntry entry = {
            0xFFFF,
            0,
            0,
            0,
            0
        };
        add_resource(entry);
    }
}

void SrBattleInventory::setSlotsInUse(u16 slotsInUse) {
    slots_in_use = slotsInUse;
}

SISTERRAY_API void initBattleInventory()
{
    gContext.battleInventory = std::make_unique<SrBattleInventory>(BATTLE_INVENTORY_SIZE);
    srLogWrite("sister ray: in_battle inventory initialized with capacity: %lu", (unsigned long)gContext.battleInventory->current_capacity());
}
