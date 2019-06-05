#ifndef BATTLE_SPELL_INIT_CALLBACKS_H
#define BATTLE_SPELL_INIT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../menu_utils.h"
#include "../../../widgets/widget.h"
#include "../../../events/menu_events.h"
#include "../../menu.h"
#include "battle_spell_widget_names.h"


void initBattleMagicViewWidget(const BattleSpellDrawEvent* event);
Widget* allocateSpellRow(const char* name, i32 xCoordinate, i32 yCoordinate);


#endif
