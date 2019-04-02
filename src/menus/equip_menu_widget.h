#ifndef EQUIP_MENU_WIDGET
#define EQUIP_MENU_WIDGET

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "menu_utils.h"
#include "../widgets/widget.h"
#include "../widgets/primitives.h"
#include "../widgets/assets.h"

Widget* initEquipMenuWidget();
void initCurrentEqupWidget(Widget* mainWidget);
void initGearMateriaSlotWidget(Widget* mainWidget);
void initStatDiffWidget(Widget* mainWidget);

#endif
