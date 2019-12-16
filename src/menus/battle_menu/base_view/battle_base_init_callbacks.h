#ifndef BATTLE_SUMMON_INIT_CALLBACKS_H
#define BATTLE_SUMMON_INIT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../menu_utils.h"
#include "../../../widgets/widget.h"
#include "../../menu.h"
#include "../battle_menu_widget_names.h"

void initBaseViewWidget(const MenuInitEvent* event);

Widget* allocateBaseRow(const char* name, i32 xCoordinate, i32 yCoordinate);
Widget* allocateBarWidget(const char* name, i32 xCoordinate, i32 yCoordinate);
Widget* allocateHPResourceWidget(const char* name, i32 xCoordinate, i32 yCoordinate);
Widget* allocateMPResourceWidget(const char* name, i32 xCoordinate, i32 yCoordinate);
Widget* allocateBarriersWidget(const char* name, i32 xCoordinate, i32 yCoordinate);
Widget* allocateDisplayWidget(const char* name, i32 xCoordinate, i32 yCoordinate);


#endif
