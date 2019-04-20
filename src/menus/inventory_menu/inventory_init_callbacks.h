#ifndef EQUIP_INIT_CALLBACKS_H
#define EQUIP_INIT_CALLBACKS_H

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

#endif
