#include "pre_action_callbacks.h"
#include "../battle.h"
#include "../../impl.h"
#include "../../gamedata/damage_callback_utils.h"

void runPreActionScripts(ActionContextEvent* actionEvent) {
    auto damageContext = actionEvent->damageContext;
    auto issuingActorID = actionEvent->issuingActorID;
    auto poppedAction = actionEvent->poppedAction;
    auto battleAIContext = actionEvent->battleAIContext;
    auto& actorAIStates = battleAIContext->actorAIStates;
    u16* gAttackerMask = (u16*)0x9AAD30;

    if (poppedAction->entryPriority >= 2)
        *gAttackerMask = 1 << poppedAction->queueAction.attackerActorID;
    battleAIContext->lastCommandIdx = poppedAction->queueAction.actionCommandIndex;
    battleAIContext->lastActionIdx = poppedAction->queueAction.actionAttackIndex;
    if (poppedAction->entryPriority > 0) {
        for (auto actorIdx = 0; actorIdx < 10; ++actorIdx)
            srExecuteAIScript(actorIdx, 7, 0);
    }
}

void initDamageContext(ActionContextEvent* actionEvent) {
    auto damageContext = actionEvent->damageContext;
    auto issuingActorID = actionEvent->issuingActorID;
    auto poppedAction = actionEvent->poppedAction;
    auto battleAIContext = actionEvent->battleAIContext;
    auto& actorAIStates = battleAIContext->actorAIStates;

    damageContext->attackerID = issuingActorID;
    damageContext->commandIndex = poppedAction->queueAction.actionCommandIndex;
    damageContext->relAttackIndex = (u16)poppedAction->queueAction.actionAttackIndex;
    damageContext->targetMask = (u16)poppedAction->queueAction.actionTargetMask;
    damageContext->attackerLevel = actorAIStates[issuingActorID].level;
    damageContext->attackerStatusMask = actorAIStates[issuingActorID].statusMask;
    damageContext->activeAllies = 1;
    damageContext->animationScriptID = getCommand(damageContext->commandIndex).auxData.animationScriptIndex;
    actionEvent->srDamageContext->damageFormula = gContext.damageFormulas.getResource(getCommand(damageContext->commandIndex).damageFormula);
    actionEvent->srDamageContext->hitFormula = gContext.hitFormulas.getResource(getCommand(damageContext->commandIndex).hitFormula);
    actionEvent->srDamageContext->damageType = getCommand(damageContext->commandIndex).damageType;
    damageContext->miscActionFlags = getCommand(damageContext->commandIndex).auxData.miscCommandFlags;
    damageContext->enabledMagicsIndex = -1;
    damageContext->impactSound = -1;
    damageContext->critAtkSound = -1;
    damageContext->missAtkSound = -1;
    damageContext->impactEffectID = -1;
    damageContext->enemySceneIndex = -1;
    damageContext->specialAbilityFlags = -1;
    damageContext->cameraDataSingle = -1;
    damageContext->cameraDataMultiple = -1;
    damageContext->additionalEffects = -1;
    damageContext->displayString = -1;
    damageContext->unkDword8 = -1;
    damageContext->attackerMask = 1 << issuingActorID;
    damageContext->finalTargetMask = damageContext->targetMask;
    damageContext->sceneAbilityIndex = damageContext->relAttackIndex;
    damageContext->actionCount = 1;
    damageContext->inflictStatusChance = 0xFF;
    damageContext->abilityTargetingFlags = 0xFFFF;
    for (auto followUpIdx = 0; followUpIdx < 8; ++followUpIdx)// null follow ups
        damageContext->followUpActions[followUpIdx] = -1;

    if (issuingActorID >= 4) {
        FormationActorData* formationData = getInBattleFormationActorData(issuingActorID);
        damageContext->enemySceneIndex = formationData->enemyID;
    }
}

void setEnemyCommandData(ActionContextEvent* actionEvent) {
    auto damageContext = actionEvent->damageContext;
    auto issuingActorID = actionEvent->issuingActorID;
    auto poppedAction = actionEvent->poppedAction;

    if (damageContext->commandIndex != CMD_ENEMY_ACTION)
        return;
    damageContext->commandIndexCopy = CMD_ENEMY_ACTION;
}

void setPlayerCommandData(ActionContextEvent* actionEvent) {
    auto damageContext = actionEvent->damageContext;
    auto issuingActorID = actionEvent->issuingActorID;
    auto poppedAction = actionEvent->poppedAction;

    if (!isPartyActor(issuingActorID))
        return;

    if (damageContext->commandIndex != CMD_ENEMY_ACTION) {
        damageContext->abilityTargetingFlags = getCommand(damageContext->commandIndex).gameCommand.targetingFlags;
        damageContext->cameraDataSingle = getCommand(damageContext->commandIndex).gameCommand.singleCameraID;
        damageContext->cameraDataMultiple = getCommand(damageContext->commandIndex).gameCommand.multipleCameraID;
    }

    //Set the CommandIndexCopy correctly for W-Spells
    switch (damageContext->commandIndex) {
        case 21: {
            damageContext->commandIndexCopy = CMD_MAGIC;
            break;
        }
        case 22: {
            damageContext->commandIndexCopy = CMD_SUMMON;
            break;
        }
        case 23: {
            damageContext->commandIndexCopy = CMD_ITEM;
            break;
        }
        default: {
            damageContext->commandIndexCopy = damageContext->commandIndex;
        }
    }

    damageContext->absAttackIndex = damageContext->relAttackIndex;


    // calculateSpellOffsets(issuingActorID);
    auto enabledComandData = PARTY_STRUCT_ARRAY[issuingActorID].enabledCommandArray;
    if (!(damageContext->miscActionFlags & 0x400000)) {
        for (auto commandIdx = 0; commandIdx < 16; ++commandIdx) {
            if (!(enabledComandData[commandIdx].commandID == damageContext->commandIndex))
                continue;

            if (poppedAction->entryPriority >= 5) {
                damageContext->supportMatFlags = enabledComandData[commandIdx].supportMatFlags;
            }

            switch (damageContext->commandIndex) {
                case 5:
                case 6:
                case 9:
                case 0xA:
                case 0xB:
                case 0x11:
                    if (damageContext->miscActionFlags & 0x200) {
                        if (enabledComandData[commandIdx].allCount) {
                            --enabledComandData[commandIdx].allCount;
                            srCreateEvent(2, issuingActorID, 21, 1);
                        }
                        else {
                            damageContext->miscActionFlags |= 0x100000u;
                        }
                    }
                    damageContext->abilityTargetingFlags = enabledComandData[commandIdx].targetingData;
                    break;
                default: {

                }
            }
            break;
        }
    }
}

typedef i32(*PFNSUBSR43740B)(u32);
#define sub_43740B   ((PFNSUBSR43740B)0x43740B)

typedef u16(*PFNSUBSR5C80B5)(u16);
#define getRandomTarget  ((PFNSUBSR5C80B5)0x5C80B5)

void handleToad(ActionContextEvent* actionEvent) {
    auto damageContext = actionEvent->damageContext;
    auto issuingActorID = actionEvent->issuingActorID;
    auto poppedAction = actionEvent->poppedAction;

    if (!isPartyActor(issuingActorID))
        return;

    if (actorHasStatus(issuingActorID, STS_TOAD)) {
        if (poppedAction->queueAction.actionCommandIndex == sub_43740B(issuingActorID)) {
            poppedAction->queueAction.actionCommandIndex = 1;
            poppedAction->queueAction.actionTargetMask = getRandomTarget(poppedAction->queueAction.actionTargetMask);
        }
    }
}

void setAllFlag(ActionContextEvent* actionEvent) {
    auto damageContext = actionEvent->damageContext;
    auto issuingActorID = actionEvent->issuingActorID;
    auto poppedAction = actionEvent->poppedAction;

    if (damageContext->finalTargetMask) {
        if (countTargets(damageContext->finalTargetMask) > 1) {
            damageContext->miscActionFlags = (damageContext->miscActionFlags |= 0x200);
        }
    }
    else if (isPartyActor(issuingActorID)) {
        damageContext->miscActionFlags = (damageContext->miscActionFlags |= 0x200);
    }
}

void prepareMimedAction(ActionContextEvent* actionEvent) {
    auto damageContext = actionEvent->damageContext;
    auto issuingActorID = actionEvent->issuingActorID;
    auto poppedAction = actionEvent->poppedAction;
    u16 prevActionSupportFlags = -1;
    if (!(poppedAction->queueAction.actionCommandIndex & 0x40))
        return;

    for (auto mimeCmdIdx = 0; mimeCmdIdx < 2; ++mimeCmdIdx) {
        if (G_BATTLE_PARTY34_ARRAY[issuingActorID].previousSupportMasks[mimeCmdIdx] != 0xFFFF) {
            prevActionSupportFlags = G_BATTLE_PARTY34_ARRAY[issuingActorID].previousSupportMasks[mimeCmdIdx];
            G_BATTLE_PARTY34_ARRAY[issuingActorID].previousSupportMasks[mimeCmdIdx] = -1;
            break;
        }
    }

    if (prevActionSupportFlags != -1) {
        if (prevActionSupportFlags >> 8) {
            damageContext->quadEnabled = prevActionSupportFlags >> 8;       // quadEnabled?
            damageContext->quadCount = damageContext->quadCount + 3;
        }
        if (damageContext->quadCount > 8)
            damageContext->quadCount = 8;
        damageContext->supportMatFlags = prevActionSupportFlags;
        damageContext->miscActionFlags |= 0x400000u;
    }
}
