#include "damage_callbacks.h"
#include "../battle/scene_globals.h"
#include "../battle/engine/damage_events.h"
#include "../impl.h"
#include "status_names.h"
#include "element_names.h"
#include "damage_callback_utils.h"
#include <algorithm>

typedef void(*SRPFN_SUB436E92)();
#define gameResetAnimEventCamera   ((SRPFN_SUB436E92)0x436E92)
void calculateDamage(DamageCalculationEvent* srDamageEvent, u8 attackerID, u8 targetID) {
    auto damageContext = srDamageEvent->damageContext;
    auto& aiContext = *srDamageEvent->aiContext;
    auto& gameDamageEvent = *srDamageEvent->gameDamageEvent;
    gameDamageEvent.targetID = targetID;
    gameDamageEvent.attackerID = attackerID;
    gameDamageEvent.specialDamageFlags = 0;
    aiContext.actorAIStates[targetID].lastCovered = -1;
    printDamageEvent(&gameDamageEvent);
    printDamageQueueState();
    /*srLogWrite("Running Cover Callback");
    handleCover(srDamageEvent);*/
    printDamageEvent(&gameDamageEvent);
    printDamageQueueState();
    srLogWrite("Cover callback exeucted");
    srLogWrite("setting Target Context");
    setTargetContext(gameDamageEvent.targetID, srDamageEvent);
    printDamageEvent(&gameDamageEvent);
    printDamageQueueState();
    srLogWrite("Target Context set");
    srLogWrite("Setting up status infliction");
    attemptStatusInfliction(srDamageEvent);
    srLogWrite("Status Infliction setup");

    if (gameDamageEvent.targetID != targetID) {
        srLogWrite("Handling Retarget");
        gameResetAnimEventCamera();
        damageContext->abilityFlags1 |= 0x20;
    }

    u8 targetIDLocal = damageContext->targetID;
    if (!(damageContext->specialAbilityFlags & 1))
        damageContext->abilityFlags2 |= 4;
    if (aiContext.actorAIStates[targetID].stateFlags & 0x4000)
        damageContext->abilityFlags1 |= 1u;

    srLogWrite("Preparing to run formulas");
    runFormulas(srDamageEvent);
    srLogWrite("Formulas executed");

    gContext.eventBus.dispatch(PRE_DAMAGE_DEALT, (void*)srDamageEvent);
    if (!damageContext->abilityPower)
        damageContext->abilityFlags1 |= 2u;
    /*if (sub_5DC08E(targetID))
        damageContext->elementalDefenseMask = 32;*/
    srLogWrite("applying Elemental Modifiers");
    applyElementalModifiers(srDamageEvent);
    srLogWrite("checking Damage Successful");
    handleDamageSuccessful(srDamageEvent);
    srLogWrite("applying Damage caps");
    applyDamageCaps(srDamageEvent);
    srLogWrite("Setting target hit reaction Data");
    handleDamageReaction(srDamageEvent);
    srLogWrite("Inflicting Statuses");
    handleStatusInfliction(srDamageEvent);
    handleDamage(srDamageEvent);

    if ((damageContext->abilityFlags1 & 0x40001) == 262145) {
        damageContext->abilityFlags1 |= 0xFD;
    }
    // where impact events are made
    setupGameDamageAndImpactEvents(srDamageEvent);
    handleHealEffects(srDamageEvent);

    /*if (targetID < 3 && damageContext->attackerID >= 4 && damageContext->unkActorBlockPtr->learnedESkillsMask && damageContext->commandIndexCopy == 13) {
        handleLearnESkill();
    }*/
    gameDamageEvent.targetStatusMask = aiContext.actorAIStates[targetID].statusMask;
    gameDamageEvent.damagedAnimScriptIdx = damageContext->targetReactionAnimation;

    if (damageContext->abilityFlags1 & 0x20)
        gameDamageEvent.damagedAnimScriptIdx = 9;

    handleDeathImpactSetup(srDamageEvent);
    printDamageEvent(&gameDamageEvent);
    printDamageQueueState();
}

//Hit and Damage Formulas will now live in a registry
void runFormulas(DamageCalculationEvent* srDamageEvent) {
    srLogWrite("Inside run Formulas");
    if (!checkActionDidHit(srDamageEvent))
        return;

    srLogWrite("Action didn't fail, running formulas");
    auto hitFormula = gContext.hitFormulas.getResource(srDamageEvent->srDamageContext->hitFormula);
    auto& hitModifiers = srDamageEvent->srDamageContext->hitFormulaModifiers;
    auto damageFormula = gContext.damageFormulas.getResource(srDamageEvent->srDamageContext->damageFormula);
    auto& damageModifiers = srDamageEvent->srDamageContext->dmgFormulaModifiers;
    if (isHitFormulaModifierActive(srDamageEvent, CHECK_ACCURACY)) {
       gContext.eventBus.dispatch(PRE_HIT_FORMULA, (void*)srDamageEvent);
       hitFormula.baseFormulaCallback(srDamageEvent);
       gContext.eventBus.dispatch(POST_HIT_FORMULA, (void*)srDamageEvent);
       auto rand = udist(rng) / 2;
       if (srDamageEvent->srDamageContext->hitChance_ < rand) {
           srLogWrite("Setting Action Hit: Successful");
           setActionDidHit(srDamageEvent, true);
       }
    }
    else {
        srLogWrite("Setting Action Hit: Successful");
        setActionDidHit(srDamageEvent, true);
    }
    srLogWrite("Executing ability with power: %i", srDamageEvent->damageContext->abilityPower);
    if (srDamageEvent->damageContext->abilityPower) {
        gContext.eventBus.dispatch(PRE_DAMAGE_FORMULA, (void*)srDamageEvent);
        srLogWrite("Calculating damage using a damage formula: attacker: %i, target %i",
            srDamageEvent->damageContext->attackerID, srDamageEvent->damageContext->targetID);
        damageFormula.baseFormulaCallback(srDamageEvent);
        srLogWrite("Formula Damage: %i", srDamageEvent->damageContext->currentDamage);
        gContext.eventBus.dispatch(POST_DAMAGE_FORMULA, (void*)srDamageEvent);
        srLogWrite("Formula Damage after modifiers: %i", srDamageEvent->damageContext->currentDamage);
    }
}

void handleDamageSuccessful(DamageCalculationEvent* srDamageEvent) {
    auto& damageContext = *srDamageEvent->damageContext;
    u16* word_C3F37C = (u16*)(0xC3F37C);
    bool isDead = srActorHasStatus(srDamageEvent->srDamageContext->targetState, StatusNames::DEATH);
    bool targetHasReflect = srActorHasStatus(srDamageEvent->srDamageContext->targetState, StatusNames::REFLECT);
    srLogWrite("Target Has Reflect: %i", targetHasReflect);
    bool isReflectable = actionIsReflectable(srDamageEvent) && !((1 << damageContext.targetID) & *word_C3F37C) && targetHasReflect;
    bool elementalMiss = (isDead) && !(damageContext.elementalDefenseMask & 0xC1);
    if (actionOnlyHitsDead(srDamageEvent) && !isReflectable && !elementalMiss) {
        setActionDidHit(srDamageEvent, false);
    }
    if (!checkActionDidHit(srDamageEvent)) {
        nullifyAction(srDamageEvent);
        return;

    }
    damageContext.incOnDamageDealt++;
    srDamageEvent->gameDamageEvent->specialDamageFlags |= 1;
    gContext.eventBus.dispatch(POST_DAMAGE_DEALT, (void*)srDamageEvent);
    if (damageContext.attackerID != damageContext.targetID)
        damageContext.wasDamagedMask |= 1 << damageContext.targetID;
    if (damageContext.abilityFlags1 & 4 && damageContext.attackHitCount <= 8)
        handleSteal(srDamageEvent);
    applyReflect(srDamageEvent);
    // Set Barrier/MBarrier flags
    if (checkDisplayBarrier(srDamageEvent)) {
        srDamageEvent->gameDamageEvent->specialDamageFlags |= 0x10;
    }
    if (checkDisplayMBarrier(srDamageEvent)) {
        srDamageEvent->gameDamageEvent->specialDamageFlags |= 0x20;
    }
}

typedef u8(*SRPFN_GAMEGETRAND)();
#define gameGetRand  ((SRPFN_GAMEGETRAND)0x5C8BA1)
void handleSteal(DamageCalculationEvent* srDamageEvent) {
    auto& damageContext = *srDamageEvent->damageContext;
    auto& targetState = srDamageEvent->srDamageContext->targetState;
    auto& attackerState = srDamageEvent->srDamageContext->attackerState;
    u8 targetID = damageContext.targetID;
    u16 stealString = 49;
    u16 stolenItem = 0xFFFF;
    if (damageContext.targetEnemyIndex != -1) {
        auto& enemy = *targetState.enemyData;
        for (auto& item : enemy.toSteal) {
            if (item.alreadyStolen) {
                continue;
            }
            if (!(targetState.actorTimers->unkActorFlags & 1)) {
                u16 levelDelta = damageContext.attackerLevel + 40 - damageContext.targetLevel;
                if (attackerState.actorTimers->field_C == 4 && levelDelta < 100)
                    levelDelta = 100;
                u16 levelFactor = (levelDelta * 512) / 100;
                u16 stealChance = (item.stealChance * levelFactor) / 256;
                if ((gameGetRand() & 0x3F) < stealChance) {
                    stolenItem = item.itemID;
                    item.alreadyStolen = true;
                    break;
                }

            }
        }
        if (stolenItem == 0xFFFF) {
            stealString = 48;
        }
        else {
            srCreateEvent(0, damageContext.attackerID, 3, stolenItem);
            targetState.actorTimers->unkActorFlags |= 1u;
            stealString = 47;
        }
    }
    pushDisplayString(damageContext.attackerID, stealString, 1, (u32*)&stolenItem);
}


typedef u32(*SRPFN_GETINFLICTRAND)(u32);
#define gameGetInflictRand (SRPFN_GETINFLICTRAND(0x5C8BDC))
typedef u8 (*SRPFN_GAMEGETMASKIDX)(u16);
#define getFirstMaskActiveBit ((SRPFN_GAMEGETMASKIDX)0x5C8041)
void handleCover(DamageCalculationEvent* srDamageEvent) {
    auto& damageContext = *srDamageEvent->damageContext;
    auto& damageEvent = *srDamageEvent->gameDamageEvent;
    auto& aiContext = *srDamageEvent->aiContext;
    u8 originalTarget = damageEvent.targetID;
    u16* word_9A88AA = (u16*)(0x9A88AA);
    u16 someMask = 0;
    srLogWrite("Handling Action Cover");
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

    if (validCoverTarget && srDamageEvent->srDamageContext->damageType == PHYSICAL && !inflictingToad && validCoverCommand) {
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

            if (!(actorState.actorTimers->unkActorFlags & 8) && (1 << partyIdx) & someMask && !(aiContext.actorAIStates[partyIdx].statusMask & 0x86805C45)) {
                u8 coverChance = partyState.gamePartyMember->coverChance;
                srLogWrite("actor %i has coverChance %i", damageContext.attackerID, coverChance);
                if (coverChance > gameGetInflictRand(100))
                    candidateCoverers |= 1 << partyIdx;
            }
        }
        u16 coveringActorMask = getRandomMaskBit((aiContext.actorPartyMask & candidateCoverers));
        if (coveringActorMask) {
            damageEvent.targetID = getFirstMaskActiveBit(coveringActorMask);
            damageEvent.attackerID = originalTarget;
            aiContext.actorAIStates[damageEvent.targetID].lastCovered = aiContext.actorAIStates[originalTarget].characterID;
        }
    }
}

void setupGameDamageAndImpactEvents(DamageCalculationEvent* srDamageEvent) {
    auto& damageContext = *srDamageEvent->damageContext;
    auto& srDamageContext = *srDamageEvent->srDamageContext;
    auto& damageEvent = *srDamageEvent->gameDamageEvent;
    auto& aiContext = *srDamageEvent->aiContext;
    damageEvent.targetStatusMask = aiContext.actorAIStates[damageContext.targetID].statusMask;
    damageEvent.damagedAnimScriptIdx = damageContext.targetReactionAnimation;

    if (damageContext.abilityFlags1 & 2) {
        if (!wasStatusChanged(srDamageEvent)) {
            if (damageEvent.impactEventQueueIdx == -1) {
                createImpactEvent(
                    &damageEvent,
                    -1,
                    0,
                    -1,
                    -1,
                    aiContext.actorAIStates[damageContext.targetID].currentHP,
                    aiContext.actorAIStates[damageContext.targetID].currentMP);
            }
        }
    }
    else {
        createImpactEvent(
            &damageEvent,
            damageContext.killRecoverDamage,
            damageContext.abilityFlags2,
            damageContext.attackImpactSound,
            damageContext.attackImpactEffectID,
            aiContext.actorAIStates[damageContext.targetID].currentHP,
            aiContext.actorAIStates[damageContext.targetID].currentMP);
    }

    if (damageContext.abilityFlags1 & 0x20)
        damageEvent.damagedAnimScriptIdx = 9;
}

void handleHealEffects(DamageCalculationEvent* srDamageEvent) {
    auto& damageContext = *srDamageEvent->damageContext;
    if (!(damageContext.specialAbilityFlags & 0x10))
        calculateHealAmounts(srDamageEvent);
    if (damageContext.miscActionFlags & 0x80)
        calculateHealAmounts(srDamageEvent);
    if (damageContext.miscActionFlags & 0x40)
        calculateHealAmounts(srDamageEvent);
}

void calculateHealAmounts(DamageCalculationEvent* srDamageEvent) {
    auto& damageContext = *srDamageEvent->damageContext;
    bool healActive = (damageContext.abilityFlags2 & 1) != 0;
    auto& attackerTimers = *srDamageEvent->srDamageContext->attackerState.actorTimers;
    u16 currentDamage = damageContext.currentDamage;
    auto tempTarget = damageContext.currentUnitIDTempt;
    auto someFlags = ((damageContext.specialAbilityFlags & 0x20 != 0) ? 0 : 3) | (2 - ((damageContext.abilityFlags2 & 4) != 0));
    if (damageContext.miscActionFlags & 0x80 || someFlags & 1) {
        currentDamage /= 100;
        if (tempTarget == damageContext.targetID && damageContext.targeCurrentMP < currentDamage)
            currentDamage = damageContext.targeCurrentMP;
        if (healActive)
            currentDamage = -currentDamage;
        attackerTimers.drainedMP -= currentDamage;
    }
    if (damageContext.miscActionFlags & 0x40 || someFlags & 2) {
        currentDamage /= 10;
        if (tempTarget == damageContext.targetID && damageContext.targetCurrentHP < currentDamage)
            currentDamage = damageContext.targetCurrentHP;
        if (healActive)
            currentDamage = -currentDamage;
        attackerTimers.drainedHP -= currentDamage;
    }
}

void handleDeathImpactSetup(DamageCalculationEvent* srDamageEvent) {
    auto& damageContext = *srDamageEvent->damageContext;
    auto& damageEvent = *srDamageEvent->gameDamageEvent;
    auto& srDamageContext = *srDamageEvent->srDamageContext;
    auto& aiContext = *srDamageEvent->aiContext;
    auto& targetState = srDamageContext.targetState;
    if (aiContext.actorAIStates[damageContext.targetID].statusMask & 1) {
        damageEvent.specialDamageFlags |= 4;
        damageEvent.specialDamageFlags &= 0xFFF7u;
        damageContext.wasKilledMask |= 1 << damageContext.targetID;

        //Handle Flash
        if (damageContext.commandIndexCopy == 26) {
            if (targetState.party10->deathType < 17)
                targetState.party10->deathType = 8;
            createImpactEvent(&damageEvent, -2, 0, damageContext.attackImpactSound, damageContext.impactEffectID,
                aiContext.actorAIStates[damageContext.targetID].currentHP,
                aiContext.actorAIStates[damageContext.targetID].currentMP);
        }
    }
}

void setTargetContext(u8 targetID, DamageCalculationEvent* srDamageEvent) {
    auto& damageContext = *srDamageEvent->damageContext;

    damageContext.targetID = targetID;
    srDamageEvent->srDamageContext->targetState = gContext.battleActors.getActiveBattleActor(damageContext.targetID);
    auto& targetBattleVars = *srDamageEvent->srDamageContext->targetState.actorBattleVars;

    srDamageEvent->srDamageContext->targetState = gContext.battleActors.getActiveBattleActor(targetID);
    damageContext.targetEnemyIndex = -1;
    //damageContext.unkDword10 = 0;
    damageContext.abilityFlags2 = 0;
    damageContext.targetDefense = 0;
    damageContext.currentDamage = 0;
    damageContext.targetStateFlags = 0;
    damageContext.abilityFlags1 = damageContext.miscActionFlags;
    damageContext.finalHitRate = damageContext.abilityHitRate;
    damageContext.targetReactionAnimation = targetBattleVars.damageAnimID;
    damageContext.targetStatusMask = targetBattleVars.statusMask;
    damageContext.targetLevel = targetBattleVars.level;
    damageContext.targetCurrentHP = targetBattleVars.currentHP;
    damageContext.targeCurrentMP = targetBattleVars.currentMP;
    damageContext.vTimerBlock = srDamageEvent->srDamageContext->targetState.actorTimers;
    damageContext.party34Block = srDamageEvent->srDamageContext->targetState.party34;
    damageContext.attackImpactSound = -1;
    damageContext.attackImpactEffectID = -1;
    damageContext.killRecoverDamage = -1;
    if (!damageContext.abilityPower)
        damageContext.targetReactionAnimation = 51;

    else if (!damageContext.abilityPower) {
        setActionDidHit(srDamageEvent, false);
    }

    damageContext.elementalDefenseMask = 0;
    damageContext.currentDamage = 0;

    if (damageContext.specialAbilityFlags & 0x400) {
        /*if (damageContext->specialAbilityFlags & 4)
            damageContext->targetDefense = getStatWithModifiers(
                targetActorID,
                aiContext.actorAIStates[targetActorID].mdefense,
                3);
        else
            damageContext->targetDefense = getStatWithModifiers(
                targetActorID,
                aiContext.actorAIStates[targetActorID].defense,
                2);
    }
    if (damageContext->targetDefense > 512)
        damageContext->targetDefense = 512;*/
    }
    if (damageContext.targetID >= 4)
        damageContext.targetEnemyIndex = getInBattleFormationActorData(damageContext.targetID - 4)->enemyID;
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
        srLogWrite("PERFORMING INFLICTION CALCULATION for status: %s", infliction.statusName.c_str());
        if (didInflictionSucceed(infliction, srDamageEvent)) {
            if (infliction.doesToggle) {
                srLogWrite("adding status %s to toggle-list", infliction.statusName);
                toggle.push_back(infliction.statusName);
                continue;
            }
            if (infliction.doesRemove) {
                srLogWrite("adding status %s to remove-list", infliction.statusName);
                remove.push_back(infliction.statusName);
                continue;
            }
            srLogWrite("adding status %s to toggle-list", infliction.statusName);
            add.push_back(infliction.statusName);
        }
    }
    srDamageEvent->srDamageContext->toAddStatuses = add;
    srDamageEvent->srDamageContext->toRemoveStatuses = remove;
    srDamageEvent->srDamageContext->toToggleStatuses = toggle;
}

bool didInflictionSucceed(StatusInfliction infliction, DamageCalculationEvent* srDamageEvent) {
    auto damageContext = srDamageEvent->damageContext;
    u16* gEscapedActorsMask = (u16*)0x9AAD24;

    /*auto levelDelta = damageContext->attackerLevel - damageContext->targetLevel;
    auto magDelta = AI_BATTLE_STATE.actorAIStates[damageContext->attackerID].magAtk - AI_BATTLE_STATE.actorAIStates[damageContext->targetID].mdefense;*/
    srLogWrite("Handling status infliction for target %d", srDamageEvent->damageContext->targetID);
    const auto& inflictions = srDamageEvent->srDamageContext->statusToInflict;
    auto inflictChance = 4 * infliction.inflictionChance;
    srLogWrite("Infliction Chance: %i", inflictChance);
    if ((damageContext->targetStatusMask & 0x800) && std::find_if(
        inflictions.begin(), inflictions.end(), [&](StatusInfliction elmnt) {return elmnt.statusName == StatusNames::FROG;}) != inflictions.end())
        inflictChance = 252;
    if ((damageContext->targetStatusMask & 0x1000) && std::find_if(
        inflictions.begin(), inflictions.end(), [&](StatusInfliction elmnt) {return elmnt.statusName == StatusNames::SMALL;}) != inflictions.end())
        inflictChance = 252;
    /*if (damageContext->targetID < 3 && aRcBattleBattle & combinedMask)
        inflictChance = 252;*/

    if (inflictChance < 252) {
        inflictChance = damageContext->MPTurboBoost * inflictChance / 100 + inflictChance;
        if ((damageContext->abilityTargetingFlags & 0xC) != 4 && damageContext->targetCount > 1)
            inflictChance = 2 * inflictChance / 3;
        if (damageContext->quadCount)
            inflictChance >>= 1;
    }
    // apply target resistances for this status, reducing infliction chance
    const auto& status = gContext.statuses.getElement(infliction.statusName);
    auto res = srDamageEvent->srDamageContext->targetState.battleStats->at(status.resName).activeValue;
    auto pen = srDamageEvent->srDamageContext->attackerState.battleStats->at(status.penName).activeValue + srDamageEvent->srDamageContext->attackStats[status.penName].statValue;
    auto modifier = res - pen;
    inflictChance -= (modifier / 100) * inflictChance;
    // return no to death immunity in weird context
    auto isInflictingDeath = std::find_if(inflictions.begin(), inflictions.end(), [&](StatusInfliction elmnt) {return elmnt.statusName == StatusNames::DEATH; }) != inflictions.end();
    if (isInflictingDeath && (1 << damageContext->currentUnitIDTempt) & *gEscapedActorsMask) {
        srLogWrite("INFLICTION FAILED DUE TO TARGETING ESCAPED/DEATH");
        return false;
    }

    if (inflictChance >= gameGetInflictRand(100) + 1) {
        srLogWrite("INFLICTION SUCCESSFUL");
        return true;
    }
    srLogWrite("INFLICTION FAILED");
    return false;
}

void handleStatusInfliction(DamageCalculationEvent* srDamageEvent) {
    auto damageContext = srDamageEvent->damageContext;
    auto srDamageContext = srDamageEvent->srDamageContext;
    auto& aiContext = *srDamageEvent->aiContext;
    u8* byte_7B76C0 = (u8*)0x7B76C0;
    u8 tempTarget = damageContext->targetID;
    if (!checkActionDidHit(srDamageEvent)) {
        setStatusWasChanged(srDamageEvent, false);
        return;
    }

    auto& targetStatuses = *srDamageContext->targetState.activeStatuses;
    auto& addStatuses = srDamageContext->toAddStatuses;
    auto& removeStatuses = srDamageContext->toRemoveStatuses;
    auto& toggleStatuses = srDamageContext->toToggleStatuses;
    for (const auto& statusName : toggleStatuses) {
        auto wasFound = std::find_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {return statusName == activeStatus.statusName; }) != targetStatuses.end();
        if (wasFound) {
            removeStatuses.push_back(statusName);
            continue;
        }
        addStatuses.push_back(statusName);
    }

    std::unordered_set<std::string> srPreviousStatuses;
    for (auto activeStatus : targetStatuses) {
        srPreviousStatuses.insert(activeStatus.statusName);
    }

    u32 previousTargetStatus = 0;
    for (const auto& activeStatus : targetStatuses) {
        const auto& status = gContext.statuses.getElement(activeStatus.statusName);
        if (status.isGameStatus) {
            previousTargetStatus &= (1 << status.gameIndex);
        }
    }
    // add every status to be added
    for (auto statusName : addStatuses) {
        srLogWrite("ATTEMPTING TO INFLICT STATUS: %s", statusName.c_str());
        auto wasFound = std::find_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {return statusName == activeStatus.statusName; }) != targetStatuses.end();
        const auto& status = gContext.statuses.getElement(statusName);

        if (!wasFound || status.allowMultiple) {
            if (!status.removeOnInflict.empty()) {
                auto it = std::remove_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {
                    return std::find(status.removeOnInflict.begin(), status.removeOnInflict.end(), activeStatus.statusName) != status.removeOnInflict.end();
                    });
            }
            if (!status.neutralizeOnInflict.empty()) {
                auto it = std::remove_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {
                    return std::find(status.neutralizeOnInflict.begin(), status.neutralizeOnInflict.end(), activeStatus.statusName) != status.neutralizeOnInflict.end();
                    });
                if (it != targetStatuses.end()) {
                    continue;
                }
            }
            ActiveStatus activeStatus = ActiveStatus{ statusName };
            targetStatuses.push_back(activeStatus);
        }
    }
    for (auto& targetStatus : targetStatuses) {
        srLogWrite("target now has active status: %s", targetStatus.statusName.c_str());
    }
    //Remove all status to be removed
    targetStatuses.erase(
        std::remove_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {
            return std::find_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {
                return (std::find(removeStatuses.begin(), removeStatuses.end(), activeStatus.statusName) != removeStatuses.end()); }) != targetStatuses.end(); }), targetStatuses.end());

    //now that Active Statuses have been synced, set game statuses appropriately
    std::unordered_set<std::string> srCurrentStatuses;
    for (auto activeStatus : targetStatuses) {
        srCurrentStatuses.insert(activeStatus.statusName);
    }

    u32 newTargetStatus = 0;
    for (const auto& activeStatus : targetStatuses) {
        const auto& status = gContext.statuses.getElement(activeStatus.statusName);
        if (status.isGameStatus) {
            newTargetStatus |= (1 << status.gameIndex);
        }
    }
    srLogWrite("gameTargetMask: %x", newTargetStatus);
    aiContext.actorAIStates[tempTarget].statusMask = newTargetStatus;

    //If no status was inflicted
    
    if (srPreviousStatuses == srCurrentStatuses) {
        setStatusWasChanged(srDamageEvent, false);
    }
    else {
        if (damageContext->combinedStatusMask & newTargetStatus && damageContext->attackerID != tempTarget)
            damageContext->actionCounterable |= 1 << tempTarget;

        //Handle the case where death has been inflicted
        if ((newTargetStatus ^ previousTargetStatus) & 1) {
            if (previousTargetStatus & 1){
                if (damageContext->targetID >= 4)
                    damageContext->targetReactionAnimation = 57;
            }
            else if (damageContext->targetStatusMask & 0x400) {
                damageContext->targetReactionAnimation = 48;
            }
            else if (damageContext->targetStatusMask & 0x800) {
                damageContext->targetReactionAnimation = 5;
            }
            else {
                u16 defaultAnimScript;
                if (damageContext->targetID >= 3)
                    defaultAnimScript = aiContext.actorAIStates[damageContext->targetID].damageAnimID;
                else
                    defaultAnimScript = byte_7B76C0[damageContext->targetReactionAnimation];
                damageContext->targetReactionAnimation = defaultAnimScript;
            }
        }
        else {
            srDamageEvent->gameDamageEvent->specialDamageFlags |= 8;
        }
    }
}

typedef u16(*SRPFN_HANDLEDAMAGEORHEAL)(u32, u32, u32, bool);
#define handleDamageOrHeal (SRPFN_HANDLEDAMAGEORHEAL(0x5C8126))
void handleDamage(DamageCalculationEvent* srDamageEvent) {
    auto damageContext = srDamageEvent->damageContext;
    auto& aiContext = *srDamageEvent->aiContext;
    auto& actorState = aiContext.actorAIStates[damageContext->targetID];
    auto& target34 = *srDamageEvent->srDamageContext->targetState.party34;
    auto& attackerTimers = *srDamageEvent->srDamageContext->targetState.actorTimers;
    auto isHeal = damageContext->abilityFlags2 & 1;

    if (!damageContext->currentDamage)
        return;

    if (damageContext->abilityFlags2 & 4) {
        actorState.currentMP = handleDamageOrHeal(actorState.currentMP, damageContext->currentDamage, actorState.maxMP, isHeal);
        return;
    }

    u32 previousHP = actorState.currentHP;
    actorState.currentHP = handleDamageOrHeal(actorState.currentHP, damageContext->currentDamage, actorState.maxHP, isHeal);
    if (!actorState.currentHP) {
        if (previousHP) {
            //Handle Death inflicted
            actorState.statusMask |= 1u;
            if (damageContext->miscActionFlags & 0x2000)
                handleMorph(srDamageEvent);
        }
    }
    handleLimitFill(srDamageEvent);
}

void handleMorph(DamageCalculationEvent* srDamageEvent) {
    auto damageContext = srDamageEvent->damageContext;
    auto& srDamageContext = srDamageEvent->srDamageContext;
    auto& targetState = srDamageContext->targetState;
    auto& aiContext = *srDamageEvent->aiContext;
    auto& actorState = aiContext.actorAIStates[damageContext->targetID];
    auto& target34 = *srDamageEvent->srDamageContext->targetState.party34;
    auto& attackerTimers = *srDamageEvent->srDamageContext->targetState.actorTimers;
    auto isHeal = damageContext->abilityFlags2 & 1;

    if (damageContext->targetID >= 4 && !(targetState.actorTimers->unkActorFlags & 0x10)) {
        u16 morphItemID = srDamageContext->targetState.enemyData->morphItem;
        bool canMorph = srDamageContext->targetState.enemyData->canMorph;
        if (canMorph) {
            if (targetState.party10->deathType < 17)
                targetState.party10->deathType = 7;

            srCreateEvent(0, damageContext->attackerID, 3, morphItemID);
            pushDisplayString(damageContext->attackerID, 86, 1, (u32*)&morphItemID);
            targetState.actorTimers->unkActorFlags |= 0x10u;
        }
    }
}

void handleLimitFill(DamageCalculationEvent* srDamageEvent) {
    auto damageContext = srDamageEvent->damageContext;
    auto& aiContext = *srDamageEvent->aiContext;
    auto& actorState = aiContext.actorAIStates[damageContext->targetID];
    auto& target34 = *srDamageEvent->srDamageContext->targetState.party34;
    auto& attackerTimers = *srDamageEvent->srDamageContext->targetState.actorTimers;
    auto isHeal = damageContext->abilityFlags2 & 1;

    if (damageContext->targetID < 3 && damageContext->attackerID >= 4 && !isHeal && (target34.limitLevelIdx < 4) && !(attackerTimers.unkActorFlags & 8)) {
        auto limitFill = 256;
        srLogWrite("incrementing limit break fill for party member %d", damageContext->targetID);
        if (damageContext->targetStatusMask & 0x20) {
            limitFill = 512;
        }
        else if (damageContext->targetStatusMask & 0x10) {
            limitFill = 128;
        }
        u16 limitIncrement = limitFill * ((300 * damageContext->currentDamage) / actorState.maxHP);
        limitIncrement /= target34.limitLevelDivisor;
        u16 newLimitValue = target34.limitBar + limitIncrement;
        if (newLimitValue > 255)
            newLimitValue = 255;
        target34.limitBar = newLimitValue;
    }
}

typedef void(*SRPFN_GAMESETREACTIONANIM)(bool);
#define gameSetHitReaction (SRPFN_GAMESETREACTIONANIM(0x5DB8D6))
void handleDamageReaction(DamageCalculationEvent* srDamageEvent) {
    auto damageContext = srDamageEvent->damageContext;
    if (damageContext->abilityFlags1 & 3) {
        if (!checkActionDidHit(srDamageEvent)) {
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
    auto& damageContext = srDamageEvent->damageContext;
    auto& target34 = *srDamageEvent->srDamageContext->targetState.party34;
    auto& targetBattleVars = *srDamageEvent->srDamageContext->targetState.actorBattleVars;
    auto& attackerTimers = *srDamageEvent->srDamageContext->attackerState.actorTimers;

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

    if (srDamageEvent->srDamageContext->damageType == PHYSICAL) {
        if (targetBattleVars.stateFlags & 0x200)
            damageContext->currentDamage = 0;
    }
    else if (srDamageEvent->srDamageContext->damageType == MAGICAL) {
        damageContext->currentDamage = 0;
    }

    if (damageContext->currentDamage && attackerTimers.currentHP == 7777)
        damageContext->currentDamage = 7777;
}


typedef bool(*SRPFN_GETREFLECTTYPE)(u32, u32);
#define isCrossRowReflect (SRPFN_GETREFLECTTYPE(0x5DB166))
typedef bool (*SRPFN_MAKEREFLECTMASK)(u8);
#define makeReflectMask    ((SRPFN_MAKEREFLECTMASK)0x5DB199)
void applyReflect(DamageCalculationEvent* srDamageEvent) {
    auto damageContext = srDamageEvent->damageContext;
    auto& aiContext = *srDamageEvent->aiContext;
    u16* G_ATTACKER_TARGETS = (u16*)0xC3F340;
    u8* reflectTargets = (u8*)(0xC3F358);
    u32* dword_C3F364 = (u32*)(0xC3F364);
    u16* word_C3F37C = (u16*)(0xC3F37C);
    const auto& targetState = srDamageEvent->srDamageContext->targetState;
    const auto& targetStatuses = *targetState.activeStatuses;
    bool reflectIsActive = std::find_if(targetStatuses.begin(), targetStatuses.end(), [&](const ActiveStatus& activeStatus) {return activeStatus.statusName == StatusNames::REFLECT; }) != targetStatuses.end();
    bool isReflectable = !(damageContext->specialAbilityFlags & 0x200) && !((1 << damageContext->targetID) /*& word_C3F37C[0]*/) && reflectIsActive;
    if (isReflectable) {
        srLogWrite("REFLECT TRIGGERED");
        u8 reflectTarget;
        //For party actors, just reflect to attacker
        if (isCrossRowReflect(damageContext->attackerID, damageContext->targetID)) {
            reflectTarget = damageContext->attackerID;
        }
        else {
            if (reflectTargets[damageContext->targetID] == -1) {
                u16 reflectMask = makeReflectMask(damageContext->targetID);
                reflectTargets[damageContext->targetID] = getFirstMaskActiveBit(reflectMask);
            }
            reflectTarget = reflectTargets[damageContext->targetID];
        }
        *dword_C3F364 |= 2u;
        G_ATTACKER_TARGETS[damageContext->targetID] |= 1 << reflectTarget;
        nullifyAction(srDamageEvent);
        if (targetState.actorTimers->innateStatusMask & 0x40000) {
            *word_C3F37C |= 1 << damageContext->targetID; //Handle the case where it is auto-reflect, as inflicted by the reflect ring
        }
        if (targetState.actorTimers->reflectCount) {
            targetState.actorTimers->reflectCount--;
        }
        else {
            srDamageEvent->srDamageContext->toRemoveStatuses.push_back(StatusNames::REFLECT);
        }
        damageContext->abilityFlags1 |= 2u;
        if (damageContext->targetID < 3)
            damageContext->targetReactionAnimation = 10;
        srDamageEvent->gameDamageEvent->specialDamageFlags |= 2; //Display reflect flag
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

    auto& targetStats = *targetState.battleStats;
    auto& attackerStats = *attackerState.battleStats;
    for (const auto& name : setupEvent->srDamageContext->attackElements) {
        const auto& element = gContext.elements.getElement(name);
        if (std::find(srDamageContext.attackElements.begin(), srDamageContext.attackElements.end(), name) != srDamageContext.attackElements.end()) {
            auto res = targetStats[element.resName];
            if (res.activeValue == 255) {
                aiContext.actorAIStates[damageContext.targetID].currentHP = aiContext.actorAIStates[damageContext.targetID].maxHP;
                aiContext.actorAIStates[damageContext.targetID].currentMP = aiContext.actorAIStates[damageContext.targetID].maxMP;
                damageContext.abilityFlags2 = 1;
                damageContext.abilityFlags1 &= 0xFFFFFFFD;
                srDamageContext.toAddStatuses.erase(std::remove(srDamageContext.toAddStatuses.begin(), srDamageContext.toAddStatuses.end(), StatusNames::DEATH));
                damageContext.killRecoverDamage = -3;
            }
            if (res.activeValue == -255) {
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
            auto pen = attackerStats[element.penName];
            auto eRes = res.activeValue - pen.activeValue;
            auto aff = attackerStats[element.affName];
            if (eRes > 100) {
                damageContext.abilityFlags2 ^= 1u;
            }
            elementalDamageModifier = elementalDamageModifier - (eRes / 100);
            elementalDamageModifier = (aff.activeValue / 100) + elementalDamageModifier;
            if (name == ElementNames::RESTORE) {
                damageContext.abilityFlags2 |= 1u;
            }
        }
    }
    damageContext.currentDamage = damageContext.currentDamage * elementalDamageModifier;
}
