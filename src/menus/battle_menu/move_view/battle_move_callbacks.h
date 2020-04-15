#ifndef BATTLE_MOVE_CALLBACKS_H
#define BATTLE_MOVE_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "battle_move_init_callbacks.h"
#include "battle_move_input_callbacks.h"
#include "battle_move_draw_callbacks.h"
#include "../battle_menu_widget_names.h"


void initializeBattleMoveMenu();
void registerMoveMenuListeners();

#endif
