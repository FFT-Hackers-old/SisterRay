#ifndef EQUIP_MENU_H
#define EQUIP_MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "equip_widget_names.h"
#include "equip_draw_callbacks.h"
#include "../../widgets/widget.h"

SISTERRAY_API void equipMenuUpdateHandler(i32 updateStateMask);
color getStatDisplayColor(u8 equippedStat, u8 toEquipStat);
void displayMenuCursors(Menu* menuObject, u16 menuState, u32 stateControlMask);

#endif
