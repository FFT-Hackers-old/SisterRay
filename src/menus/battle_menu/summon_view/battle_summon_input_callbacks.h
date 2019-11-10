#ifndef BATTLE_SPELL_INPUT_CALLBACKS_H
#define BATTLE_SPELL_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "battle_summon_init_callbacks.h"
#include "battle_summon_input_callbacks.h"

void handleSelectSummon(const MenuInputEvent* event);
void handleExitSummon(const MenuInputEvent* event);

#endif
