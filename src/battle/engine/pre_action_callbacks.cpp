#include "pre_action_callbacks.h"

void runPreActionScripts(BattleQueueEntry* poppedAction, u8 issuingActorID) {
    u16* gAttackerMask = (u16*)0x9AAD30;

    if (poppedAction->entryPriority >= 2)
        gAttackerMask = 1 << poppedAction->queueAction.attackerActorID;
    AI_BATTLE_CONTEXT->lastCommandIdx = poppedAction->queueAction.actionCommandIndex;
    AI_BATTLE_CONTEXT->lastActionIdx = poppedAction->queueAction.actionAttackIndex;
    if (poppedAction->entryPriority > 0) {
        for (auto actorIdx = 0; actorIdx < 10; ++actorIdx)
            srExecuteAIScript(actorIdx, 7, 0);
    }
}

void initDamageContext(BattleQueueEntry* poppedAction, u8 issuingActorID) {
    gDamageContextPtr->attackerID = issuingActorID;
    gDamageContextPtr->commandIndex = poppedAction->queueAction.commandIndex;
    gDamageContextPtr->relAttackIndex = (u16)poppedAction->queueAction.attackIndex;
    gDamageContextPtr->targetMask = (u16)poppedAction->queueAction.targetMask;
    gDamageContextPtr->attackerLevel = actorAIStates[issuingActorID].level;
    gDamageContextPtr->attackerStatusMask = actorAIStates[issuingActorID].statusMask;
    gDamageContextPtr->activeAllies = 1;
    gDamageContextPtr->animationScriptID = gContext.auxCommands.get_resource(gDamageContextPtr->commandIndex).animationScriptIndex;
    gDamageContextPtr->damageFormulaID = gContext.auxCommands.get_resource(gDamageContextPtr->commandIndex).damageCalculationByte;
    gDamageContextPtr->miscActionFlags = gContext.auxCommands.get_resource(gDamageContextPtr->commandIndex).miscCommandFlags;
    gDamageContextPtr->animationBaseOffset = -1;
    gDamageContextPtr->impactSound = -1;
    gDamageContextPtr->critAtkSound = -1;
    gDamageContextPtr->missAtkSound = -1;
    gDamageContextPtr->impactEffectID = -1;
    gDamageContextPtr->enemySceneIndex = -1;
    gDamageContextPtr->specialAbilityFlags = -1;
    gDamageContextPtr->cameraDataSingle = -1;
    gDamageContextPtr->cameraDataMultiple = -1;
    gDamageContextPtr->additionalEffects = -1;
    gDamageContextPtr->displayString = -1;
    gDamageContextPtr->unkDword8 = -1;
    gDamageContextPtr->attackerMask = 1 << issuingActorID;
    gDamageContextPtr->finalTargetMask = gDamageContextPtr->targetMask;
    gDamageContextPtr->sceneAbilityIndex = gDamageContextPtr->relAttackIndex;
    gDamageContextPtr->doubleCutRelated = 1;
    gDamageContextPtr->inflictStatusChance = 0xFF;
    gDamageContextPtr->abilityTargetingFlags = 0xFFFF;
    for (auto followUpIdx = 0; followUpIdx < 8; ++followUpIdx)// null follow ups
        gDamageContextPtr->followUpActions[followUpIdx] = -1;
}

void setCommandData(BattleQueueEntry* poppedAction, u8 issuingActorID) {
    if (!isPartyActor(issuingActorID))
        return;

    if (gDamageContextPtr->commandIndex != CMD_ENEMY_ACTION) {
        gDamageContextPtr->abilityTargetingFlags = gContext.commands.get_resource(gDamageContextPtr->commandIndex).targetingFlags;
        gDamageContextPtr->cameraDataSingle = gContext.commands.get_resource(gDamageContextPtr->commandIndex).singleCameraID;
        gDamageContextPtr->cameraDataMultiple = gContext.commands.get_resource(gDamageContextPtr->commandIndex).multipleCameraID;
    }

    //Set the CommandIndexCopy correctly for W-Spells
    switch (gDamageContextPtr->commandIndex) {
        case 21: {
            gDamageContextPtr->commandIndexCopy = CMD_MAGIC;
            break;
        }
        case 22: {
            gDamageContextPtr->commandIndexCopy = CMD_SUMMON;
            break;
        }
        case 23: {
            gDamageContextPtr->commandIndexCopy = CMD_ITEM;
            break;
        }
        default: {
            gDamageContextPtr->commandIndexCopy = gDamageContextPtr->commandIndex;
        }
    }

    gDamageContextPtr->absAttackIndex = gDamageContextPtr->relAttackIndex;


    // calculateSpellOffsets(issuingActorID);
    auto enabledComandData = PARTY_STRUCT_ARRAY[issuingActorID].enabledCommandArray;
    if (!(gDamageContextPtr->miscActionFlags & 0x400000)) {
        for (auto commandIdx = 0; commandIdx < 16; ++commandIdx) {
            if (!(enabledComandData[commandIdx].commandID == gDamageContextPtr->commandIndex))
                continue;

            if (poppedAction->entryPriority >= 5) {
                gDamageContextPtr->supportMatFlags = enabledComandData[commandIdx].supportMatFlags;
            }

            switch (gDamageContextPtr->commandIndex) {
                case 5:
                case 6:
                case 9:
                case 0xA:
                case 0xB:
                case 0x11:
                    if (gDamageContextPtr->miscActionFlags & 0x200) {
                        if (enabledComandData[commandIdx].allCount) {
                            --enabledComandData[commandIdx].allCount;
                            srCreateEvent(2, issuingActorID, 21, 1);
                        }
                        else {
                            gDamageContextPtr->miscActionFlags |= 0x100000u;
                        }
                    }
                    gDamageContextPtr->abilityTargetingFlags = enabledComandData[commandIdx].targetingData;
                    break;
                default: {

                }
            }
            break;
        }
    }
}

void handleToad(BattleQueueEntry* poppedAction, u8 issuingActorID) {
    if (!isPartyActor(issuingActorID))
        return;

    if (actorHasStatus(issuingActorID, STS_TOAD) {
        if (poppedAction->queueAction.actionCommandIndex == sub_43740B(issuingActorID)) {
            poppedAction->queueAction.actionCommandIndex = 1;
                poppedAction->queueAction.actionTargetMask = getRandomTarget(poppedAction->queueAction.actionTargetMask);
        }
    }
}

void setAllFlag(BattleQueueEntry* poppedAction, u8 issuingActorID) {
    if (gDamageContextPtr->finalTargetMask) {
        if (countTargets(gDamageContextPtr->finalTargetMask) > 1) {
            gDamageContextPtr->miscActionFlags = (gDamageContextPtr->miscActionFlags |= 2u);
        }
    }
    else if (isPartyActor(issuingActorID)) {
        gDamageContextPtr->miscActionFlags = (gDamageContextPtr->miscActionFlags |= 2u);
    }
}

void prepareMimedAction(BattleQueueEntry* poppedAction, u8 issuingActorID) {
    u16 prevActionSupportFlags = -1;
    if (!(poppedAction->queueAction.actionCommandIndex & 0x40))
        return;

    for (auto mimeCmdIdx = 0; mimeCmdIdx < 2; ++mimeCmdIdx) {
        if (gUnkActorArray[issuingActorID].previousSupportMasks[mimeCmdIdx] != 0xFFFF) {
            prevActionSupportFlags = gUnkActorArray[issuingActorID].previousSupportMasks[mimeCmdIdx];
            gUnkActorArray[issuingActorID].previousSupportMasks[mimeCmdIdx] = -1;
            break;
        }
    }

    if (prevActionSupportFlags != -1) {
        if (prevActionSupportFlags >> 8) {
            gDamageContextPtr->quadEnabled = prevActionSupportFlags >> 8;       // quadEnabled?
            gDamageContextPtr->quadCount = gDamageContextPtr->quadCount + 3;
        }
        if (gDamageContextPtr->quadCount > 8)
            gDamageContextPtr->quadCount = 8;
        gDamageContextPtr->supportMatFlags = prevActionSupportFlags;
        gDamageContextPtr->miscActionFlags |= 0x400000u;
    }
}
