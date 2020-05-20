#ifndef EQUIP_INIT_CALLBACKS_H
#define EQUIP_INIT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "equip_widget_names.h"
#include "../menu.h"

void initCharDataWidget(const MenuInitEvent* event);
void initGearMateriaSlotWidget(const MenuInitEvent* event);
void initGearDescWidget(const MenuInitEvent* event);
void initStatDiffWidget(const MenuInitEvent* event);
void initGearListWidget(const MenuInitEvent* event);

Widget* allocateEquipRow(const char* name, i32 xCoordinate, i32 yCoordinate);
void equipRowUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);
Widget* allocateStatDiffRow(const char* name, i32 xCoordinate, i32 yCoordinate);
void gearViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);

#endif
