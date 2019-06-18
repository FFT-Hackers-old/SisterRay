#ifndef BATTLE_CHANGE_INPUT_CALLBACKS_H
#define BATTLE_CHANGE_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "battle_change_init_callbacks.h"
#include "battle_change_input_callbacks.h"

void handleSelectChange(const BattleSpellInputEvent* event);
void handleExitChange(const BattleSpellInputEvent* event);

#endif
