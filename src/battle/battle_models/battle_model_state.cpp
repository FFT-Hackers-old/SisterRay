#include "battle_models_api.h"
#include "../../impl.h"


SISTERRAY_API BattleModelState* getBattleModelState(u8 actorID) {
    return &(BATTLE_MODEL_STATE_BIG_ARRAY[actorID]);
}


SISTERRAY_API BattleModelStateSmall* getBattleModelState74(u8 actorID) {
    return &(BATTLE_MODEL_STATE74_ARRAY[actorID]);
}


SISTERRAY_API ModelRotationData* getBattleModelRotationData(u8 actorID) {
    return &(BATTLE_MODEL_ROTATION_DATA[actorID]);
}

SISTERRAY_API void setModelVanish(u8 actorID) {
    getBattleModelState(actorID)->modelEffectFlags |= 2;
}

SISTERRAY_API void setModelAppaer(u8 actorID) {
    getBattleModelState(actorID)->modelEffectFlags |= 4;
}

SISTERRAY_API void setActorIdleAnim(u8 actorID, u16 animScriptIdx) {
    u8* actorIdleAnimScripts = (u8*)0xBF2DF8;
    actorIdleAnimScripts[actorID] = animScriptIdx;
    gContext.battleActors.getActiveBattleActor(actorID).actorBattleVars->idleAnimScript = animScriptIdx;
    gContext.battleActors.getActiveBattleActor(actorID).actorBattleVars->idleAnimHolder = animScriptIdx;
    gContext.battleActors.getActiveBattleActor(actorID).party10->idleAnimScript = animScriptIdx;
}

SISTERRAY_API void setActorHurtAnim(u8 actorID, u16 animScriptIdx) {
    gContext.battleActors.getActiveBattleActor(actorID).actorBattleVars->damageAnimID = animScriptIdx;
}

#define G_ACTOR_SCREEN_CTX_ARRAY      ((ActorScreenSpaceCtx*)0xBF2E20)
SISTERRAY_API ActorScreenSpaceCtx getActorScreenSpacePosition(u8 actorIdx) {
    auto retCopy = G_ACTOR_SCREEN_CTX_ARRAY[actorIdx];
    return retCopy;
}
