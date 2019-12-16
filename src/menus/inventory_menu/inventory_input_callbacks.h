#ifndef INVENTORY_INPUT_CALLBACKS_H
#define INVENTORY_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "inventory_widget_names.h"

void chooseViewHandler(const MenuInputEvent* event);
void selectItemHandler(const MenuInputEvent* event);
void executeSwapHandler(const MenuInputEvent* event);
void useTargetedItemHandler(const MenuInputEvent* event);
void arrangeItemsHandler(const MenuInputEvent* event);
void exitInventoryHandler(const MenuInputEvent* event);
void exitItemView(const MenuInputEvent* event);
void exitSwapHandler(const MenuInputEvent* event);
void exitTargetingHandler(const MenuInputEvent* event);
void exitArrangeHandler(const MenuInputEvent* event);
void exitKeyItemsHandler(const MenuInputEvent* event);
void handleUsableItemEffects(u16 item_ID, u16 inventory_index, u32 partyMemberIndex, Menu* menu);

#endif
