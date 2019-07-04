#include "post_action_callbacks.h"
#include "../battle.h"
#include "../../impl.h"

void handleCounters(ActionContextEvent* actionEvent) {
    auto damageContext = actionEvent->damageContext;
    auto issuingActorID = actionEvent->issuingActorID;
    auto poppedAction = actionEvent->poppedAction;
    auto battleAIContext = actionEvent->battleAIContext;

    auto issuingActorMask = 1 << issuingActorID;
    battleAIContext->specialAttackFlags = damageContext->specialAbilityFlags;
    battleAIContext->lastCommandIdx = damageContext->commandIndex;
    battleAIContext->lastActionIdx = damageContext->relAttackIndex;
    battleAIContext->lastAbsActionIdx = damageContext->absAttackIndex;
    battleAIContext->lastActionElements = damageContext->attackElementsMask;
    auto& actorAIStates = battleAIContext->actorAIStates;

    u16 notRemovedMask = ~(*gRemovedMask | *gEscapedMask);
    u16 wasDamagedMask = notRemovedMask & damageContext->wasDamagedMask;
    u16 willCounterMask = notRemovedMask & damageContext->actionCounterable;
    u16 wasKilledMask = notRemovedMask & damageContext->wasKilledMask;

    for (auto actorIdx = 0; actorIdx < 10; ++actorIdx) {
        if ((1 << actorIdx) & wasKilledMask) {
            actorAIStates[actorIdx].prevAttackerMask = issuingActorMask;
            actorDeathHandler(issuingActorID, actorIdx, 0);
            continue;
        }

        if (!((1 << actorIdx) & wasDamagedMask)) {
            continue;
        }

        if (!((1 << actorIdx) & *gAttackerMask) && !(actorAIStates[actorIdx].statusMask) & 0x2404404) {
            *gAttackerMask |= 1 << actorIdx;
            actorAIStates[actorIdx].prevAttackerMask = issuingActorMask;
            srExecuteAIScript(actorIdx, 2, 0);

            if (!((1 << actorIdx) & willCounterMask))
                continue;

            if (issuingActorID >= 4)
                dispatchAutoActions(actorIdx, 2);

            if (word_9AB0CE & 4) {
                actorAIStates[actorIdx].prevMagAttackerMask = issuingActorMask;
                srExecuteAIScript(actorIdx, 5, 0);
            }
            else {
                actorAIStates[actorIdx].prevPhysAttackerMask = issuingActorMask;
                srExecuteAIScript(actorIdx, 4, 0);
            }
        }
    }
}


void setMimeData(ActionContextEvent* actionEvent) {
    auto damageContext = actionEvent->damageContext;
    auto issuingActorID = actionEvent->issuingActorID;
    auto poppedAction = actionEvent->poppedAction;

    if (issuingActorID < 3 && damageContext->commandIndex != CMD_MIME && damageContext->commandIndex != CMD_ENEMY_ACTION) {
        u16* mimeSupportFlags = (u16*)(0x9AAD32);
        auto someByte = gActorTimerBlock[issuingActorID].field_2A;
        for (auto mimeActionIdx = 0; mimeActionIdx < 2; ++mimeActionIdx) {
            if (poppedAction->entryPriority != 3 && (G_MIME_ACTIONS[mimeActionIdx].attackerID != issuingActorID || G_MIME_ACTIONS[mimeActionIdx].field_0 != someByte || damageContext->commandIndex < 21)) {
                G_MIME_ACTIONS[mimeActionIdx].attackerID = issuingActorID;
                G_MIME_ACTIONS[mimeActionIdx].field_0 = someByte;
                G_MIME_ACTIONS[mimeActionIdx].commandIdx = damageContext->commandIndex;
                G_MIME_ACTIONS[mimeActionIdx].relAttackIdx = damageContext->relAttackIndex;
                G_MIME_ACTIONS[mimeActionIdx].targetMask = damageContext->finalTargetMask;
                mimeSupportFlags[mimeActionIdx] = damageContext->supportMatFlags;
                if (damageContext->unkDword9)
                    mimeSupportFlags[mimeActionIdx] |= (u16)(damageContext->unkDword9 << 8);
                if (mimeActionIdx + 1 < 2)
                    G_MIME_ACTIONS[mimeActionIdx].attackerID = -1;
                break;
            }
        }
    }
}

void handleLuckySevens(ActionContextEvent* actionEvent) {
    auto damageContext = actionEvent->damageContext;
    auto issuingActorID = actionEvent->issuingActorID;
    auto poppedAction = actionEvent->poppedAction;
    enqueue7777Hits();
}

void handleAddedCut(ActionContextEvent* actionEvent) {
    auto damageContext = actionEvent->damageContext;
    auto issuingActorID = actionEvent->issuingActorID;
    auto poppedAction = actionEvent->poppedAction;
    if (damageContext->miscActionFlags & 0x800)// addedCut
        enqueueAction(issuingActorID, 3, 1, 0, damageContext->finalTargetMask);
}

void handleSense(ActionContextEvent* actionEvent) {
    auto damageContext = actionEvent->damageContext;
    auto issuingActorID = actionEvent->issuingActorID;
    auto poppedAction = actionEvent->poppedAction;
    if (damageContext->commandIndexCopy == CMD_SENSE) {
        damageContext->wasDamagedMask = 0;
        damageContext->actionCounterable = 0;
    }
}
