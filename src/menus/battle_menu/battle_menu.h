#ifndef BATTLE_MENU_H
#define BATTLE_MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "base_view/battle_base_callbacks.h"
#include "command_view/battle_command_callbacks.h"
#include "targeting_view/battle_targeting_callbacks.h"
#include "spell_view/battle_spell_callbacks.h"
#include "summon_view/battle_summon_callbacks.h"
#include "eskill_view/battle_eskill_callbacks.h"
#include "item_view/battle_item_callbacks.h"


void battleMenuUpdateHandler(i32 updateStateMask);
void dispatchBattleUpdates();
void initializeBattleMenu();

#endif
