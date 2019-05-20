#include "battle_context.h"
#include "../impl.h"

void srLoadAbilityData() {
        i32 sceneAbilityIndex; 
        i32 enemyActionIndex;
        u16 elementMask;
        i32 fetchedRelativeIndex;
        i32 limitAnimationScriptIndex; 
        i32 relativeLimitIndex; 
        u8 characterID; 
        u8* activeLimitIDs; 
        i32 relativeIndex; 
        i32 charLimitArrayIndex; 
        i32 deathSentenceFlag; 
        u16 cameraOverrideData;
        AttackData abilityData;
        BattleQueueEntry executingAction = (*(BattleQueueEntry*)0x9A9884);
        AttackData* currentSceneAbilities = (AttackData*)(0x9A90C4);
        char* attackNamesPtr = (char*)(0x9A9484);
        u16* currentSceneAbilityIDs = (u16*)(0x9A9444);
        AttackData* kernelAbilities = (AttackData*)(0xDB9690);
        EnemyData* enemyData = (EnemyData*)(0x9A8E9C);
        u8* kernelLimitScriptIndexes = (u8*)(0x7B76A0);
        u8* unkPartyStructPtr = (u8*)(0x9A87F4);

        cameraOverrideData = 0xFFFF;
        deathSentenceFlag = 0;
        gDamageContextPtr->MPCost = -1;
        if (gDamageContextPtr->commandIndexCopy == CMD_ENEMY_ACTION) {
            auto attackID = std::string(std::string("ETK-") + std::to_string(gDamageContextPtr->absAttackIndex));
            auto enemyAttack = gContext.attacks.get_element(attackID);
            abilityData = gContext.enemyAttacks.get_element(attackID).attackData;
            currentSceneAbilities[0] = enemyAttack.attackData;
            memcpy((void*)attackNamesPtr, (void*)(enemyAttack.attackName.str()), enemyAttack.attackName.size());
            *(attackNamesPtr + enemyAttack.attackName.size()) = char(255);
            *currentSceneAbilityIDs = enemyAttack.attackID;
            gDamageContextPtr->sceneAbilityIndex = 0;
        }
        else if (gDamageContextPtr->absAttackIndex >= 128) { //The ability data here is not contained in the kernel
            if (gDamageContextPtr->attackerID < 3) {
                characterID = unkPartyStructPtr[16 * gDamageContextPtr->attackerID]; //array access, should figure out these structs
                activeLimitIDs = (u8*)activePartyStructArray[gDamageContextPtr->attackerID].enabledLimitBytes;
                for (i32 limitIndex = 0; limitIndex < 3; ++limitIndex) {
                    relativeLimitIndex = (i8)activeLimitIDs[limitIndex];
                    if (relativeLimitIndex + 128 == gDamageContextPtr->absAttackIndex) { //If this is the limit being used
                        limitAnimationScriptIndex = 0;
                        for (charLimitArrayIndex = 0; charLimitArrayIndex < 12; ++charLimitArrayIndex) {
                            fetchedRelativeIndex = getLimitRelativeIndex(characterID, charLimitArrayIndex);
                            if (fetchedRelativeIndex != 127) {
                                if (fetchedRelativeIndex == relativeLimitIndex)
                                    break;
                                ++limitAnimationScriptIndex;
                            }
                        }
                        gDamageContextPtr->animationScriptID = limitAnimationScriptIndex + 60;
                        abilityData = (activePartyStructArray[gDamageContextPtr->attackerID].enabledLimitData[limitIndex]);
                        break;
                    }
                }
            }
        }
        else { //load abilities which have been loaded from the kernel -- this code can be relocated to load from gContext
            abilityData = kernelAbilities[gDamageContextPtr->absAttackIndex];
            if (gDamageContextPtr->absAttackIndex >= 96) {
                relativeIndex = gDamageContextPtr->absAttackIndex - 96;
                if (kernelLimitScriptIndexes[relativeIndex] != 255)
                    gDamageContextPtr->animationScriptID = kernelLimitScriptIndexes[relativeIndex];
                gDamageContextPtr->animationBaseOffset = -1;
            }
        }
        if (gDamageContextPtr->commandIndex == CMD_MAGIC && gDamageContextPtr->absAttackIndex == 54) //death sentence case hardcoded
            deathSentenceFlag = 1;
        if ((gDamageContextPtr->animationBaseOffset != -1) && !((gDamageContextPtr->miscActionflags) & 0x400000)) {
            EnabledSpell* spellData = &(activePartyStructArray[gDamageContextPtr->attackerID].enabledMagicsData[gDamageContextPtr->animationBaseOffset]);
            gDamageContextPtr->MPCost = spellData->mpCost;
            if (spellData->quadCount && spellData->quadEnabled) {
                spellData->quadCount = spellData->quadCount - 1;
                gDamageContextPtr->unkDWord5 = spellData->quadEnabled;// quadEnabled?
                gDamageContextPtr->unkDWord1 = gDamageContextPtr->unkDWord5 + 3;  // numberOfCasts?
                if (gDamageContextPtr->unkDWord1 > 8) {
                    gDamageContextPtr->unkDWord1 = 8;
                }
                srCreateEvent(2, gDamageContextPtr->attackerID, 21, 6);
            }
            else if (gDamageContextPtr->commandIndexCopy == CMD_SUMMON) {
                if (spellData->allCount) { //if yo ucan still use the summon
                    if (spellData->allCount != 255) {
                        spellData->allCount = spellData->allCount - 1;
                        srCreateEvent(2, gDamageContextPtr->attackerID, 21, 4);
                    }
                }
                else {
                    gDamageContextPtr->displayString = 121; //Probably displays "Summon Power is all used up"
                }
            }
            else if (gDamageContextPtr->miscActionflags & 0x200) { //Handle the all case
                if (spellData->allCount) {
                    if (spellData->allCount != 255) {
                        spellData->allCount = spellData->allCount - 1;
                        srCreateEvent(2, gDamageContextPtr->attackerID, 21, 2);
                    }
                }
                else if (abilityData.targetingFlags & 8) {
                    gDamageContextPtr->miscActionflags |= 0x100000u;
                }
            }
            if ((executingAction.entryPriority >= 5) && !(gDamageContextPtr->miscActionflags & 0x400000)) //priority 5 and 6 actions? what are those
                gDamageContextPtr->addedCutMPHPAbsorbByte = spellData->actorPropertiesMask;
        }
        if (deathSentenceFlag)
        {
            gDamageContextPtr->animationScriptID = 52;
        }
        else if (gDamageContextPtr->attackerID >= 4) {
            auto actingEnemyRecord =  enemyData[gDamageContextPtr->enemySceneIndex]; //Finds the "formation data" for the current actor
            for (enemyActionIndex = 0; enemyActionIndex < 16; ++enemyActionIndex) {
                if (gDamageContextPtr->absAttackIndex == actingEnemyRecord.attackSceneIDs[enemyActionIndex]) { //offset into the actual actor IDs
                    gDamageContextPtr->animationScriptID = actingEnemyRecord.attackAnimScripts[enemyActionIndex]; //offset into the animation script indexes for this model
                    cameraOverrideData = actingEnemyRecord.attackCameraIDs[enemyActionIndex];
                    break;
                }
            }
        }
        if (gDamageContextPtr->MPCost < 0)
            gDamageContextPtr->MPCost = abilityData.MPCost;
        if (executingAction.entryPriority == 3 || gDamageContextPtr->miscActionflags & 0x400000) //This is triggered by mime
            gDamageContextPtr->MPCost = 0;
        gDamageContextPtr->abilityHitRate = abilityData.abilityHitRate;
        gDamageContextPtr->damageFormulaID = abilityData.damageFormula;
        if (abilityData.elementMask == 0xFFFF) 
            elementMask = 0;
        else
            elementMask = abilityData.elementMask;
        gDamageContextPtr->attackElementsMask = elementMask;
        gDamageContextPtr->abilityPower = abilityData.attackPower;
        gDamageContextPtr->targetReactionAnimation = abilityData.targetReactionID;
        if (gDamageContextPtr->abilityTargetingFlags == 255)
            gDamageContextPtr->abilityTargetingFlags = abilityData.targetingFlags;
        if (gDamageContextPtr->commandIndexCopy == CMD_LIMIT)
            cameraOverrideData = (((*(u16*)0x9AB0CC) & 8) != 0) ? abilityData.cameraMovementMultiple : abilityData.cameraMovementSingle;
        if (cameraOverrideData == 0xFFFF) {
            gDamageContextPtr->cameraDataSingle = abilityData.cameraMovementSingle;
            gDamageContextPtr->cameraDataMultiple = abilityData.cameraMovementMultiple;
        }
        else {
            gDamageContextPtr->cameraDataSingle = cameraOverrideData;
            gDamageContextPtr->cameraDataMultiple = cameraOverrideData;
        }
        if (abilityData.impactEffectID!= 255)
            gDamageContextPtr->impactEffectID = abilityData.impactEffectID;
        gDamageContextPtr->animationEffectID = abilityData.animationEffectID;
        gDamageContextPtr->specialAbilityFlags = abilityData.specialAttackFlags;
        gDamageContextPtr->impactSound = abilityData.impactSoundID;
        gDamageContextPtr->critAtkSound = abilityData.impactSoundID;
        gDamageContextPtr->missAtkSound = abilityData.impactSoundID;
        if (!((gDamageContextPtr->specialAbilityFlags) & 4) && gAiActorVariables[gDamageContextPtr->attackerID].statusMask & 0x4000000)
            gDamageContextPtr->abilityHitRate >>= 1; //(half hit chance)
        setStatusInflictionData(abilityData.statusInflictType, abilityData.statusMask);
        if (deathSentenceFlag)
            gAiActorVariables[gDamageContextPtr->attackerID].statusMask &= 0xFFDFFFFF;
        return copyAdditionalEffects(abilityData.additionalEffect, abilityData.additionalEffectModifier);
}

void setStatusInflictionData(i32 statusInflictionByte, i32 inflictedStatusMask) {
    u16* word_9AAD1E = (u16*)(0x9AAD1E);
    gDamageContextPtr->addStatusMask = 0;     
    gDamageContextPtr->rmStatusMask = 0;      
    gDamageContextPtr->toggleStatusMask = 0;
    auto statusType = statusInflictionByte >> 6;
    if (statusType < 3)
    {
        if (inflictedStatusMask >= 0)
        {
            gDamageContextPtr->inflictStatusChance = 4 * (statusInflictionByte & 0x3F);// statusInflictionChance
            switch (statusType) {
                case 0:
                    gDamageContextPtr->addStatusMask  = inflictedStatusMask;
                    break;
                case 1:
                    gDamageContextPtr->rmStatusMask = inflictedStatusMask;
                    break;
                case 2:
                    gDamageContextPtr->toggleStatusMask = inflictedStatusMask;
                    break;
                default: {
                    break;
                }
            }
        }
        else
        {
            gDamageContextPtr->addStatusMask = 2147483648; //Figure out why this gets set
            *word_9AAD1E = (i8)inflictedStatusMask & 3;
        }
    }
}
