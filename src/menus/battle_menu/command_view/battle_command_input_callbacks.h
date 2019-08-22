#ifndef BATTLE_COMMAND_INPUT_CALLBACKS_H
#define BATTLE_COMMAND_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../battle_menu_widget_names.h"
#include "battle_command_init_callbacks.h"
#include "battle_command_input_callbacks.h"

void handleSelectCommand(const BattleSpellInputEvent* event);
void handleExitSpell(const BattleSpellInputEvent* event);

#endif
