#include "command_sr_callbacks.h"
#include "../battle/scene_globals.h"
#include "../battle/engine/damage_events.h"
#include "../impl.h"

void srLoadAbilityData() {
    CommandSetupEvent srEvent = { gDamageContextPtr };
    srSetupAction(srEvent);
}

typedef void(*PFNSR_VOIDSUB)();
#define handleCantReach ((PFNSR_VOIDSUB)0x5DA278)
void srApplyDamage(CommandSetupEvent setupEvent) {
    auto damageContext = setupEvent.damageContext;
    auto& aiContext = *AI_BATTLE_CONTEXT;
    u32* dword_C3F364 = (u32*)dword_C3F364;
    u16* word_C3F37C = (u16*)0xC3F37C;
    u16* G_ATTACKER_TARGETS = (u16*)0xC3F340;
    u16* G_TEMP_ATTACKER_TARGETS = (u16*)0xC3F368;
    u8* byte_C3F358 = (u8*)0xC3F358;

    *dword_C3F364 = 0;
    u32 localActionCount;
    if (!(damageContext->animationScriptID != -1))
        return;

    if (!(damageContext->displayString == -1)) {
        handleFailedAction(damageContext->displayString);
        return;
    }
    if (!(damageContext->commandIndexCopy > 32 || !sub_5DBE6F()))
        return;

    if (damageContext->supportMatFlags)
        setSupportMateriaFlags(damageContext->supportMatFlags);
    damageContext->usedTargetMask = damageContext->targetMask;
    damageContext->abilityPowerCopy = damageContext->abilityPower;

    // Execute all follow up actions
    while (1) {
        localActionCount = damageContext->actionCount--;// do this doublecut param times
        if (!localActionCount)
            break;

        for (u32 followUpIdx = 0; followUpIdx < 8; ++followUpIdx) {
            if (damageContext->followUpActions[followUpIdx] != 255) {
                auto followUpID = damageContext->followUpActions[followUpIdx];
                damageContext->followUpActions[followUpIdx] = -1;
                damageContext->absAttackIndex = followUpID;
                damageContext->sceneActionIndex = damageContext->absAttackIndex;
                damageContext->abilityTargetingFlags = 0xFFF;
                srLoadAbilityData(setupEvent);
                //some special flag setting, investigate and replace bit packing trick
                /*if (followUpID & 0x80) {
                    damageContext->specialAbilityFlags &=0xDFu;
                }*/
                break;
            }
        }
        word_C3F37C[0] = 0;
        damageContext->targetMask = damageContext->usedTargetMask;
        damageContext->hitCountCopy = damageContext->attackHitCount;
        createFinalTargetMask();
        handleCantReach();
        if (damageContext->miscActionFlags & 0x80000) {
            for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
                if ((1 << actorIdx) & damageContext->targetMask) {
                    if (aiContext.actorAIStates[actorIdx].statusMask & 1)
                        damageContext->targetMask &= ~(1 << actorIdx); // unflag dead targets
                }
            }
            if (!damageContext->targetMask) {
                pushActionAnimationEvent(damageContext->animationScriptID);
                break;
            }
        }
        if (!damageContext->targetMask) {
            handleFailedCast(damageContext->displayString);
            break;
        }
        // Why do this lol?
        for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
            G_TEMP_ATTACKER_TARGETS[actorIdx] = 0;
            byte_C3F358[actorIdx] = -1;
        }
        setDamageFormula();
        G_TEMP_ATTACKER_TARGETS[damageContext->attackerID] = damageContext->targetMask;
        *dword_C3F364 |= 9u;
        while (*dword_C3F364 & 1) {
            if (*dword_C3F364 & 6) {
                *dword_C3F364 &= 0xFFFFFFFD;
                if (damageContext->commandIndexCopy != 3)
                    damageContext->activeAllies = 3;
            }
            // I doubt this needs to be done this way
            for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
                G_ATTACKER_TARGETS[actorIdx] = G_TEMP_ATTACKER_TARGETS[actorIdx];
                G_TEMP_ATTACKER_TARGETS[actorIdx] = 0;
            }
            for (u8 attackerActorID = 0; attackerActorID < 10; ++attackerActorID) {
                auto targetMask = G_ATTACKER_TARGETS[attackerActorID];
                if (targetMask) {
                    damageContext->currentUnitIDTempt = attackerActorID;
                    damageContext->incOnDamageDealt = 0;
                    damageContext->attackHitCount = damageContext->hitCountCopy;
                    sub_436C30();
                    createActionAnimationEvent(targetMask);
                    do {
                        bool shouldRandomTarget = false;
                        damageContext->targetMask = targetMask;
                        if (damageContext->attackHitCount)
                            shouldRandomTarget = true;
                        if (damageContext->quadCount && (damageContext->targetingFlags & 0xC) != 4)
                            shouldRandomTarget = true;
                        if (damageContext->targetingFlags & 0x80)
                            shouldRandomTarget = true;
                        if (damageContext->commandIndexCopy == 3)
                            shouldRandomTarget = true;

                        if (shouldRandomTarget)
                            damageContext->targetMask = getRandomTarget(damageContext->targetMask);
                        for (u8 targetActorID = 0; targetActorID < 10; ++targetActorID) {
                            if ((1 << targetActorID) & damageContext->targetMask)// calculates main damage for each target
                                calculateDamage(attackerActorID, targetActorID);
                        }
                        if (damageContext->attackHitCount)
                            --damageContext->attackHitCount;

                    } while (damageContext->attackHitCount);// for every attackHitCount
                    unTarget();
                    handleAdditionalEffects(5);
                }
            }
            for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
                if (gActorTimerBlock[actorIdx].field_2C)
                    sub_5DA380(actorIdx, 0);
                if (gActorTimerBlock[actorIdx].field_30)
                    sub_5DA380(actorIdx, 1);
            }
            // This block handles quadra magic
            if (!(*dword_C3F364 & 2)) {
                if (damageContext->quadCount)
                    --damageContext->quadCount;
                if (damageContext->quadCount) {
                    u16 targetMask = damageContext->targetMask;
                    if (damageContext->commandIndexCopy == 3)
                        targetMask = word_9A8894 & 0x3F0;
                    if (targetMask) {
                        for (u8 i = 0; i < 10; ++i) {
                            if (aiContext.actorAIStates[i].statusMask & 1)
                                targetMask &= ~(1 << i);
                        }
                        G_TEMP_ATTACKER_TARGETS[damageContext->attackerID] = targetMask;
                        if (!targetMask)
                            damageContext->quadCount = 0;
                    }
                }
                if (damageContext->quadCount) {
                    *dword_C3F364 |= 4;
                }
                else {
                    *dword_C3F364 &= 0xFFFFFFFE;
                }
            }
            // End Quadra Magic

            if (damageContext->miscActionFlags & 0x1000)
                damageContext->cameraDataSingle = -4;
            else
                damageContext->cameraDataSingle = -1;
            damageContext->cameraDataMultiple = damageContext->cameraDataSingle;
        }
        //Handles updating animation scripts, should be command level perhaps
        switch (damageContext->animationScriptID) {
        case 0x18:
            damageContext->animationScriptID = 25;
            break;
        case 0x19:
            damageContext->animationScriptID = 26;
            break;
        case 0x1A:
            damageContext->animationScriptID = 27;
            break;
        case 0x16:
            damageContext->animationScriptID = 23;
            damageContext->usedTargetMask = damageContext->targetMask;
            break;
        }
    }
    if (damageContext->miscActionFlags & 0x10000)
        pushActorAnimationEvent(71);
    if (damageContext->unkDword8 != -1)
        sub_5C7D99(damageContext->unkDword8);
 }

 void calculateDamage(DamageCalculationEvent* srDamageEvent, u8 attackerID, u8 targetID) {
     auto damageContext = srDamageEvent->damageContext;
     auto& aiContext = *srDamageEvent->aiContext;
     auto& damageEvent = *newDamageEvent();
     damageEvent.targetID = targetID;
     damageEvent.attackerID = attackerID;
     damageEvent.specialDamageFlags = 0;
     aiContext.actorAIStates[targetID].lastCovered = -1;
     handleCover(damageEvent);

     setTargetContext(damageEvent->targetID, srDamageEvent);
     attemptStatusInfliction(srDamageEvent);
     if (damageEvent->targetID != targetActorID) {
         sub_436E92();
         damageContext->abilityFlags1 |= 0x20;
     }
     u8 targetID = damageContext->targetID;
     //setElementalDamageContext();
     //setElementalFlags();
     if (damageContext->attackElementsMask & 0x200)
         damageContext->abilityFlags2 |= 1u;
     if (!(damageContext->specialAbilityFlags & 1))
         damageContext->abilityFlags2 |= 4;
     if (aiContext.actorAIStates[targetID].actorFlags & 0x4000)
         damageContext->abilityFlags1 |= 1u;
     if (!(damageContext->abilityFlags1 & 1))
         runFormulas();

     handleAdditionalEffects(3);
     if (!damageContext->abilityPower)
         damageContext->abilityFlags1 |= 2u;
     if (sub_5DC08E(targetID))
         damageContext->elementalDefenseMask = 32;
     applyElementalModifiers();
     bool isReflectable = !(damageContext->specialAbilityFlags & 0x200) && !((1 << targetID) & word_C3F37C[0]) && damageContext->targetStatusMask & 0x40000;
     if (!(damageContext->specialAbilityFlags & 0x100) && !isReflectable && !(damageContext->targetStatusMask & 1) && !(damageContext->elementalDefenseMask & 0xC1)) {
         damageContext->abilityFlags1 |= 1u;
     }
     if (damageContext->abilityFlags1 & 1) {
         nullDamageAndInflictions();
     }
     else {
         ++damageContext->incOnDamageDealt;
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
             damageEvent.specialDamageFlags |= 0x10 ;
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
     if (damageContext->abilityFlags1 & 2) {
         if ((unsigned int)&unk_800000 & damageContext->abilityFlags1)
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

     if (aiContext.actorAIStates[targetID].statusMask & 1) {
         damageEvent.specialDamageFlags |= 4;
         damageEvent.specialDamageFlags &= 0xFFF7u;
         damageContext->wasKilledMask |= 1 << targetID;
         if (damageContext->commandIndexCopy == 26) {
             if (gUnkBattleStructArray[targetID].deathType < 17)
                 gUnkBattleStructArray[targetID].deathType = 8;
             initializeImpactEvent(damageEvent, -2, 0, damageContext->attackImpactSound, damageContext->impactEffectID);
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
         v1 = (UnkActorStateStruct*)-1;
     else
         v1 = &unkActorArray[targetActorID];
     damageContext->party34Block = v1;
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
     auto inflictChance = damageContext->inflictStatusChance;
     auto levelDelta = damageContext->attackerLevel - damageContext->targetLevel;
     auto magDelta = AI_BATTLE_STATE.actorAIStates[damageContext->attackerID].magAtk
         - AI_BATTLE_STATE.actorAIStates[damageContext->targetID].mdefense;
     combinedMask = damageContext->toggleStatusMask | damageContext->rmStatusMask | damageContext->addStatusMask;
     didInflict = 1;
     damageContext->attackerLevel;
     damageContext->targetLevel;
     if ((damageContext->targetStatusMask & 0x800) == combinedMask)
         inflictChance = 252;
     if ((damageContext->targetStatusMask & 0x1000) == combinedMask)
         inflictChance = 252;
     if (damageContext->targetID < 3 && (unsigned int)aRcBattleBattle & combinedMask)
         inflictChance = 252;
     if (inflictChance < 252)
     {
         inflictChancea = damageContext->MPTurboBoost * inflictChance / 100 + inflictChance;
         if ((damageContext->targetingFlags & 0xC) != 4 && damageContext->targetCount > 1)
             inflictChancea = 2 * inflictChancea / 3;
         if (damageContext->quadCount)
             inflictChancea >>= 1;
         if ((unsigned __int8)getRandomNumber_0(100) + 1 >= inflictChancea)
             didInflict = 0;
     }
     if (combinedMask & 1 && (1 << damageContext->targetID) & (unsigned __int16)gEscapedActorsMask)
         didInflict = 0;
     return didInflict;
 }

 void handleStatusInfliction(CommandSetupEvent* setupEvent, auto& damageEvent) {
     auto damageContext = setupEvent.damageContext;
     auto srDamageContext = setupEvent->srDamageContext;
     auto& aiContext = *setupEvent.aiContext;
     u8 targetID = damageContext->targetID;
     if (damageContext->abilityFlags1 & 1) {
         damageContext->abilityFlags1 |= 0x800000;
         return;
     }

     auto& targetStatuses = *srDamageContext->targetState->activeStatuses;
     auto& addStatuses = srDamageContext->toAddStatuses;
     auto& removeStatuses = srDamageContext->toRemoveStatuses;
     auto& toggleStatuses = srDamageContext->toToggleStatuses;

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


     for (auto statusName : toggleStatuses) {
         auto wasFound = std::find_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {return statusName == activeStatus.statusName}) != targetStatuses.end();
         const auto& status = gContext.statuses.getElement(statusName);
         if (!wasFound || status.allowMultiple) {
             ActiveStatus activeStatus = ActiveStatus{ statusName };
             targetStatuses.push_back(activeStatus);
         }
     }
     targetStatuses.erase(
         std::remove_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {
             return std::find_if(targetStatuses.begin(), targetStatuses.end(), [&](ActiveStatus activeStatus) {
                 return (std::find(toggleStatuses.begin(), toggleStatuses.end(), activeStatus.statusName) != toggleStatuses.end())}) != targetStatuses.end()}));

     //now that Active Statuses have been synced, set game statuses appropriately

     u32 targetSusceptibleMask = ~damageContext->targetStatusImmuneMask;
     u32 targetStatusMask = damageContext->targetStatusMask;
     // Handles incopmatible statuses haste/slow and sadness/fury

     inflictConflictingStatus(setupEvent, STS_HASTE, STS_SLOW);
     inflictConflictingStatus(setupEvent, STS_FURY, STS_SADNESS);

     if (damageContext->killRecoverDamage == -2)
         targetSusceptibleMask |= 1u;
     u32 removedMask = ~(targetSusceptibleMask & damageContext->rmStatusMaskCopy);
     u32 inflictedMask = targetSusceptibleMask & damageContext->addStatusMaskCopy | targetStatusMask;
     u32 newTargetStatus = targetSusceptibleMask & damageContext->changeStatusMaskCopy ^ removedMask & inflictedMask;
     damageContext->targetStatusMask = newTargetStatus;
     aiContext.actorAIStates[targetID].statusMasks = newTargetStatus;

     if (targetStatusMask == newTargetStatus) {
         damageContext->abilityFlags1 |= 0x800000;
     }
     else {
         if (damageContext->combinedStatusMask & newTargetStatus && damageContext->attackerID != targetID)
             damageContext->actionCounterable |= 1 << targetID;
         if ((newTargetStatus ^ targetStatusMask) & 1) {
             gameSetHitReaction(targetStatusMask & 1);
         }
         else {
             damageEvent.specialDamageFlags |= 8;
         }
     }
 }

 void inflictConflictingStatus(CommandSetupEvent setupEvent, u32 statMask1, u32 statMask2) {
     auto damageContext = setupEvent.damageContext;
     if (damageContext->addStatusMaskCopy & statMask1) {
         damageContext->addStatusMaskCopy &= ~statMask1;
         damageContext->addStatusMaskCopy &= ~statMask2;
         damageContext->rmStatusMaskCopy &= statMask2;
     }
     if (damageContext->addStatusMaskCopy & statMask1) {
         damageContext->addStatusMaskCopy &= ~statMask1;
         damageContext->addStatusMaskCopy &= ~statMask2;
         damageContext->rmStatusMaskCopy &= statMask2;
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
         auto damageCap = mpDamageCap;
     else
         auto damageCap = hpDamageCap;
     if (damageContext->currentDamage > damageCap)
         damageContext->currentDamage = damageCap;

     if (damageContext->specialAbilityFlags & 4){
         if (targetBattleVars.actorFlags & 0x200)
             damageContext->currentDamage = 0;
     }
     else if (targetBattleVars.actorFlags & 0x100){
         damageContext->currentDamage = 0;
     }

     if (damageContext->currentDamage && attackerTimers.currentHP == 7777)
         damageContext->currentDamage = 7777;
 }
 void applyReflect(DamageCalculationEvent* srDamageEvent) {
     auto damageContext = srDamageEvent->damageContext;
     auto& aiContext = *srDamageEvent->aiContext;
     u16* G_ATTACKER_TARGETS = (u16*)0xC3F340;
     applyElementalModifiers();
     u8* reflectTargets = (u8*)(0xC3F358);
     bool isReflectable = !(damageContext->specialAbilityFlags & 0x200) && !((1 << damageContext->targetID) & word_C3F37C[0]) && damageContext->targetStatusMask & 0x40000;
     if (isReflectable) {
         u8 reflectTarget;
         //For party actors, just reflect to attacker
         if (sub_5DB166(damageContext->attackerID, damageContext->targetID)) {
             reflectTarget = damageContext->attackerID;
         }
         else {
             //get random reflect target
             if (reflectTargets[damageContext->targetID] == -1) {
                 u16 reflectMask = sub_5DB199(damageContext->targetID);
                 reflectTargets[damageContext->targetID] = sub_5C8041(reflectMask);
             }
             reflectTarget = reflectTargets[damageContext->targetID];
         }
         dword_C3F364 |= 2u;
         G_ATTACKER_TARGETS[damageContext->targetID] |= 1 << reflectTarget;
         nullDamageAndInflictions();
         if (srDamageEvent->targetState->actorTimers->field_34 & 0x40000) {
             word_C3F37C[0] |= 1 << damageContext->targetID;
         }
         else if (srDamageEvent->targetState->actorTimers->reflectCount) {
             --srDamageEvent->targetState->actorTimers->reflectCount;
         }
         else {
             damageContext->rmStatusMaskCopy |= 0x40000u;
         }
         damageContext->abilityFlags1 |= 2u;
         if (damageContext->targetID < 3)
             damageContext->targetReactionAnimation = 10;
     }
 }
 // Much fuller elemental logic, supporting resistances and multiple elements
 void applyElementalModifiers(DamageCalculationEvent* setupEvent) {
     auto& damageContext = *setupEvent->damageContext;
     auto& attackerState = *setupEvent->attackerState;
     auto& targetState = *setupEvent->attackerState;

     float damageModifier = 1;
     for (const auto& element: gContext.elements) {
         if (damageContext.attackElementsMask & 1 << element.gameIndex) {
             auto res = targetState.battleStats[element.resName];
             auto aff = attackerState.battleStats[element.affName];
             damageModifier = damageModifier - (res / 100);
             damageModifier = (aff / 100) + damageModifier;
         }
     }
     damageContext.currentDamage = damageContext.currentDamage * damageModifier;
 }

void srSetupAction(CommandSetupEvent setupEvent) {
    i32 sceneAbilityIndex;
    i32 enemyActionIndex;
    u16 elementMask;
    u8 characterID;

    auto deathSentenceFlag = 0;
    u16 cameraOverrideData = 0xFFFF;
    auto damageContext = setupEvent.damageContext;
    const SrAttack& action = getCommandAction(damageContext->commandIndex, damageContext->relAttackIndex);
    const AttackData& abilityData = action.attackData;
    BattleQueueEntry executingAction = (*(BattleQueueEntry*)0x9A9884);
    AttackData* currentSceneAbilities = (AttackData*)(0x9A90C4);
    char* attackNamesPtr = (char*)(0x9A9484);
    u16* currentSceneAbilityIDs = (u16*)(0x9A9444);

    damageContext->MPCost = -1;
    switch (damageContext->commandIndexCopy) {
        case 20: {
            if (damageContext->attackerID < 3)
                initializeLimitContext(damageContext);
            break;
        }
        case 32: {
            currentSceneAbilities[0] = abilityData;
            *currentSceneAbilityIDs = action.attackID;
            damageContext->sceneAbilityIndex = action.attackID;
            break;
        }
        default: {
            break;
        }
    }
    if (damageContext->commandIndex == CMD_MAGIC && damageContext->relAttackIndex == 54) //death sentence case hardcoded
        deathSentenceFlag = 1;

    if (!((damageContext->miscActionFlags) & 0x400000)) {
        EnabledSpell* spellData = getSpellSlot(damageContext->attackerID, damageContext->commandIndexCopy, damageContext->relAttackIndex);
        if (spellData)
            updatePlayerSpellData(damageContext, spellData, abilityData);
    }

    if (deathSentenceFlag) {
        damageContext->animationScriptID = 52;
    }
    else if (damageContext->attackerID >= 4) {
        const auto& actingEnemyRecord = *getInBattleActorEnemyData(damageContext->attackerID - 4); //Finds the "formation data" for the current actor
        for (enemyActionIndex = 0; enemyActionIndex < 16; ++enemyActionIndex) {
            if (damageContext->absAttackIndex == actingEnemyRecord.attackSceneIDs[enemyActionIndex]) { //offset into the actual actor IDs
                damageContext->animationScriptID = actingEnemyRecord.attackAnimScripts[enemyActionIndex]; //offset into the animation script indexes for this model
                cameraOverrideData = actingEnemyRecord.attackCameraIDs[enemyActionIndex];
                break;
            }
        }
    }

    if (damageContext->MPCost = -1)
        damageContext->MPCost = abilityData.MPCost;

    if (executingAction.entryPriority == 3 || damageContext->miscActionFlags & 0x400000) //This is triggered by mime
        damageContext->MPCost = 0;

    damageContext->abilityHitRate = abilityData.abilityHitRate;
    damageContext->damageFormulaID = abilityData.damageFormula;

    if (abilityData.elementMask == 0xFFFF)
        elementMask = 0;
    else
        elementMask = abilityData.elementMask;

    damageContext->attackElementsMask = elementMask;
    damageContext->abilityPower = abilityData.attackPower;
    damageContext->targetReactionAnimation = abilityData.targetReactionID;

    damageContext->abilityTargetingFlags = abilityData.targetingFlags;

    if (damageContext->commandIndexCopy == CMD_LIMIT)
        cameraOverrideData = (((*(u16*)0x9AB0CC) & 8) != 0) ? abilityData.cameraMovementMultiple : abilityData.cameraMovementSingle;

    if (cameraOverrideData == 0xFFFF) {
        damageContext->cameraDataSingle = abilityData.cameraMovementSingle;
        damageContext->cameraDataMultiple = abilityData.cameraMovementMultiple;
    }
    else {
        damageContext->cameraDataSingle = cameraOverrideData;
        damageContext->cameraDataMultiple = cameraOverrideData;
    }

    if (abilityData.impactEffectID != 255)
        damageContext->impactEffectID = abilityData.impactEffectID;
    damageContext->animationEffectID = abilityData.animationEffectID;
    damageContext->specialAbilityFlags = abilityData.specialAttackFlags;
    damageContext->impactSound = abilityData.impactSoundID;
    damageContext->critAtkSound = abilityData.impactSoundID;
    damageContext->missAtkSound = abilityData.impactSoundID;

    if (!((damageContext->specialAbilityFlags) & 4) && gAiActorVariables[damageContext->attackerID].statusMask & 0x4000000)
        damageContext->abilityHitRate >>= 1; //(half hit chance)
    setStatusInflictionData(damageContext, abilityData.statusInflictType, abilityData.statusMask);

    if (deathSentenceFlag)
        gAiActorVariables[damageContext->attackerID].statusMask &= 0xFFDFFFFF;
    copyAdditionalEffects(abilityData.additionalEffect, abilityData.additionalEffectModifier);
}

//Kill this during Limit Break reimplementation
void initializeLimitContext(DamageCalcStruct* damageContext) {
    u8 characterID;
    u8* activeLimitIDs;
    u8* kernelLimitScriptIndexes = (u8*)(0x7B76A0);
    u8* unkPartyStructPtr = (u8*)(0x9A87F4);

    characterID = unkPartyStructPtr[16 * damageContext->attackerID];
    activeLimitIDs = getGamePartyMember(damageContext->attackerID)->enabledLimitBytes;
    if (damageContext->absAttackIndex >= 96 && damageContext->absAttackIndex <= 128) {
        auto scriptIndex = damageContext->absAttackIndex - 96;
        if (kernelLimitScriptIndexes[scriptIndex] != 255)
            damageContext->animationScriptID = kernelLimitScriptIndexes[scriptIndex];
        damageContext->enabledMagicsIndex = -1;
        return;
    }
    for (i32 limitIndex = 0; limitIndex < 3; ++limitIndex) {
        auto relativeLimitIndex = (i8)activeLimitIDs[limitIndex];
        if (relativeLimitIndex + 128 == damageContext->absAttackIndex) {
            auto limitAnimationScriptIndex = 0;
            for (auto charLimitArrayIndex = 0; charLimitArrayIndex < 12; ++charLimitArrayIndex) {
                auto fetchedRelativeIndex = getLimitRelativeIndex(characterID, charLimitArrayIndex);
                if (fetchedRelativeIndex != 127) {
                    if (fetchedRelativeIndex == relativeLimitIndex)
                        break;
                    ++limitAnimationScriptIndex;
                }
            }
            damageContext->animationScriptID = limitAnimationScriptIndex + 60;
            break;
        }
    }
}

void updatePlayerSpellData(DamageCalcStruct* damageContext, EnabledSpell* spellData, const AttackData& abilityData) {
    BattleQueueEntry executingAction = (*(BattleQueueEntry*)0x9A9884);

    damageContext->MPCost = spellData->mpCost;
    if ((executingAction.entryPriority >= 5) && !(damageContext->miscActionFlags & 0x400000)) //priority 5 and 6 actions? what are those
        damageContext->supportMatFlags = spellData->supportEffectsMask;
    srLogWrite("Setting MP Cost for player spell to: %i", damageContext->MPCost);
    if (spellData->quadCount && spellData->quadEnabled) {
        spellData->quadCount = spellData->quadCount - 1;
        damageContext->quadEnabled = spellData->quadEnabled;// quadEnabled?
        damageContext->quadCount = damageContext->quadEnabled + 3;  // numberOfCasts?
        if (damageContext->quadCount > 8) {
            damageContext->quadCount = 8;
        }
        srCreateEvent(2, damageContext->attackerID, 21, 6);
    }
    else if (damageContext->commandIndexCopy == CMD_SUMMON) {
        if (spellData->allCount) { //if yo ucan still use the summon
            if (spellData->allCount != 255) {
                spellData->allCount = spellData->allCount - 1;
                srCreateEvent(2, damageContext->attackerID, 21, 4);
            }
        }
        else {
            damageContext->displayString = 121; //Probably displays "Summon Power is all used up"
        }
    }
    else if (damageContext->miscActionFlags & 0x200) { //Handle the all case
        if (spellData->allCount) {
            if (spellData->allCount != 255) {
                spellData->allCount = spellData->allCount - 1;
                srCreateEvent(2, damageContext->attackerID, 21, 2);
            }
        }
        else if (abilityData.targetingFlags & 8) {
            damageContext->miscActionFlags |= 0x100000u;
        }
    }
}

void setStatusInflictionData(DamageCalcStruct* damageContext, i32 statusInflictionByte, i32 inflictedStatusMask) {
    u16* word_9AAD1E = (u16*)(0x9AAD1E);
    damageContext->addStatusMask = 0;
    damageContext->rmStatusMask = 0;
    damageContext->toggleStatusMask = 0;
    auto statusType = statusInflictionByte >> 6;
    if (statusType < 3) {
        if (inflictedStatusMask >= 0) {
            damageContext->inflictStatusChance = 4 * (statusInflictionByte & 0x3F);// statusInflictionChance
            switch (statusType) {
                case 0:
                    damageContext->addStatusMask = inflictedStatusMask;
                    break;
                case 1:
                    damageContext->rmStatusMask = inflictedStatusMask;
                    break;
                case 2:
                    damageContext->toggleStatusMask = inflictedStatusMask;
                    break;
                default: {
                    break;
                }
            }
        }
        else {
            damageContext->addStatusMask = 2147483648; //Figure out why this gets set
            *word_9AAD1E = (i8)inflictedStatusMask & 3;
        }
    }
}
