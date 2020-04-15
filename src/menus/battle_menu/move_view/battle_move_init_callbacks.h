#ifndef BATTLE_MOVE_INIT_CALLBACKS_H
#define BATTLE_MOVE_INIT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../menu_utils.h"
#include "../../../widgets/widget.h"
#include "../../menu.h"
#include "../battle_menu_widget_names.h"


void initBattleMoveViewWidget(const MenuInitEvent* event);
Widget* allocateMoveRow(const char* name, i32 xCoordinate, i32 yCoordinate);
void battleMoveNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);


#endif
