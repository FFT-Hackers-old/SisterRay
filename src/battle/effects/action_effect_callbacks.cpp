#include "action_effect_callbacks.h"
#include "../battle_utils.h"
#include "../../impl.h"
#include "action_effects.h"
#include "action_effect_types.h"
#include "../actions/actions_api.h"

#define displayActionStringEffect       ((PFNSREFFECTCALLBACK)0x42782A)
void srDisplayActionStringEffect() {
    displayActionStringEffect();
}

typedef void(*SRPFNSUB42519A)(u16, u8*, u16, PFNSREFFECTCALLBACK, u16, R3PointWord*);
#define gameDispatchActorMoveEffects    ((SRPFNSUB42519A)0x42519A)

void srDispatchMoveEffects(u16 actorID, u8* scriptPtr, u16 moveAmt, PFNSREFFECTCALLBACK moveCallback, u16 frameCount, R3PointWord* moveRelativeBase) {
    gameDispatchActorMoveEffects(actorID, scriptPtr, moveAmt, moveCallback, frameCount, moveRelativeBase);
}


typedef void(*SRPFNSUB5BFEC9)(u16, ModelPosition*);
#define gameComputeMultitargetMoveBase    ((SRPFNSUB5BFEC9)0x5BFEC9)

void srComputeMultitargetMoveBase(u16 targetMask, ModelPosition* ret) {
    gameComputeMultitargetMoveBase(targetMask, ret);
}


typedef GameRotationMatrix*(*SRPFN_SETDISPLAYDAMAGE)(u8);
#define sub_5C0158     ((SRPFN_SETDISPLAYDAMAGE)0x5C0158)
void srDisplayDamageEvents(u32 damagedMask) {
    srLogWrite("Dispatch ERUPTION Events State: %x", damagedMask);
    for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
        if ((1 << actorIdx) & damagedMask) {
            srLogWrite("DISPATCHING A DAMAGE EVENT FOR ACTOR actorIdx");
            sub_5C0158(actorIdx);
        }
    }
}

void srDispatchTargetReaction(u8 targetIdx) {
    sub_5C0158(targetIdx);
}

void srTriggerDamageEffect() {
    auto& triggerDamageMainCtx = *(TriggerDamageEffectCtx*)getEffect60QueueTop();

    if (triggerDamageMainCtx.elapsedFrames){
        if (triggerDamageMainCtx.elapsedFrames != 1)
            return;
    }
    else {
        triggerDamageMainCtx.displayDelay = 3;
        ++triggerDamageMainCtx.elapsedFrames;
    }

    if (!triggerDamageMainCtx.displayDelay) {
        triggerDamageMainCtx.handlerControl = 0xFFFF;
        if ((triggerDamageMainCtx.impactEventQueueIdx & 0x8000u) == 0) {
            auto& impactEvent = *getImpactEvent(triggerDamageMainCtx.impactEventQueueIdx);
            auto srExtendedImpactEvent = getExtendedImpactEvent(triggerDamageMainCtx.impactEventQueueIdx);
            auto damageEvent = getDamageEvent(triggerDamageMainCtx.damageEventQueueIdx);
            auto targetID = impactEvent.targetID;
            auto targetState = gContext.battleActors.getActiveBattleActor(targetID);
            if (targetID >= 0 && targetID < 3) {
                if (impactEvent.currentTargetHP <= targetState.actorBattleVars->maxHP)
                    targetState.actorTimers->currentHP = impactEvent.currentTargetHP;
                if (impactEvent.currentTargetMP <= targetState.actorBattleVars->maxMP)
                    targetState.actorTimers->currentMP = impactEvent.currentTargetMP;
            }
            TriggerDamageDisplayEvent triggerDamageEvent{ damageEvent, &impactEvent, srExtendedImpactEvent, &targetState };
            gContext.eventBus.dispatch(TRIGGER_DAMAGE_DISPLAY, &triggerDamageEvent);
        }
    }
    --triggerDamageMainCtx.displayDelay;
}


#define gameDisplayDamageNumbers       ((PFNSREFFECTCALLBACK)0x5BB410)
#define gameDisplayBarrierEffects      ((PFNSR_VOIDSUB)0x425F3F)
#define gameHandleTargetReactions      ((PFNSR_VOIDSUB)0x42613A)
typedef int(*PFNSR_SOMECRITSUB)(u32, u32, u32);
#define sub_5BD436                     ((PFNSR_SOMECRITSUB)0x5BD436)
void targetReactionMain() {
    auto& reactionMainCtx = *(TargetReactionEffectCtx*)getEffect100QueueTop();
    if (reactionMainCtx.reactionDelay) {
        --reactionMainCtx.reactionDelay;
        return;
    }

    if (reactionMainCtx.actionFlags & 2) //Checks to see if action crits
        triggerScreenFlash(250, 250, 250);


    if (reactionMainCtx.impactEventQueueIdx != 0xFFFF && reactionMainCtx.field_18 != 1) {
        gameDisplayBarrierEffects();
        auto& damageDisplayCtx = *((DamageDisplayEffectCtx*)srCreateEffect(gameDisplayDamageNumbers, EFFECT60));
        damageDisplayCtx.actionFlags = reactionMainCtx.actionFlags;
        damageDisplayCtx.dealtDamage = reactionMainCtx.dealtDamage;
        damageDisplayCtx.damagedActorIdx = reactionMainCtx.damagedActorIdx;

        auto& inflictDamageCtx = *((TriggerDamageEffectCtx*)srCreateEffect(srTriggerDamageEffect, EFFECT60));
        inflictDamageCtx.damageEventQueueIdx = reactionMainCtx.damageEventQueueIdx;
        inflictDamageCtx.impactEventQueueIdx = reactionMainCtx.impactEventQueueIdx;
    }
    /*Routine dispatches impact effects, sounds, etc. May modularize in the future, for now delegation makes sense*/
    gameHandleTargetReactions();

}

void triggerScreenFlash(u8 red, u8 green, u8 blue) {
    u32* dword_BFB2E0 = (u32*)0xBFB2E0;
    *dword_BFB2E0 = sub_5BD436(250, 250, 250);
}
