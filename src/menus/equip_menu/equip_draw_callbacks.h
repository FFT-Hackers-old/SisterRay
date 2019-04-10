#ifndef EQUIP_DRAW_CALLBACKS
#define EQUIP_DRAW_CALLBACKS

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"
#include "equip_widget_names.h"

void handleChangeCharacter(const EquipDrawEvent* params);
void handleUpdateDescription(const EquipDrawEvent* params);
void handleUpdateGearSlotsWidget(const EquipDrawEvent* params);
void handleUpdateStatMenuWidget(const EquipDrawEvent* params);
#endif
