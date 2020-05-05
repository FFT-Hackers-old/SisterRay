#include "damage_callback_utils.h"
#include "status_names.h"
#include "../impl.h"

bool actorIsDamageImmune(ActorBattleVars battleVars, ActorBattleState srActorState, bool isMagic) {
    bool isDamageImmune = false;
    if (isMagic) {
        if (battleVars.stateFlags & 0x200)
            isDamageImmune = 1;
    }
    else if (battleVars.stateFlags & 0x100) {
        isDamageImmune = 1;
    }
    bool immuneStatii = (srActorHasStatus(srActorState, StatusNames::SHIELD) || srActorHasStatus(srActorState, StatusNames::PETRIFY));
    return isDamageImmune || immuneStatii;
}


bool srActorHasStatus(const ActorBattleState& srActorState, std::string statusName) {
    const auto& status = gContext.statuses.getElement(statusName);
    const auto& battleVars = *srActorState.actorBattleVars;
    if (status.isGameStatus) {
        bool gameStatusActive = battleVars.statusMask & 1 << status.gameIndex;
        bool srStatusActive = std::find_if(srActorState.activeStatuses->begin(), srActorState.activeStatuses->end(),
            [&](ActiveStatus status){return status.statusName == statusName; }) != srActorState.activeStatuses->end();
        if (srStatusActive != gameStatusActive) {
            srLogWrite("sr status and game status are not synced for status %s", statusName.c_str());
        }
        return gameStatusActive && srStatusActive;
    }
    else {
        return std::find_if(srActorState.activeStatuses->begin(), srActorState.activeStatuses->end(),
            [&](ActiveStatus status) {return status.statusName == statusName; }) != srActorState.activeStatuses->end();
    }
}

bool srActorHasStatus(const std::vector<ActiveStatus>& activeStatuses, std::string statusName) {
    const auto& status = gContext.statuses.getElement(statusName);
    if (status.isGameStatus) {
        bool srStatusActive = std::find_if(activeStatuses.begin(), activeStatuses.end(),
            [&](ActiveStatus status) {return status.statusName == statusName; }) != activeStatuses.end();
        return srStatusActive;
    }
    else {
        return std::find_if(activeStatuses.begin(), activeStatuses.end(),
            [&](ActiveStatus status) {return status.statusName == statusName; }) != activeStatuses.end();
    }
}

void setActionDidHit(DamageCalculationEvent* dmgEvent, bool didHit) {
    if (!didHit) {
        dmgEvent->damageContext->abilityFlags1 &= 1;
    }
    else {
        if (dmgEvent->damageContext->targetStateFlags & 2)
            dmgEvent->aiContext->actorAIStates[dmgEvent->damageContext->targetID].stateFlags ^= 0x80u;
    }
}

bool checkActionDidHit(DamageCalculationEvent* dmgEvent) {
    return !(dmgEvent->damageContext->abilityFlags1 & 1);
}

bool isHitFormulaModifierActive(DamageCalculationEvent* srDamageEvent, HitModifiers modifier) {
    auto& hitModifiers = srDamageEvent->srDamageContext->hitFormulaModifiers;
    return std::find(hitModifiers.begin(), hitModifiers.end(), modifier) != hitModifiers.end();
}

bool isDmgFormulaModifierActive(DamageCalculationEvent* srDamageEvent, DamageModifiers modifier) {
    auto& damageModifiers = srDamageEvent->srDamageContext->dmgFormulaModifiers;
    return std::find(damageModifiers.begin(), damageModifiers.end(), modifier) != damageModifiers.end();
}

void setDisplayBarrier(DamageCalculationEvent* srDamageEvent) {
    srDamageEvent->damageContext->abilityFlags1 |= 0x4000;
}

void setDisplayMBarrier(DamageCalculationEvent* srDamageEvent) {
    srDamageEvent->damageContext->abilityFlags1 |= 0x8000;
}

bool checkDisplayBarrier(const DamageCalculationEvent* srDamageEvent) {
    return srDamageEvent->damageContext->abilityFlags1 & 0x4000;
}

bool checkDisplayMBarrier(const DamageCalculationEvent* srDamageEvent) {
    return srDamageEvent->damageContext->abilityFlags1 & 0x8000;
}

bool actionIsReflectable(const DamageCalculationEvent* srDamageEvent) {
    return !(srDamageEvent->damageContext->specialAbilityFlags & 0x200);
}

bool actionOnlyHitsDead(const DamageCalculationEvent* srDamageEvent) {
    return !(srDamageEvent->damageContext->specialAbilityFlags & 0x100);
}

void setStatusWasChanged(DamageCalculationEvent* srDamageEvent, bool wasChanged) {
    if (!wasChanged) {
        srDamageEvent->damageContext->abilityFlags1 |= 0x800000;
        return;
    }
    srDamageEvent->damageContext->abilityFlags1 &= (~0x800000);
}

bool wasStatusChanged(const DamageCalculationEvent* srDamageEvent) {
    return !(srDamageEvent->damageContext->abilityFlags1 & 0x800000);
}


SISTERRAY_API bool maskIsSingleTarget(u16 targetMask) {
    return targetMask > 0 && (targetMask & (targetMask - 1)) ==  0;
}


SISTERRAY_API u8 getFirstMaskBitIdx(u16 targetMask) {
    if (targetMask == 0) {
        return 0xFF;
    }
    for (auto idx = 0; idx < 10; idx++) {
        if (targetMask & (1 << idx))
            return idx;
    }
}
