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


void gearViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex);

#endif
