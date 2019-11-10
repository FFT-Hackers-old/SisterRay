#ifndef BATTLE_SUMMON_MENU_H
#define BATTLE_SUMMON_MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../menu_utils.h"
#include "../battle_menu_widget_names.h"
#include "battle_summon_callbacks.h"
#include "../../../widgets/widget.h"
#include "../../../events/menu_events.h"

void battleSummonUpdateHandler(i32 updateStateMask);
void battleSummonInputHandler();

#endif
