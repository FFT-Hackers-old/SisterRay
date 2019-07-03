#include "post_action_callbacks.h"

void handleCounters(BattleQueueEntry* poppedAction, u8 issuingActorID) {
    auto issuingActorMask = 1 << issuingActorID;
    AI_BATTLE_CONTEXT->specialAttackFlags = gDamageContextPtr->specialAbilityFlags;
    AI_BATTLE_CONTEXT->lastCommandIdx = gDamageContextPtr->commandIndex;
    AI_BATTLE_CONTEXT->lastActionIdx = gDamageContextPtr->relAttackIndex;
    AI_BATTLE_CONTEXT->lastAbsActionIdx = gDamageContextPtr->absAttackIndex;
    AI_BATTLE_CONTEXT->lastActionElements = gDamageContextPtr->attackElementsMask;
    auto& actorAIStates = AI_BATTLE_CONTEXT->actorAIStates;
    u16 notRemovedMask = ~(*gRemovedMask | *gEscapedMask);
    u16 wasDamagedMask = v14 & gDamageContextPtr->wasDamagedMask;
    u16 willCounterMask = v14 & gDamageContextPtr->actionCounterable;
    u16 wasKilledMask = v14 & gDamageContextPtr->wasKilledMask;

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


void setMimeData(BattleQueueEntry* poppedAction, u8 issuingActorID) {
    if (issuingActorID < 3 && gDamageContextPtr->commandIndex != CMD_MIME && gDamageContextPtr->commandIndex != CMD_ENEMY_ACTION) {
        u16* mimeSupportFlags = (u16*)(0x9AAD32);
        auto someByte = gActorTimerBlock[issuingActorID].field_2A;
        for (auto mimeActionIdx = 0; mimeActionIdx < 2; ++mimeActionIdx) {
            if (poppedAction->entryPriority != 3 && (G_MIME_ACTIONS[mimeActionIdx].attackerID != issuingActorID || G_MIME_ACTIONS[mimeActionIdx].field_0 != someByte || gDamageContextPtr->commandIndex < 21)) {
                G_MIME_ACTIONS[mimeActionIdx].attackerID = issuingActorID;
                G_MIME_ACTIONS[mimeActionIdx].field_0 = someByte;
                G_MIME_ACTIONS[mimeActionIdx].commandIdx = gDamageContextPtr->commandIndex;
                G_MIME_ACTIONS[mimeActionIdx].relAttackIdx = gDamageContextPtr->relAttackIndex;
                G_MIME_ACTIONS[mimeActionIdx].targetMask = gDamageContextPtr->finalTargetMask;
                mimeSupportFlags[mimeActionIdx] = gDamageContextPtr->supportMatFlags;
                if (gDamageContextPtr->unkDword9)
                    mimeSupportFlags[mimeActionIdx] |= (u16)(gDamageContextPtr->unkDword9 << 8);
                if (mimeActionIdx + 1 < 2)
                    G_MIME_ACTIONS[mimeActionIdx].attackerID = -1;
                break;
            }
        }
    }
}

void handleLuckySevens(BattleQueueEntry* poppedAction, u8 issuingActorID) {
    enqueue7777Hits();
}

void handleAddedCut(BattleQueueEntry* poppedAction, u8 issuingActorID) {
    if (gDamageContextPtr->miscActionFlags & 0x800)// addedCut
        enqueueAction(issuingActorID, 3, 1, 0, gDamageContextPtr->finalTargetMask);
}
