#ifndef INVENTORY_MENU_H
#define INVENTORY_MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>

SISTERRAY_API i32 onEnterInventory();
SISTERRAY_API void inventoryMenuUpdateHandler(int a1);
void displayActiveCursorStates(int a1);
void displayInventoryViews(int a1);
void renderMainInventoryView(int custom_arrange_active);
void renderCharacterPortraits();
void renderKeyItemsView();
void handleInventoryInput(int a1);
void handleUsableItemEffects(u16 item_ID, u16 inventory_index);

#endif
