#ifndef INVENTORY_MENU_H
#define INVENTORY_MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>

SISTERRAY_API i32 onEnterInventory();
SISTERRAY_API void inventoryMenuUpdateHandler(i32 updateStateMask);
void displayActiveCursorStates(i32 updateStateMask);
void displayInventoryViews(i32 updateStateMask);
void renderMainInventoryView(i32 mainViewContextIndex);
void renderCharacterPortraits();
void renderKeyItemsView();
void handleInventoryInput(i32 updateStateMask);
void handleUsableItemEffects(u16 item_ID, u16 inventory_index);

#endif
