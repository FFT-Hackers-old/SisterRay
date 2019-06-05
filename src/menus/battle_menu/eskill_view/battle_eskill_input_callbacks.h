#ifndef BATTLE_ESKILL_INPUT_CALLBACKS_H
#define BATTLE_ESKILL_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "battle_eskill_init_callbacks.h"
#include "battle_eskill_input_callbacks.h"

void handleSelectESkill(const BattleSpellInputEvent* event);
void handleExitESkill(const BattleSpellInputEvent* event);

#endif
