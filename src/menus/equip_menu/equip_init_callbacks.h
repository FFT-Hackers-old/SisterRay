#ifndef EQUIP_INIT_CALLBACKS_H
#define EQUIP_INIT_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"
#include "equip_widget_names.h"
#include "../menu.h"

void initCharDataWidget(const EquipInitEvent* event);
void initGearMateriaSlotWidget(const EquipInitEvent* event);
void initGearDescWidget(const EquipInitEvent* event);
void initStatDiffWidget(const EquipInitEvent* event);

#endif
