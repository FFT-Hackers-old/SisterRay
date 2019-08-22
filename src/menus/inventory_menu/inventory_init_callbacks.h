#ifndef INVENTORY_INIT_CALLBACKS_H
#define INVENTORY_INIT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"
#include "inventory_widget_names.h"
#include "../menu.h"


void initViewChoiceWidget(const InventoryInitEvent* event);
void initCharViewWidget(const InventoryInitEvent* event);
void initItemViewWidget(const InventoryInitEvent* event);
void keyItemsViewWidget(const InventoryInitEvent* event);
void itemDescriptionWidget(const InventoryInitEvent* event);
void arrangeTypeWidget(const InventoryInitEvent* event);

Widget* allocateInventoryRow(const char* name, i32 xCoordinate, i32 yCoordinate);
void inventoryRowUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void keyItemViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);

#endif
