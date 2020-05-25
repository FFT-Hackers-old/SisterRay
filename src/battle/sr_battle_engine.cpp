#include "sr_battle_engine.h"
#include "effects/effects_api.h"
#include "battle_models/battle_models.h"
#include "battle_models/model_setup_routines.h"
#include "actions/actions_api.h"
#include "effects/effects_api.h"
#include "animation_script/animation_script_parser.h"
#include "ai_script/ai_script_engine.h"
#include "time/battle_time.h"
#include "battle.h"
#include "string_display_event.h"
#include "battle_actors.h"
#include "../impl.h"

#define FF7_SET_PLAYER_MODEL      ((void*)0x42A330)
#define FF7_ANIMATE_WEAPON        ((void*)0x4303CC)
#define FF7_DISPATCH_AUTO_ACTIONS ((void*)0x5C8CFA)
#define FF7_ACTION_QUEUE_POP      ((void*)0x435D81)
#define FF7_RUN_AI_SCRIPT         ((void*)0x5D7FE4)
#define FF7_DISPLAY_BATTLE_STRING ((void*)0x6D7245)
#define FF7_INIT_PLAYER_ACTORS    ((void*)0x5CF650)
#define FF7_INIT_ENEMY_ACTORS     ((void*)0x5D0690)
#define FF7_SETUP_BATTLE          ((void*)0x437DB0)
#define FF7_END_SUMMON            ((void*)0x5C18BC)
#define FF7_RUN_SUMMON_ANIM_SCRIPT ((void*)0x5C1D9A)
#define FF7_TARGET_REACTION_MAIN   ((void*)0x425FC4)
#define FF7_UPDATE_ACTOR_TIMERS   ((void*)0x4339C2)
#define FF7_UPDATE_RESOURCES      ((void*)0x4320AC)
#define FF7_DISPATCH_TIMERS       ((void*)0x43526A)


PFNSETPLAYERDATA* oldSetPlayerModel;

void initializeSrBattleEngine() {
    mogReplaceFunction(FF7_TRANSLATE_ENEMY_BDATA, &translateEnemyABData);
    mogReplaceFunction(FF7_TRANSLATE_PLAYER_BDATA, &translatePlayerABData);
    mogReplaceFunction(FF7_CREATE_MODEL, &srCreateModel);
    mogReplaceFunction(FF7_SET_PLAYER_MODEL, &srSetPlayerModel);
    mogReplaceFunction(FF7_ANIMATE_WEAPON, &playCorrectWeaponAnimation);
    mogReplaceFunction(FF7_DISPATCH_AUTO_ACTIONS, &dispatchAutoActions);
    mogReplaceFunction(FF7_ACTION_QUEUE_POP, &srHandlePoppedAction);
    mogReplaceFunction(LOAD_FORMATION_HANDLER, &srLoadBattleFormation);
    mogReplaceFunction(FF7_DISPLAY_BATTLE_STRING, &nopBattleString);
    mogReplaceFunction(RUN_ANIMATION_SCRIPT, &runAnimationScript);
    //mogReplaceFunction(FF7_RUN_AI_SCRIPT, &srRunAIScript);
    mogReplaceFunction(EXECUTE_AI_SCRIPT_HANDLER, &srExecuteAIScript);
    mogReplaceFunction(FF7_INIT_PLAYER_ACTORS, &initializePlayerActors);
    mogReplaceFunction(FF7_INIT_ENEMY_ACTORS, &initializeEnemyActors);
    mogReplaceFunction(FF7_SETUP_BATTLE, &setupBattle);
    mogReplaceFunction(FF7_END_SUMMON, &endSummonHandler);
    mogReplaceFunction(FF7_RUN_SUMMON_ANIM_SCRIPT, &playSummonAnimationScript);
    mogReplaceFunction(FF7_TARGET_REACTION_MAIN, &targetReactionMain);
    mogReplaceFunction(FF7_UPDATE_ACTOR_TIMERS, &srUpdateActorTimers);
    mogReplaceFunction(FF7_UPDATE_RESOURCES, &srHandleFillATB);
    mogReplaceFunction(FF7_DISPATCH_TIMERS, &srDispathTimeEvents);
    registerActionPopCallbacks();
}
