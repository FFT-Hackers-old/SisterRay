#ifndef CMD_SELECT_CALLBACKS_H
#define CMD_SELECT_CALLBACKS_H

#include <SisterRay/SisterRay.h>
#include "../events/battle_events.h"
#include "../menus/menu_interface.h"
#include "../menus/battle_menu/battle_menu_widget_names.h"

typedef void(*SRPFNSCMDSELECTCALLBACK)(SelectCommandEvent);

handleSelectMagic(SelectCommandEvent* event);
handleSelectSummon(SelectCommandEvent* event);
handleSelectItem(SelectCommandEvent* event);

#endif // !COMMAND_CALLBACKS_H
