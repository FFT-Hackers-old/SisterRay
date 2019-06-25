#include "sr_battle_engine.h"
#include "../impl.h"

#define FF7_SET_PLAYER_MODEL  ((void*)0x42A330)
#define FF7_ANIMATE_WEAPON   ((void*)0x4303CC)


PFNSETPLAYERDATA* oldSetPlayerModel;

void initializeSrBattleEngine() {
    mogReplaceFunction(FF7_TRANSLATE_ENEMY_BDATA, &translateEnemyABData);
    mogReplaceFunction(FF7_TRANSLATE_PLAYER_BDATA, &translatePlayerABData);
    mogReplaceFunction(FF7_CREATE_MODEL, &srCreateModel);
    mogReplaceFunction(FF7_SET_PLAYER_MODEL, &srSetPlayerModel);
    mogReplaceFunction(FF7_ANIMATE_WEAPON, &playCorrectWeaponAnimation);
    //oldRunAnimationScript = (PFNRUNANIMSCRIPT*)mogRedirectFunction(RUN_ANIMATION_SCRIPT, &animationScriptTrampoline);
}
