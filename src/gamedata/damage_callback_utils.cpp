#include "damage_callback_utils.h"
#include "status_names.h"
#include "../impl.h"
#include <algorithm>

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


bool srActorHasStatus(const ActorBattleState& srActorState, const std::string statusName) {
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

bool didInflictionSucceed(StatusInfliction infliction, ActorBattleState attackerState, ActorBattleState& targetState, std::unordered_map<std::string, SrStaticStat> attackStats) {
    auto inflictChance = 4 * infliction.inflictionChance;
    if (inflictChance > 252) {
        inflictChance = 252;
    }
    srLogWrite("Infliction Chance: %i", inflictChance);

    const auto& status = gContext.statuses.getElement(infliction.statusName);
    auto res = targetState.battleStats->at(status.resName).activeValue;
    auto pen = attackerState.battleStats->at(status.penName).activeValue + attackStats[status.penName].statValue;
    auto modifier = res - pen;
    inflictChance -= (modifier / 100) * inflictChance;

    if (inflictChance >= udist(rng)) {
        srLogWrite("INFLICTION SUCCESSFUL");
        return true;
    }
    srLogWrite("INFLICTION FAILED");
    return false;
}

bool srInflictStatus(ActorBattleState& targetState, std::string statusName) {
    srLogWrite("ATTEMPTING TO INFLICT STATUS: %s", statusName.c_str());
    auto& targetStatuses = *targetState.activeStatuses;
    auto wasFound = (std::find_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {return statusName == activeStatus.statusName; }) != targetStatuses.end());
    const auto& status = gContext.statuses.getElement(statusName);
    if (!wasFound || status.behavior.allowMultiple) {
        if (!status.removeOnInflict.empty()) {
            auto it = std::remove_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {
                return std::find(status.removeOnInflict.begin(), status.removeOnInflict.end(), activeStatus.statusName) != status.removeOnInflict.end();
                });
        }
        if (!status.neutralizeOnInflict.empty()) {
            auto it = std::remove_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {
                return std::find(status.neutralizeOnInflict.begin(), status.neutralizeOnInflict.end(), activeStatus.statusName) != status.neutralizeOnInflict.end();
                });
        }
        ActiveStatus activeStatus = ActiveStatus{ statusName };
        targetStatuses.push_back(activeStatus);
    }
    if (status.isGameStatus) {
        targetState.actorBattleVars->statusMask |= (1 << status.gameIndex);
    }
    return true;
}

bool srRemoveStatus(ActorBattleState& targetState, std::string statusName) {
    auto& targetStatuses = *targetState.activeStatuses;
    auto wasFound = (std::find_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {return statusName == activeStatus.statusName; }) != targetStatuses.end());
    const auto& status = gContext.statuses.getElement(statusName);
    if (wasFound) {
        targetStatuses.erase(std::remove_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) { return statusName == activeStatus.statusName;  }), targetStatuses.end());
        return true;
    }
    return false;
}


void setActionDidHit(DamageCalculationEvent* dmgEvent, bool didHit) {
    if (!didHit) {
        dmgEvent->damageContext->hitTypeFlags |= 1;
    }
    else {
        dmgEvent->damageContext->hitTypeFlags &= ~1;
        if (dmgEvent->damageContext->targetStateFlags & 2)
            dmgEvent->aiContext->actorAIStates[dmgEvent->damageContext->targetID].stateFlags ^= 0x80u;
    }
    srLogWrite("Damage hit flags: %x", dmgEvent->damageContext->hitTypeFlags);
}

bool checkActionDidHit(DamageCalculationEvent* dmgEvent) {
    return !(dmgEvent->damageContext->hitTypeFlags & 1);
}

bool isHitFormulaModifierActive(DamageCalculationEvent* srDamageEvent, HitModifiers modifier) {
    auto& hitModifiers = srDamageEvent->srDamageContext->hitFormulaModifiers;
    return (std::find(hitModifiers.begin(), hitModifiers.end(), modifier) != hitModifiers.end());
}

bool isDmgFormulaModifierActive(DamageCalculationEvent* srDamageEvent, DamageModifiers modifier) {
    auto& damageModifiers = srDamageEvent->srDamageContext->dmgFormulaModifiers;
    return std::find(damageModifiers.begin(), damageModifiers.end(), modifier) != damageModifiers.end();
}

void setDisplayBarrier(DamageCalculationEvent* srDamageEvent) {
    srDamageEvent->damageContext->hitTypeFlags |= 0x4000;
}

void setDisplayMBarrier(DamageCalculationEvent* srDamageEvent) {
    srDamageEvent->damageContext->hitTypeFlags |= 0x8000;
}

bool checkDisplayBarrier(const DamageCalculationEvent* srDamageEvent) {
    return srDamageEvent->damageContext->hitTypeFlags & 0x4000;
}

bool checkDisplayMBarrier(const DamageCalculationEvent* srDamageEvent) {
    return srDamageEvent->damageContext->hitTypeFlags & 0x8000;
}

bool actionIsReflectable(const DamageCalculationEvent* srDamageEvent) {
    return !(srDamageEvent->damageContext->specialAbilityFlags & 0x200);
}

bool actionOnlyHitsDead(const DamageCalculationEvent* srDamageEvent) {
    return !(srDamageEvent->damageContext->specialAbilityFlags & 0x100);
}

void setStatusWasChanged(DamageCalculationEvent* srDamageEvent, bool wasChanged) {
    if (!wasChanged) {
        srDamageEvent->damageContext->hitTypeFlags |= 0x800000;
        return;
    }
    srDamageEvent->damageContext->hitTypeFlags &= (~0x800000);
}

bool wasStatusChanged(const DamageCalculationEvent* srDamageEvent) {
    return !(srDamageEvent->damageContext->hitTypeFlags & 0x800000);
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
