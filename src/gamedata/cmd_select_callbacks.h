#ifndef CMD_SELECT_CALLBACKS_H
#define CMD_SELECT_CALLBACKS_H

#include <SisterRay/SisterRay.h>
#include "../events/battle_events.h"
#include "../menus/menu_interface.h"
#include "../menus/battle_menu/battle_menu_widget_names.h"

typedef void(*SRPFNCMDSELECTCALLBACK)(SelectCommandEvent*);

void handleCommandTarget(SelectCommandEvent* event);
void handleWeaponTarget(SelectCommandEvent* event);
void cmdMagicSelectHandler(SelectCommandEvent* event);
void cmdSummonSelectHandler(SelectCommandEvent* event);
void cmdItemSelectHandler(SelectCommandEvent* event);
void cmdWMagicSelectHandler(SelectCommandEvent* event);
void cmdWSummonSelectHandler(SelectCommandEvent* event);
void cmdWItemSelectHandler(SelectCommandEvent* event);

#endif // !COMMAND_CALLBACKS_H
