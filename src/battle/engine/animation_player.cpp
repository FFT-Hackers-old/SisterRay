#include "animation_player.h"
#include "../battle_models/battle_model_state_interface.h"
#include "../../impl.h"

void srHandleAnimateModel(u16 actorID) {
    u8* byte_9ADEF8 = (u8*)0x9ADEF8;
    u8* byte_BFD0E4 = (u8*)0xBFD0E4;
    u8* actorIdleAnimScripts = (u8*)0xBF2DF8;
    auto& scriptOwnerModelState = *getBattleModelState(actorID);
    auto& scriptOwner74State = *getBattleModelState74(actorID);
    auto& scriptOwnerRotationData = *getBattleModelRotationData(actorID);
    if (!(scriptOwner74State.field_C & 2)) {
        scriptOwner74State.field_36++;
        // Handle the idle animation case
        if (scriptOwnerModelState.animScriptIndex == actorIdleAnimScripts[actorID]) {
            auto unkModeToggle = scriptOwner74State.field_33;
            switch (unkModeToggle) {
            case 1: {
                srPlayModelAnimation(actorID);
                srPlayModelAnimation(actorID);
                break;
            }
            case 2: {
                if (*byte_BFD0E4 & 1)
                    srPlayModelAnimation(actorID);
                break;
            }
            case 3: {
                scriptOwnerModelState.field_74 = 0;
                scriptOwnerRotationData.field_0 = 0;
                *byte_9ADEF8 = 1;
                srPlayModelAnimation(actorID);
                break;
            }
            default: {
                srPlayModelAnimation(actorID);
                break;
            }
            }
            return;
        }
        srPlayModelAnimation(actorID);
    }
}

void srPlayModelAnimation(u16 actorID) {
    auto& actorModelState = *getBattleModelState(actorID);
    auto battleModel = actorModelState.modelDataPtr;
    u8* byte_9ADEF8 = (u8*)0x9ADEF8;
    u8* byte_9ADEFC = (u8*)0x9ADEFC;
    u8* byte_9ADF04 = (u8*)0x9ADF04;
    u8* byte_9ADF00 = (u8*)0x9ADF00;
    u8* byte_BF2A30 = (u8*)0xBF2A30;

    if (battleModel) {
        actorModelState.field_10 = battleModel->skeletonData->totalBones;
        actorModelState.tableRelativeModelAnimIdx = -1;
        auto modelAnimationIndex = actorModelState.runningAnimIdx;
        if (modelAnimationIndex < 0) {
            actorModelState.isScriptExecuting = 1;
            srLogWrite("attempting to play invalid model animation for actor: %d, index: %d", actorID, modelAnimationIndex);
            return;
        }

        actorModelState.setForLimitBreaks = 0;
        if (modelAnimationIndex >= battleModel->totalAnimations) {
            actorModelState.isScriptExecuting = 1;
            srLogWrite(
                "attempting to play invalid model animation for actor: %d, index: %d, max_anims: %d",
                actorID, modelAnimationIndex, battleModel->totalAnimations);
            return;
        }

        actorModelState.tableRelativeModelAnimIdx = modelAnimationIndex;
        auto playingAnimation = battleModel->animationsTable[modelAnimationIndex];
        if (playingAnimation) {
            actorModelState.currentPlayingFrame = actorModelState.playedAnimFrames;
            if (!*byte_9ADEF8) {
                auto continuePlaying = 1;
                if (*byte_9ADEFC && *byte_9ADF04 && actorID == *byte_9ADF00) {
                    if (*byte_BF2A30) {
                        actorModelState.playedAnimFrames = playingAnimation->frameCount - 1;
                        actorModelState.currentPlayingFrame = actorModelState.playedAnimFrames;
                        continuePlaying = 0;
                    }
                    else {
                        *byte_9ADF04 = 0;
                        *byte_9ADEFC = 0;
                    }
                }
                if (continuePlaying)
                    actorModelState.playedAnimFrames++;
            }

            if (actorModelState.playedAnimFrames < playingAnimation->frameCount) {
                actorModelState.isScriptExecuting = 0;
            }
            else {
                actorModelState.playedAnimFrames = playingAnimation->frameCount - 1;
                actorModelState.isScriptExecuting = 1;
            }
        }
        return;
    }
    srLogWrite("CRITICAL: Feteched nullptr for actor %d when executing animation!", actorID);
}


typedef i64(*SRPFNCALCULATEVECTORCOPMONENTS)(i32);
#define gameCalculateXComponent         ((SRPFNCALCULATEVECTORCOPMONENTS)0x662538)
i64 srCalculateXVectorComponent(i32 distance) {
    return gameCalculateXComponent(distance);
}


#define gameCalculateZComponent         ((SRPFNCALCULATEVECTORCOPMONENTS)0x6624FD)
i64 srCalculateZVectorComponent(i32 distance) {
    return gameCalculateZComponent(distance);
}

