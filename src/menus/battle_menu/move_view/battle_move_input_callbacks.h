#ifndef BATTLE_MOVE_INPUT_CALLBACKS_H
#define BATTLE_MOVE_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../battle_menu_widget_names.h"
#include "../../../widgets/widget.h"
#include "../../menu.h"

void handleSelectMove(const MenuInputEvent* event);
void goToCommand(const MenuInputEvent* srMenuEvent);

#endif
