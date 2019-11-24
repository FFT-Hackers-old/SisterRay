#ifndef BATTLE_SUMMON_INPUT_CALLBACKS_H
#define BATTLE_SUMMON_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../../events/menu_events.h"
#include "../battle_menu_widget_names.h"

void handleSelectSummon(const MenuInputEvent* event);
void handleExitSummon(const MenuInputEvent* event);

#endif
