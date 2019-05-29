#ifndef BATTLE_ITEM_INPUT_CALLBACKS_H
#define BATTLE_ITEM_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "battle_item_init_callbacks.h"
#include "battle_item_input_callbacks.h"

void handleSelectItem(const BattleSpellInputEvent* event);
void handleExitItem(const BattleSpellInputEvent* event);

#endif
