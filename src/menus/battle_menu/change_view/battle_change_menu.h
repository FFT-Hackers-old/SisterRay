#ifndef BATTLE_CHANGE_MENU_H
#define BATTLE_CHANGE_MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../battle_menu_utils.h"
#include "battle_change_widget_names.h"
#include "battle_change_callbacks.h"
#include "../../..//widgets/widget.h"
#include "../../../events/menu_events.h"

void battleChangeUpdateHandler(i32 updateStateMask);
void battleChangeInputHandler();

#endif
