#ifndef BATTLE_SPELL_INPUT_CALLBACKS_H
#define BATTLE_SPELL_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "battle_spell_init_callbacks.h"
#include "battle_spell_input_callbacks.h"

void handleSelectSpell(const MenuInputEvent* event);
void handleExitSpell(const MenuInputEvent* event);

#endif
