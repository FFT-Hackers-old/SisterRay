#ifndef BATTLE_COMMAND_INPUT_CALLBACKS_H
#define BATTLE_COMMAND_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../battle_menu_widget_names.h"
#include "../../../widgets/widget.h"
#include "../../menu.h"

void handleSelectCommand(const MenuInputEvent* event);
void openMove(const MenuInputEvent* event);

#endif
