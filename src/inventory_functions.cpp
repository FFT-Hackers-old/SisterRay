#include "inventory_functions.h"
#include "impl.h"

SISTERRAY_API u8 setupBattleInventory() {
    i32 inventorySize = gContext.inventory->current_capacity();
    u16 itemID;
    u8 quantity;
    u8 targetFlags;
    u16 restrictionMask;

    u16 totalItemsCount = 0;
    for (i32 inventory_index = 0; inventory_index < inventorySize; inventory_index++) {
        itemID = gContext.inventory->get_resource(inventory_index).item_id;
        quantity = gContext.inventory->get_resource(inventory_index).quantity;
        targetFlags = get_target_flags(itemID);
        restrictionMask = get_restriction_mask(itemID);

        if (itemID != 0xFFFF) {
            BattleInventoryEntry entry = {
                itemID,
                quantity,
                targetFlags,
                (u8)restrictionMask,
                0
            };
            gContext.battle_inventory->update_resource(inventory_index, entry);
            totalItemsCount++;
        }
    }
    gContext.battle_inventory->setSlotsInUse(totalItemsCount);
    return (u8)totalItemsCount; //The code expects this to return the "current item count" as a byte, we are making sure this isn't actually read in the code
}
