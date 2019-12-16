#ifndef BATTLE_TARGETING_INPUT_CALLBACKS_H
#define BATTLE_TARGETING_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../menu_utils.h"
#include "../../../widgets/widget.h"
#include "../battle_menu_widget_names.h"

void handleSelectTargets(const MenuInputEvent* event);
void handleExitSelectTargets(const MenuInputEvent* event);

#endif
