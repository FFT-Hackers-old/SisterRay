#include "sr_battle_engine.h"
#include "../impl.h"

#define FF7_SET_PLAYER_MODEL  ((void*)0x42A330)
#define FF7_ANIMATE_WEAPON   ((void*)0x4303CC)
#define FF7_DISPATCH_AUTO_ACTIONS ((void*)0x5C8CFA)


PFNSETPLAYERDATA* oldSetPlayerModel;

void initializeSrBattleEngine() {
    mogReplaceFunction(FF7_TRANSLATE_ENEMY_BDATA, &translateEnemyABData);
    mogReplaceFunction(FF7_TRANSLATE_PLAYER_BDATA, &translatePlayerABData);
    mogReplaceFunction(FF7_CREATE_MODEL, &srCreateModel);
    mogReplaceFunction(FF7_SET_PLAYER_MODEL, &srSetPlayerModel);
    mogReplaceFunction(FF7_ANIMATE_WEAPON, &playCorrectWeaponAnimation);
    mogReplaceFunction(FF7_DISPATCH_AUTO_ACTIONS, &dispatchAutoActions)
    //oldRunAnimationScript = (PFNRUNANIMSCRIPT*)mogRedirectFunction(RUN_ANIMATION_SCRIPT, &animationScriptTrampoline);
}
