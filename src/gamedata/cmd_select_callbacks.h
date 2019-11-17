#ifndef CMD_SELECT_CALLBACKS_H
#define CMD_SELECT_CALLBACKS_H

#include <SisterRay/SisterRay.h>
#include "../events/battle_events.h"
#include "../menus/menu_interface.h"
#include "../menus/battle_menu/battle_menu_widget_names.h"

typedef void(*SRPFNCMDSELECTCALLBACK)(SelectCommandEvent);

void handleSelectMagic(SelectCommandEvent* event);
void handleSelectSummon(SelectCommandEvent* event);
void handleSelectItem(SelectCommandEvent* event);

#endif // !COMMAND_CALLBACKS_H
