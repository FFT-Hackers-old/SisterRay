#ifndef INVENTORY_DRAW_CALLBACKS
#define INVENTORY_DRAW_CALLBACKS

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"
#include "inventory_widget_names.h"

void handleCustomSortActive(const MenuDrawEvent* event);
void handleKeyItemsViewActive(const MenuDrawEvent* event);
void handlePartyViewActive(const MenuDrawEvent* event);
void handleUpdateDescription(const MenuDrawEvent* event);
void handleArrangeActive(const MenuDrawEvent* event);
void handleMenuViewActive(const MenuDrawEvent* event);
#endif
