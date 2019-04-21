#ifndef INVENTORY_INPUT_CALLBACKS_H
#define INVENTORY_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"
#include "inventory_widget_names.h"

void chooseViewHandler(const InventoryInputEvent* event);
void selectItemHandler(const InventoryInputEvent* event);
void executeSwapHandler(const InventoryInputEvent* event);
void useTargetedItemHandler(const InventoryInputEvent* event);
void arrangeItemsHandler(const InventoryInputEvent* event);
void exitInventoryHandler(const InventoryInputEvent* event);
void exitItemView(const InventoryInputEvent* event);
void exitSwapHandler(const InventoryInputEvent* event);
void exitTargetingHandler(const InventoryInputEvent* event);
void exitArrangeHandler(const InventoryInputEvent* event);
void exitKeyItemsHandler(const InventoryInputEvent* event);
void handleUsableItemEffects(u16 item_ID, u16 inventory_index);

#endif
