#include <stdlib.h>
#include <string.h>
#include "impl.h"
#include "kernel_utils.h"
#include "inventory.h"


/*Method to initialize the new inventories with an appropriate size*/
SISTERRAY_API void InitInventory()
{
    SrItemInventory* inventory = &gContext.inventory;
    initStaticObjectRegistry<InventoryEntry, SrItemInventory>(inventory, INVENTORY_SIZE);
    srLogWrite("sister ray: inventory initialized with size: ", (unsigned long)gContext.inventory.capacity);
}

SISTERRAY_API void InitBattleInventory()
{
    SrBattleInventory* inventory = &gContext.battle_inventory;
    initStaticObjectRegistry<BattleInventoryEntry, SrBattleInventory>(inventory, INVENTORY_SIZE);
    srLogWrite("sister ray: in_battle inventory initialized with size: ", (unsigned long)gContext.inventory.capacity);
}

/*utility for decrementing the quantity of an item at a particular inventory index*/
void handle_decrement_inventory(u16 inventory_index, u8 decrement_quantity) {
    if (gContext.inventory.data[inventory_index].quantity + decrement_quantity = > 1) {
        gContext.inventory.data[inventory_index].item_id = gContext.inventory.data[inventory_index].quantity - decrement_quantity;
    }
    else {
        gContext.inventory.data[inventory_index].item_id = 0xFFFF;
        gContext.inventory.data[inventory_index].quantity = 0;
    }
};
