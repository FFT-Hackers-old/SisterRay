#ifndef BATTLE_SPELL_INPUT_CALLBACKS_H
#define BATTLE_SPELL_INPUT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../../events/menu_events.h"
#include "../battle_menu_widget_names.h"

void handleSelectSpell(const MenuInputEvent* event);
void handleExitSpell(const MenuInputEvent* event);

#endif
