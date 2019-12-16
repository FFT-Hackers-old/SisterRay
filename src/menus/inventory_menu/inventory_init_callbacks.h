#ifndef INVENTORY_INIT_CALLBACKS_H
#define INVENTORY_INIT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "inventory_widget_names.h"
#include "../menu.h"
#include "../../inventories/inventory_utils.h"


void initViewChoiceWidget(const MenuInitEvent* event);
void initCharViewWidget(const MenuInitEvent* event);
void initItemViewWidget(const MenuInitEvent* event);
void keyItemsViewWidget(const MenuInitEvent* event);
void itemDescriptionWidget(const MenuInitEvent* event);
void arrangeTypeWidget(const MenuInitEvent* event);

Widget* allocateInventoryRow(const char* name, i32 xCoordinate, i32 yCoordinate);
void inventoryRowUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
void keyItemViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);

#endif
