#ifndef STATUS_DRAW_CALLBACKS_H
#define STATUS_DRAW_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "status_widget_names.h"

void handleStatusMenuChangeCharacter(const MenuDrawEvent* event);
void handleActivateCombatStats(const MenuDrawEvent* event);
#endif
