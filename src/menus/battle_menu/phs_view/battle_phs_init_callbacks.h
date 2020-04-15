#ifndef BATTLE_PHS_INIT_CALLBACKS_H
#define BATTLE_PHS_INIT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../menu_utils.h"
#include "../../../widgets/widget.h"
#include "../../menu.h"
#include "../battle_menu_widget_names.h"


void initBattlePHSViewWidget(const MenuInitEvent* event);
Widget* allocatePHSRow(const char* name, i32 xCoordinate, i32 yCoordinate);
void battlePHSUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);


#endif
