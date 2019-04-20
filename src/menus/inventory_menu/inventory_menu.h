#ifndef INVENTORY_MENU_H
#define INVENTORY_MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../events/menu_events.h"

SISTERRAY_API void inventoryMenuUpdateHandler(i32 updateStateMask);
void displayActiveCursorStates(i32 updateStateMask, Menu* menu);
void displayInventoryViews(i32 updateStateMask);
void handleInventoryInput(i32 updateStateMask);

#endif
