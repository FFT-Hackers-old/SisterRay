#ifndef INVENTORY_UTILS_H
#define INVENTORY_UTILS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>

/*utility for decrementing the quantity of an item at a particular inventory index*/
i16 sortInventory(i32 sort_type);
void testFillInventory();
void addInventoryEntry(u16 item_id, u8 quantity);
bool usableInInventoryMenu(u16 item_id);
u16 getCharacterRestrictionMask(u16 item_id);
u16 getRestrictionMask(u16 item_id);
u8 getTargetFlags(u16 item_id);
void updateInventoryEntry(u16 item_id, u8 quantity);

#endif
