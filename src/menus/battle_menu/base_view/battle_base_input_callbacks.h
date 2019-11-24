#ifndef BATTLE_SPELL_INPUT_CALLBACKS_H
#define BATTLE_SPELL_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../menu_utils.h"
#include "../../../widgets/widget.h"
#include "../../../events/menu_events.h"
#include "../battle_menu_widget_names.h"

void handlePauseBattle(const MenuInputEvent* event);
void swapCharacterBattle(const MenuInputEvent* event);

#endif
