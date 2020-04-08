#include "sr_battle_engine.h"
#include "../impl.h"

#define FF7_SET_PLAYER_MODEL      ((void*)0x42A330)
#define FF7_ANIMATE_WEAPON        ((void*)0x4303CC)
#define FF7_DISPATCH_AUTO_ACTIONS ((void*)0x5C8CFA)
#define FF7_ACTION_QUEUE_POP      ((void*)0x435D81)
#define FF7_RUN_AI_SCRIPT         ((void*)0x5C86E0)
#define FF7_DISPLAY_BATTLE_STRING ((void*)0x6D7245)
#define FF7_INIT_PLAYER_ACTORS    ((void*)0x5CF650)
#define FF7_INIT_ENEMY_ACTORS     ((void*)0x5D0690)


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
    mogReplaceFunction(FF7_INIT_PLAYER_ACTORS, &initializePlayerActors);
    mogReplaceFunction(FF7_INIT_ENEMY_ACTORS, &initializeEnemyActors);
    registerActionPopCallbacks();
}
