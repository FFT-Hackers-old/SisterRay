#include "inventory_functions.h"
#include "inventory_utils.h"
#include "../impl.h"

SISTERRAY_API u8 setupBattleInventory() {
    i32 inventorySize = gContext.inventory->currentCapacity();
    u16 itemID;
    u8 quantity;
    u8 targetFlags;
    u16 restrictionMask;

    u16 totalItemsCount = 0;
    for (i32 inventory_index = 0; inventory_index < inventorySize; inventory_index++) {
        itemID = gContext.inventory->getResource(inventory_index).materiaID;
        quantity = gContext.inventory->getResource(inventory_index).quantity;
        targetFlags = getTargetFlags(itemID);
        restrictionMask = getRestrictionMask(itemID);

        if (itemID != 0xFFFF) {
            BattleInventoryEntry entry = {
                itemID,
                quantity,
                targetFlags,
                (u8)restrictionMask,
                0
            };
            gContext.battleInventory->updateResource(inventory_index, entry);
            totalItemsCount++;
        }
    }
    gContext.battleInventory->setSlotsInUse(totalItemsCount);
    return (u8)totalItemsCount; //The code expects this to return the "current item count" as a byte, we are making sure this isn't actually read in the code
}
