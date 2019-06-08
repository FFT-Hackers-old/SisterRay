#ifndef BATTLE_CHANGE_INIT_CALLBACKS_H
#define BATTLE_CHANGE_INIT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../menu_utils.h"
#include "../../../widgets/widget.h"
#include "../../../events/menu_events.h"
#include "../../menu.h"
#include "battle_change_widget_names.h"


void initBattleChangeViewWidget(const BattleSpellDrawEvent* event);
Widget* allocateChangeRow(const char* name, i32 xCoordinate, i32 yCoordinate);


#endif
