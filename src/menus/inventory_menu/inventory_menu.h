#ifndef INVENTORY_MENU_H
#define INVENTORY_MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "inventory_widget_names.h"

SISTERRAY_API void inventoryMenuUpdateHandler(i32 updateStateMask);
void displayInventoryCursorStates(Menu* menu, u16 menuState, u32 stateControlMask);

#endif
