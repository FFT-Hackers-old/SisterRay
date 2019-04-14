#ifndef EQUIP_DRAW_CALLBACKS
#define EQUIP_DRAW_CALLBACKS

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"
#include "equip_widget_names.h"

void handleChangeCharacter(const EquipDrawEvent* event);
void handleUpdateDescription(const EquipDrawEvent* event);
void handleUpdateGearSlotsWidget(const EquipDrawEvent* event);
void handleUpdateStatMenuWidget(const EquipDrawEvent* event);
void enableListWidget(const EquipDrawEvent* event);
color getStatDisplayColor(u8 equippedStat, u8 toEquipStat);
#endif
