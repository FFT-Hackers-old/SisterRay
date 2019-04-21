#ifndef EQUIP_DRAW_CALLBACKS
#define EQUIP_DRAW_CALLBACKS

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"
#include "inventory_widget_names.h"

void handleCustomSortActive(const InventoryDrawEvent* event);
void handleKeyItemsViewActive(const InventoryDrawEvent* event);
void handlePartyViewActive(const InventoryDrawEvent* event);
void handleUpdateDescription(const InventoryDrawEvent* event);
void handleArrangeActive(const InventoryDrawEvent* event);
void handleMenuViewActive(const InventoryDrawEvent* event);
#endif
