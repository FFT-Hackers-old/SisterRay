#ifndef EQUIP_DRAW_CALLBACKS
#define EQUIP_DRAW_CALLBACKS

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"
#include "equip_widget_names.h"

void handleChangeCharacter(const MenuDrawEvent* event);
void handleUpdateDescription(const MenuDrawEvent* event);
void handleUpdateGearSlotsWidget(const MenuDrawEvent* event);
void handleUpdateStatMenuWidget(const MenuDrawEvent* event);
void enableListWidget(const MenuDrawEvent* event);
color getStatDisplayColor(u8 equippedStat, u8 toEquipStat);
#endif
