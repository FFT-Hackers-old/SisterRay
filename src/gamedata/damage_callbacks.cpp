#include "damage_callbacks.h"
#include "../battle/scene_globals.h"
#include "../battle/engine/damage_events.h"
#include "../impl.h"
#include "status_names.h"
#include "element_names.h"

void calculateDamage(DamageCalculationEvent* srDamageEvent, u8 attackerID, u8 targetID) {
    auto damageContext = srDamageEvent->damageContext;
    auto& aiContext = *srDamageEvent->aiContext;
    auto& damageEvent = *newDamageEvent();
    damageEvent.targetID = targetID;
    damageEvent.attackerID = attackerID;
    damageEvent.specialDamageFlags = 0;
    aiContext.actorAIStates[targetID].lastCovered = -1;
    handleCover(damageEvent);
    setTargetContext(damageEvent->originalTarget, srDamageEvent);
    attemptStatusInfliction(srDamageEvent);

    if (damageEvent.targetID != targetID) {
        sub_436E92();
        damageContext->abilityFlags1 |= 0x20;
    }
    u8 targetID = damageContext->targetID;
    if (!(damageContext->specialAbilityFlags & 1))
        damageContext->abilityFlags2 |= 4;
    if (aiContext.actorAIStates[targetID].stateFlags & 0x4000)
        damageContext->abilityFlags1 |= 1u;

    if (!(damageContext->abilityFlags1 & 1))
        runFormulas();

    handleAdditionalEffects(3);
    if (!damageContext->abilityPower)
        damageContext->abilityFlags1 |= 2u;
    /*if (sub_5DC08E(targetID))
        damageContext->elementalDefenseMask = 32;*/
    applyElementalModifiers();
    const auto& activeStatuses = *srDamageEvent->srDamageContext->targetState->activeStatuses;
    bool isDead = std::find_if(activeStatuses.begin(), activeStatuses.end(), [&](ActiveStatus activeStatus) {return activeStatus.statusName == StatusNames::DEATH});
    bool isReflectable = !(damageContext->specialAbilityFlags & 0x200) && !((1 << targetID) & word_C3F37C[0]) && damageContext->targetStatusMask & 0x40000;
    bool elementalMiss = (isDead) && !(damageContext->elementalDefenseMask & 0xC1));
    if (!(damageContext->specialAbilityFlags & 0x100) && !isReflectable && !elementalMiss) {
        damageContext->abilityFlags1 |= 1u;
    }
    if (damageContext->abilityFlags1 & 1) {
        nullifyAction(srDamageEvent);
    }
    else {
        damageContext->incOnDamageDealt++;
        damageEvent.specialDamageFlags |= 1;
        handleAdditionalEffects(4);
        if (damageContext->attackerID != targetID)
            damageContext->damageCalcMask |= 1 << targetID;
        if (damageContext->abilityFlags1 & 4 && damageContext->attackHitCount <= 8)
            handleSteal();
        applyReflect(srDamageEvent);
        damageEvent.specialDamageFlags |= 2; //Display reflect flag
        // Set Barrier/MBarrier flags
        if (damageContext->abilityFlags1 & 0x4000) {
            damageEvent.specialDamageFlags |= 0x10;
        }
        if (damageContext->abilityFlags1 & 0x8000) {
            damageEvent.specialDamageFlags |= 0x20;
        }
    }
    applyDamageCaps(srDamageEvent);
    handleReactions(srDamageEvent);
    handleStatusInfliction(srDamageEvent);
    handleDamage(srDamageEvent);

    if ((damageContext->abilityFlags1 & 0x40001) == 262145) {
        damageContext->abilityFlags1 |= 0xFD;
    }
    // where impact events are made
    setupGameDamageAndImpactEvents(srDamageEvent);
    if (!(damageContext->specialAbilityFlags & 0x10))
        sub_5DF3ED();
    if (damageContext->miscActionFlags & 0x80)
        sub_5DF30B(damageContext->currentUnitIDTempt, damageContext->targetID, damageContext->currentDamage / 100, 1);
    if (damageContext->miscActionFlags & 0x40)
        sub_5DF30B(damageContext->currentUnitIDTempt, damageContext->targetID, damageContext->currentDamage / 10, 2);
    if (targetID < 3 && damageContext->attackerID >= 4 && damageContext->unkActorBlockPtr->learnedESkillsMask && damageContext->commandIndexCopy == 13) {
        handleLearnESkill();
    }
    damageEvent.targetStatusMask = aiContext.actorAIStates[targetID].statusMask;
    damageEvent.damagedAnimScriptIdx = damageContext->targetReactionAnimation;

    if (damageContext->abilityFlags1 & 0x20)
        damageEvent.damagedAnimScriptIdx = 9;

    handleDeathImpactSetup(srDamageEvent);
}

//Hit and Damage Formulas will now live in a registry
void runFormulas(DamageCalculationEvent* srDamageEvent) {
    counter = dword_C069C0[damageContext->formulaType];
    chosen_formula = gFormulaPtrTable[damageContext->formulaID];
    while (1) {
        u8 type_order_array_index = (gFormulaExecutionOrderArray[counter++];
        if (type_order_array_index == 8)
            break;
        gHitFormulaTable[type_order_array_index]();
    }
    if (damageContext->abilityPower) {
        if (chosen_formula) {
            handleAdditionalEffects(0);
            ((void(__cdecl*)(signed int))chosen_formula)(8);
            handleAdditionalEffects(1);
        }
    }
}


typedef u32(*SRPFN_GETINFLICTRAND)(u32);
#define gameGetInflictRand (SRPFN_GETINFLICTRAND(0x5C8BDC))
void handleCover(DamageCalculationEvent* srDamageEvent) {
    auto& damageContext = *srDamageEvent->damageContext;
    auto& damageEvent = *srDamageEvent->gameDamageEvent;
    auto& aiContext = *srDamageEvent->aiContext;
    u8 originalTarget = damageEvent->originalTarget;
    u16 someMask = 0;
    for (u8 partyIdx = 0; partyIdx < 3; ++partyIdx) {
        someMask = word_9A88AA[partyIdx];
        if ((1 << originalTarget) & someMask)
            break;
    }
    bool validCoverTarget = (originalTarget < 3 && damageContext.abilityPower && originalTarget != damageContext.attackerID && damageContext.targetCount == 1);
    bool validCoverCommand = (damageContext.commandIndexCopy != 32 || damageContext.animationEffectID == 0xFF);
    const auto& toAdd = srDamageEvent->srDamageContext->toAddStatuses;
    const auto& toRemove = srDamageEvent->srDamageContext->toRemoveStatuses;
    const auto& toToggle = srDamageEvent->srDamageContext->toToggleStatuses;
    bool inflictingToad = (std::find(toAdd.begin(), toAdd.end(), StatusNames::FROG) != toAdd.end())
        && (std::find(toRemove.begin(), toRemove.end(), StatusNames::FROG) != toRemove.end())
        && (std::find(toToggle.begin(), toToggle.end(), StatusNames::FROG) != toToggle.end());

    if (validCoverTarget && !(damageContext.specialAbilityFlags & 4) && !inflictingToad && validCoverCommand) {
        auto candidateCoverers = 0;
        for (u8 partyIdx = 0; partyIdx < 3; ++partyIdx) {
            if (originalTarget == partyIdx)
                continue;
            if (damageContext.attackerID == partyIdx)
                continue;
            const auto& actorState = gContext.battleActors.getActiveBattleActor(partyIdx);
            const auto& partyState = gContext.party.getActivePartyMember(partyIdx);
            if (!(partyState.gamePartyMember->coverChance))
                continue;

            if (!(actorState.actorTimers.unkActorFlags & 8) && (1 << partyIdx) & someMask && !(AI_BATTLE_STATE.actorAIStates[partyIdx].statusMask & 0x86805C45)) {
                u8 coverChance = partyState.gamePartyMember->coverChance;
                if (coverChance > gameGetInflictRand(100))
                    candidateCoverers |= 1 << partyIdx;
            }
        }
        u16 coveringActor = getRandomTarget((aiContext.actorPartyMask & candidateCoverers));
        if (coveringActor) {
            damageEvent.targetID = sub_5C8041(coveringActor);
            damageEvent.attackerID = originalTarget;
            aiContext.actorAIStates[damageEvent.targetID].lastCovered = aiContext.actorAIStates[originalTarget].charID;
        }
    }
}

void setupGameDamageAndImpactEvents(DamageCalculationEvent* srDamageEvent) {
    auto& damageContext = *srDamageEvent->damageContext;
    auto& damageEvent = *srDamageEvent->gameDamageEvent;
    auto& aiContext = *srDamageEvent->aiContext;
    damageEvent.targetStatusMask = aiContext.actorAIStates[srDamageEvent->damageContext.originalTarget].statusMask;
    damageEvent.damagedAnimScriptIdx = damageContext.targetReactionAnimation;

    if (damageContext->abilityFlags1 & 2) {
        if (0x800000 & damageContext->abilityFlags1)
            sub_5DC323(damageEvent);
    }
    else {
        initializeImpactEvent(
            damageEvent,
            damageContext->killRecoverDamage,
            damageContext->abilityFlags2,
            damageContext->attackImpactSoundID,
            damageContext->attackImpactEffectID);
    }

    if (damageContext->abilityFlags1 & 0x20)
        damageEvent.damagedAnimScriptIdx = 9;
}

void handleDeathImpactSetup(DamageCalculationEvent* srDamageEvent) {
    auto& damageContext = *srDamageEvent->damageContext;
    auto& damageEvent = *srDamageEvent->gameDamageEvent;
    auto& aiContext = *srDamageEvent->aiContext;
    if (aiContext.actorAIStates[srDamageEvent->damageContext.originalTarget].statusMask & 1) {
        damageEvent.specialDamageFlags |= 4;
        damageEvent.specialDamageFlags &= 0xFFF7u;
        damageContext.wasKilledMask |= 1 << damageContext.targetID;

        //Handle Flash
        if (damageContext.commandIndexCopy == 26) {
            if (gUnkBattleStructArray[damageContext.targetID].deathType < 17)
                gUnkBattleStructArray[damageContext.targetID].deathType = 8;
            initializeImpactEvent(damageEvent, -2, 0, damageContext.attackImpactSound, damageContext.impactEffectID);
        }
    }
}

void setTargetContext(u8 targetID, DamageCalculationEvent* srDamageEvent) {
    auto damageContext = srDamageEvent->damageContext;
    auto& targetBattleVars = *srDamageEvent->targetState->actorBattleVars;

    damageContext->targetID = targetID;
    damageContext->targetEnemyIndex = -1;
    damageContext->unkDword10 = 0;
    damageContext->abilityFlags2 = 0;
    damageContext->targetDefense = 0;
    damageContext->currentDamage = 0;
    damageContext->targetStateFlags = 0;
    damageContext->abilityFlags1 = damageContext->miscActionFlags;
    damageContext->finalHitRate = damageContext->abilityHitRate;
    damageContext->targetReactionAnimation = targetBattleVars.damageAnimID;
    damageContext->targetStatusMask = targetBattleVars.statusMask;
    damageContext->targetLevel = targetBattleVars.level;
    damageContext->targetCurrentHP = targetBattleVars.currentHP;
    damageContext->targetCurrentMP = targetBattleVars.currentMP;
    damageContext->vTimerBlock = &gActorTimerBlock[targetActorID];
    if (targetID >= 3)
        coverChance = (UnkActorStateStruct*)-1;
    else
        coverChance = &unkActorArray[targetActorID];
    damageContext->party34Block = coverChance;
    damageContext->attackImpactSoundID = -1;
    damageContext->attackImpactEffectID = -1;
    damageContext->killRecoverDamage = -1;
    if (!damageContext->abilityPower)
        damageContext->targetReactionAnimation = 51;

    else if (!damageContext->abilityPower) {
        damageContext->abilityFlags1 |= 1u;
    }

    damageContext->elementalDefenseMask = 0;
    damageContext->currentDamage = 0;

    if (damageContext->specialAbilityFlags & 0x400) {
        if (damageContext->specialAbilityFlags & 4)
            damageContext->targetDefense = getStatWithModifiers(
                targetActorID,
                AI_BATTLE_STATE.actorAIStates[targetActorID].mdefense,
                3);
        else
            damageContext->targetDefense = getStatWithModifiers(
                targetActorID,
                AI_BATTLE_STATE.actorAIStates[targetActorID].defense,
                2);
    }
    if (damageContext->targetDefense > 512)
        damageContext->targetDefense = 512;
    if (targetActorID >= 4)
        damageContext->targetEnemyIndex = formationData[8 * (targetActorID - 4)];
}

void attemptStatusInfliction(DamageCalculationEvent* srDamageEvent) {
    auto damageContext = srDamageEvent->damageContext;
    damageContext->addStatusMaskCopy = 0;
    damageContext->rmStatusMaskCopy = 0;
    damageContext->changeStatusMaskCopy = 0;
    damageContext->combinedStatusMask = 0;

    auto& inflictions = srDamageEvent->srDamageContext->statusToInflict;
    //Iterate over inflictions
    auto remove = std::vector<std::string>();
    auto add = std::vector<std::string>();
    auto toggle = std::vector<std::string>();
    for (const auto& infliction : inflictions) {
        if (didInflictionSucceed(infliction, srDamageEvent)) {
            if (infliction.doesToggle) {
                toggle.push_back(infliction);
                continue;
            }
            if (infliction.doesRemove) {
                remove.push_back(infliction);
                continue;
            }
            add.push_back(infliction);
        }
    }
    srDamageEvent->srDamageContext->toAddStatuses = add;
    srDamageEvent->srDamageContext->toRemoveStatuses = remove;
    srDamageEvent->srDamageContext->toToggleStatuses = toggle;

    damageContext->targetStatusImmuneMask = sub_434568(damageContext->targetID, 1, (damageContext->rmStatusMaskCopy & 1) != 0);
}

bool didInflictionSucceed(StatusInfliction infliction, DamageCalculationEvent* srDamageEvent) {
    auto damageContext = srDamageEvent->damageContext;

    /*auto levelDelta = damageContext->attackerLevel - damageContext->targetLevel;
    auto magDelta = AI_BATTLE_STATE.actorAIStates[damageContext->attackerID].magAtk - AI_BATTLE_STATE.actorAIStates[damageContext->targetID].mdefense;*/

    const auto& inflictions = srDamageEvent->srDamageContext->statusToInflict;
    auto inflictChance = 4 * infliction.inflictionChance;
    if ((damageContext->targetStatusMask & 0x800) && std::find_if(
        inflictions.begin(), inflictions.end(), [&](StatusInfliction elmnt) {return elmnt.statusName == StatusNames::FROG}) != inflictions.end())
        inflictChance = 252;
    if ((damageContext->targetStatusMask & 0x1000) && std::find_if(
        inflictions.begin(), inflictions.end(), [&](StatusInfliction elmnt) {return elmnt.statusName == StatusNames::SMALL}) != inflictions.end())
        inflictChance = 252;
    if (damageContext->targetID < 3 && aRcBattleBattle & combinedMask)
        inflictChance = 252;

    if (inflictChance < 252) {
        inflictChance = damageContext->MPTurboBoost * inflictChance / 100 + inflictChance;
        if ((damageContext->abilityTargetingFlags & 0xC) != 4 && damageContext->targetCount > 1)
            inflictChance = 2 * inflictChance / 3;
        if (damageContext->quadCount)
            inflictChance >>= 1;
    }
    // apply target resistances for this status, reducing infliction chance
    const auto& status = gContext.statuses.getElement(infliction.statusName);
    auto res = srDamageEvent->srDamageContext->targetState->battleStats->at(status.resName).activeValue;
    auto pen = srDamageEvent->srDamageContext->attackerState->battleStats->at(status.penName).activeValue + srDamageEvent->srDamageContext->attackStats[status.penName].statValue;
    auto modifier = res - pen;
    inflictChance -= (modifier / 100) * inflictChance;
    // return no to death immunity in weird context
    auto isInflictingDeath = std::find_if(inflictions.begin(), inflictions.end(), [&](StatusInfliction elmnt) {return elmnt.statusName == StatusNames::DEATH}) != inflictions.end();
    if (isInflictingDeath && (1 << damageContext->targetID) & gEscapedActorsMask)
        return false;

    if (inflictChance >= gameGetInflictRand(100) + 1) {
        return true;
    }
    return false;
}

void handleStatusInfliction(CommandSetupEvent* setupEvent, DamageCalculationEvent* srDamageEvent) {
    auto damageContext = setupEvent.damageContext;
    auto srDamageContext = setupEvent->srDamageContext;
    auto& aiContext = *setupEvent.aiContext;
    u8 targetID = damageContext->originalTarget;
    if (damageContext->abilityFlags1 & 1) {
        damageContext->abilityFlags1 |= 0x800000;
        return;
    }

    auto& targetStatuses = *srDamageContext->targetState->activeStatuses;
    auto& addStatuses = srDamageContext->toAddStatuses;
    auto& removeStatuses = srDamageContext->toRemoveStatuses;
    auto& toggleStatuses = srDamageContext->toToggleStatuses;
    for (const auto& statusName : toggleStatuses) {
        auto wasFound = std::find_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {return statusName == activeStatus.statusName}) != targetStatuses.end();
        if (wasFound) {
            removeStatuses.push_back(statusName);
            continue;
        }
        addStatuses.push_back(statusName);
    }

    u32 previousTargetStatus = 0;
    for (const auto& activeStatus : targetStatuses) {
        const auto& status = gContext.statuses.getElement(activeStatus.statusName);
        if (status.isGameStatus) {
            previousTargetStatus& (1 << status.gameIndex);
        }
    }
    // add every status to be added
    for (auto statusName : addStatuses) {
        auto wasFound = std::find_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {return statusName == activeStatus.statusName}) != targetStatuses.end();
        const auto& status = gContext.statuses.getElement(statusName);

        if (!wasFound || status.allowMultiple) {
            if (!status.removeOnInflict.empty()) {
                auto it = std::remove_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {
                    return std::find(status.removeOnInflict.begin(), status.removeOnInflict.end(), activeStatus.statusName) != status.removeOnInflict.end()
                    });
            }
            if (!status.neutralizeOnInflict.empty()) {
                auto it = std::remove_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {
                    return std::find(status.neutralizeOnInflict.begin(), status.neutralizeOnInflict.end(), activeStatus.statusName) != status.neutralizeOnInflict.end()
                    });
                if (it != targetStatuses.end()) {
                    continue;
                }
            }
            ActiveStatus activeStatus = ActiveStatus{ statusName };
            targetStatuses.push_back(activeStatus);
        }
    }
    //Remove all stats to be removed
    targetStatuses.erase(
        std::remove_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {
            return std::find_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {
                return (std::find(removeStatuses.begin(), removeStatuses.end(), activeStatus.statusName) != removeStatuses.end())}) != targetStatuses.end()}));

    //now that Active Statuses have been synced, set game statuses appropriately
    u32 newTargetStatus = 0;
    for (const auto& activeStatus : targetStatuses) {
        const auto& status = gContext.statuses.getElement(activeStatus.statusName);
        if (status.isGameStatus) {
            newTargetStatus& (1 << status.gameIndex);
        }
    }
    aiContext.actorAIStates[targetID].statusMasks = newTargetStatus;

    //If no status was inflicted
    if (previousTargetStatus == newTargetStatus) {
        damageContext->abilityFlags1 |= 0x800000;
    }
    else {
        if (damageContext->combinedStatusMask & newTargetStatus && damageContext->attackerID != targetID)
            damageContext->actionCounterable |= 1 << targetID;

        //Handle the case where death has been inflicted
        if ((newTargetStatus ^ previousTargetStatus) & 1) {
            gameSetHitReaction(previousTargetStatus & 1);
        }
        else {
            srDamageEvent->gameDamageEvent->specialDamageFlags |= 8;
        }
    }
}

typedef void(*SRPFN_HANDLEDAMAGEORHEAL)(u32, u32, u32 bool);
#define handleDamageOrHeal (SRPFN_HANDLEDAMAGEORHEAL(0x5C8126))
void handleDamage(DamageCalculationEvent* srDamageEvent) {
    auto damageContext = srDamageEvent->damageContext;
    auto& aiContext = *srDamageEvent->aiContext;
    auto& actorState = aiContext.actorAIStates[damageContext->targetID];
    auto& target34 = *srDamageEvent->targetState->party34;
    auto& targetTimers = *srDamageEvent->targetState->actorTimers;
    auto isHeal = damageContext->abilityFlags2 & 1;

    if (!damageContext->currentDamage)
        return;

    if (damageContext->abilityFlags2 & 4) {
        actorState.currentMP = handleDamageOrHeal(actorState.currentMP, damageContext->currentDamage, actorState.maxMP, isHeal);
        return;
    }

    u32 previousHP = actorState.currentHP;
    actorState.currentHP = handleDamageOrHeal(actorState->currentHP, damageContext->currentDamage, actorState->maxHP, isHeal);
    if (!actorState.currentHP) {
        if (previousHP) {
            //Handle Death inflicted
            actorState.statusMask |= 1u;
            if (damageContext->miscActionFlags & 0x2000)
                handleMorph();
        }
    }

    //Handle Limit Fill
    if (damageContext->targetID < 3 && damageContext->attackerID >= 4 && !isHeal && (target34.limitLevelIdx < 4) && !(targetTimers.unkActorFlags & 8)) {
        auto limitFill = 256;
        if (damageContext->targetStatusMask & 0x20) {
            limitFill = 512;
        }
        else if (damageContext->targetStatusMask & 0x10) {
            limitFill = 128;
        }
        u16 limitIncrement = limitFill * ((300 * damageContext->currentDamage) / actorState->maxHP);
        limitIncrement /= target34.limitLevelDivisor;
        u16 newLimitValue = target34.limitBar + limitIncrement;
        if (newLimitValue > 255)
            newLimitValue = 255;
        target34.limitBar = newLimitValue;
    }
}

typedef void(*SRPFN_GAMESETREACTIONANIM)(bool);
#define gameSetHitReaction (SRPFN_GAMESETREACTIONANIM(0x5DB8D6))
void handleReactions(DamageCalculationEvent* srDamageEvent) {
    auto damageContext = srDamageEvent->damageContext;
    if (damageContext->abilityFlags1 & 3) {
        if (damageContext->abilityFlags1 & 1) {
            damageContext->attackImpactSound = damageContext->missAtkSound;
        }
        else {
            damageContext->attackImpactSound = damageContext->impactSound;
            if (damageContext->elementalDefenseMask & 1)
                gameSetHitReaction(0);
        }
    }
    else {
        if (!(damageContext->abilityFlags2 & 1) && damageContext->attackerID != damageContext->targetID)
            damageContext->actionCounterable |= 1 << damageContext->targetID;

        if (damageContext->killRecoverDamage == -1)
            damageContext->killRecoverDamage = damageContext->currentDamage;

        damageContext->attackImpactEffectID = damageContext->impactEffectID;

        u16 impactSound = 0;
        if (damageContext->abilityFlags2 & 2)
            impactSound = damageContext->critAtkSound;
        else
            impactSound = damageContext->impactSound;

        damageContext->attackImpactSound = impactSound;
        if (!(damageContext->abilityFlags2 & 1) && damageContext->killRecoverDamage)
            gameSetHitReaction(0);
        else
            damageContext->targetReactionAnimation = 51;
    }
}

void applyDamageCaps(DamageCalculationEvent* srDamageEvent) {
    auto damageContext = srDamageEvent->damageContext;
    auto& target34 = *srDamageEvent->targetState->party34;
    auto& targetBattleVars = *srDamageEvent->targetState->actorBattleVars;
    auto& attackerTimers = *srDamageEvent->attackerState->actorTimers;

    u16 hpDamageCap = 9999;
    u16 mpDamageCap = 999;
    if (damageContext->targetID >= 3) {
        hpDamageCap = 9999;
        mpDamageCap = 999;
    }
    else {
        hpDamageCap = target34.field_16;
        mpDamageCap = target34.field_14;
    }

    auto damageCap = 0;
    if (damageContext->abilityFlags2 & 4)
        damageCap = mpDamageCap;
    else
        damageCap = hpDamageCap;

    if (srDamageEvent->srDamageContext->useActionDamageLimits) {
        if (damageContext->abilityFlags2 & 4)
            damageCap = srDamageEvent->srDamageContext->hpDamageLimit;
        else
            damageCap = srDamageEvent->srDamageContext->mpDamageLimit;
    }

    if (damageContext->currentDamage > damageCap)
        damageContext->currentDamage = damageCap;

    if (damageContext->specialAbilityFlags & 4) {
        if (targetBattleVars.actorFlags & 0x200)
            damageContext->currentDamage = 0;
    }
    else if (targetBattleVars.actorFlags & 0x100) {
        damageContext->currentDamage = 0;
    }

    if (damageContext->currentDamage && attackerTimers.currentHP == 7777)
        damageContext->currentDamage = 7777;
}


typedef void(*SRPFN_GETREFLECTTYPE)(u32, u32);
#define isCrossRowReflect (SRPFN_GETREFLECTTYPE(0x5DB166))
void applyReflect(DamageCalculationEvent* srDamageEvent) {
    auto damageContext = srDamageEvent->damageContext;
    auto& aiContext = *srDamageEvent->aiContext;
    u16* G_ATTACKER_TARGETS = (u16*)0xC3F340;
    u8* reflectTargets = (u8*)(0xC3F358);
    u32* dword_C3F364 = (u32*)(0xC3F364);
    const auto& targetState = *srDamageEvent->srDamageContext->targetState;
    const auto& targetStatuses = targetState.activeStatuses;
    bool reflectIsActive = std::find_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {return activeStatus.statusName = StatusNames::REFLECT}) != targetStatuses.end();
    bool isReflectable = !(damageContext->specialAbilityFlags & 0x200) && !((1 << damageContext->targetID) & word_C3F37C[0]) && reflectIsActive;
    if (isReflectable) {
        u8 reflectTarget;
        //For party actors, just reflect to attacker
        if (isCrossRowReflect(damageContext->attackerID, damageContext->targetID)) {
            reflectTarget = damageContext->attackerID;
        }
        else {
            if (reflectTargets[damageContext->targetID] == -1) {
                u16 reflectMask = sub_5DB199(damageContext->targetID);
                reflectTargets[damageContext->targetID] = sub_5C8041(reflectMask);
            }
            reflectTarget = reflectTargets[damageContext->targetID];
        }
        *dword_C3F364 |= 2u;
        G_ATTACKER_TARGETS[damageContext->targetID] |= 1 << reflectTarget;
        nullifyAction(srDamageEvent);
        /*if (targetState.actorTimers->innateStatusMask & 0x40000) {
            word_C3F37C[0] |= 1 << damageContext->targetID; //Handle the case where it is auto-reflect, as inflicted by the reflect ring
        }*/
        if (targetState.actorTimers->reflectCount) {
            targetState.actorTimers->reflectCount--;
        }
        else {
            srDamageEvent->srDamageContext->toRemoveStatuses.push_back(StatusNames::REFLECT);
        }
        damageContext->abilityFlags1 |= 2u;
        if (damageContext->targetID < 3)
            damageContext->targetReactionAnimation = 10;
    }
}

// Used when something is reflected etc to null damage and status
void nullifyAction(DamageCalculationEvent* srDamageEvent) {
    auto& srDamageContext = srDamageEvent->srDamageContext;
    srDamageContext->toAddStatuses = std::vector<std::string>();
    srDamageContext->toRemoveStatuses = std::vector<std::string>();
    srDamageContext->toToggleStatuses = std::vector<std::string>();
    srDamageEvent->damageContext->currentDamage = 0;
}
// Much fuller elemental logic, supporting resistances and multiple elements
void applyElementalModifiers(DamageCalculationEvent* setupEvent) {
    auto& damageContext = *setupEvent->damageContext;
    auto& srDamageContext = *setupEvent->srDamageContext;
    auto& attackerState = srDamageContext.attackerState;
    auto& targetState = srDamageContext.attackerState;
    auto& aiContext = *setupEvent->aiContext;
    float elementalDamageModifier = 1;

    for (const auto& name : setupEvent->srDamageContext->attackElements) {
        const auto& element = gContext.elements.getElement(name);
        if (std::find(srDamageContext.attackElements.begin(), srDamageContext.attackElements.end(), name) != srDamageContext.attackElements.end()) {
            auto res = targetState.battleStats[element.resName];
            if (res == 255) {
                aiContext.actorAIStates[damageContext.targetID].currentHP = aiContext.actorAIStates[damageContext.targetID].maxHP;
                aiContext.actorAIStates[damageContext.targetID].currentMP = aiContext.actorAIStates[damageContext.targetID].maxMP;
                damageContext.abilityFlags2 = 1;
                damageContext.abilityFlags1 &= 0xFFFFFFFD;
                srDamageContext.toAddStatuses.erase(std::remove(srDamageContext.toAddStatuses.begin(), srDamageContext.toAddStatuses.end(), StatusNames::DEATH));
                damageContext.killRecoverDamage = -3;
            }
            if (res == -255) {
                std::vector<std::string> toAdd = { StatusNames::DEATH };
                srDamageContext.toAddStatuses = toAdd;
                std::vector<std::string> toRemove = {};
                for (const auto& statusPair : gContext.statuses.named_registry) {
                    if (statusPair.first != StatusNames::DEATH) {
                        toRemove.push_back(statusPair.first);
                    }
                }
                damageContext.abilityFlags1 &= 0xFFFFFFFD;
                damageContext.abilityFlags2 &= 0xFFFFFFFE;
                damageContext.killRecoverDamage = -2;
                break;
            }
            auto pen = attackerState.battleStats[element.penName];
            auto eRes = res - pen;
            auto aff = attackerState.battleStats[element.affName];
            if (eRes > 100) {
                damageContext.abilityFlags2 ^= 1u;
            }
            elementalDamageModifier = elementalDamageModifier - (eRes / 100);
            elementalDamageModifier = (aff / 100) + elementalDamageModifier;
            if (name == ElementNames::RESTORE) {
                damageContext.abilityFlags2 |= 1u;
            }
        }
    }
    damageContext.currentDamage = damageContext.currentDamage * elementalDamageModifier;
}
