#include "command_sr_callbacks.h"
#include "damage_callbacks.h"
#include "../battle/scene_globals.h"
#include "../battle/engine/damage_events.h"
#include "../impl.h"
#include "status_names.h"
#include "element_names.h"
#include "damage_callback_utils.h"

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
                                auto gameDamageEvent = newDamageEvent();
                                DamageCalculationEvent srDamageEvent{ setupEvent.damageContext, setupEvent.srDamageContext, setupEvent.aiContext, gameDamageEvent };
                                srDamageEvent.srDamageContext->attackerState = gContext.battleActors.getActiveBattleActor(attackerActorID);
                                calculateDamage(&srDamageEvent, attackerActorID, targetActorID);
                        }
                        if (damageContext->attackHitCount)
                            --damageContext->attackHitCount;

                    } while (damageContext->attackHitCount);// for every attackHitCount
                    unTarget();
                    handleAdditionalEffects(5);
                }
            }
            handleDrainEffects(setupEvent);
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

 void handleDrainEffects(CommandSetupEvent setupEvent) {
     for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
         const auto& actorState = gContext.battleActors.getActiveBattleActor(actorIdx);
         if (actorState.actorTimers->drainedHP)
             handleActorDrain(setupEvent, actorIdx, false);
         if (actorState.actorTimers->drainedMP)
             handleActorDrain(setupEvent, actorIdx, true);
     }
 }

 void handleActorDrain(CommandSetupEvent setupEvent, u8 actorIdx, bool drainMP) {
     auto& aiContext = *setupEvent.aiContext;
     auto& damageCtx = *setupEvent.damageContext;
     auto& actorState = gContext.battleActors.getActiveBattleActor(actorIdx);
     bool isDead = std::find(actorState.activeStatuses->begin(), actorState.activeStatuses->end(), StatusNames::DEATH) != actorState.activeStatuses->end();
     if (!isDead) {
         auto animationEvent = createAnimEvent(actorIdx, 1, 46, 0, 0, 0, 0, 0xFFFF);
         auto damageFlagLocal = 1;
         auto drainedHP = 0;
         if (!drainMP) {
             drainedHP = actorState.actorTimers->drainedHP;
             actorState.actorTimers->drainedHP = 0;
         }
         else{
             drainedHP = actorState.actorTimers->drainedMP;
             actorState.actorTimers->drainedMP = 0;
         }
         damageCtx.targetID = actorIdx;
         damageCtx.abilityFlags2 = a2 != 0 ? 4 : 0;
         if (drainedHP < 0) {
             drainedHP = -drainedHP;
             damageCtx.abilityFlags2 |= 1;
         }
         bool isMagic = damageCtx.specialAbilityFlags & 4;
         if (actorIsDamageImmune(aiContext.actorAIStates[actorIdx], actorState, isMagic))
             drainedHP = 0;
         damageCtx.currentDamage = drainedHP;

         auto damageEvent = newDamageEvent();
         DamageCalculationEvent srDamageEvent{setupEvent.damageContext, setupEvent.srDamageContext, setupEvent.aiContext, damageEvent};
         handleDamage(srDamageEvent);
         if (actorPosessesStatus(aiContext.actorAIStates[actorIdx], actorState, StatusNames::DEATH)) {
             damageFlagLocal = 5;
             damageCtx.wasKilledMask |= 1 << actorIdx;
         }
         damageEvent->targetID = actorIdx;
         damageEvent->attackerID = actorIdx;
         damageEvent->damagedAnimScriptIdx = 46;
         damageEvent->specialDamageFlags = damageFlagLocal;
         damageEvent->targetStatusMask = aiContext.actorAIStates[actorIdx].statusMask;
         createImpactEvent(damageEvent, damageCtx.currentDamage, damageCtx.abilityFlags2, -1, -1,
             aiContext.actorAIStates[actorIdx].currentHP, aiContext.actorAIStates[actorIdx].currentHP);
         damageEvent->targetID = 0xFF;
     }
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
    setupEvent.srDamageContext->attackStats = action.attackStats;
    setupEvent.srDamageContext->statusToInflict = action.statusAttack;
    setupEvent.srDamageContext->attackElements = action.attackElements;
    setupEvent.srDamageContext->useActionDamageLimits = action.useOverrideLimits;
    setupEvent.srDamageContext->hpDamageLimit = action.hpDamageLimit;
    setupEvent.srDamageContext->mpDamageLimit = action.mpDamageLimit;

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
