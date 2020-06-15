#include "animation_player.h"
#include "../battle_models/battle_model_state_interface.h"
#include "../../impl.h"

u32 previousFrameInputMask;

void srHandleAnimateModel(u8 actorID) {
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

void srPlayModelAnimation(u8 actorID) {
    auto& actorModelState = *getBattleModelState(actorID);
    auto battleModel = actorModelState.modelDataPtr;
    u8* byte_9ADEF8 = (u8*)0x9ADEF8;
    u8* byte_9ADEFC = (u8*)0x9ADEFC;
    u8* byte_9ADF04 = (u8*)0x9ADF04;
    u8* byte_9ADF00 = (u8*)0x9ADF00;
    u8* byte_BF2A30 = (u8*)0xBF2A30;

    if (battleModel) {
        actorModelState.totalBones = battleModel->skeletonData->totalBones;
        actorModelState.tableRelativeModelAnimIdx = -1;
        ModelAnimationEvent modelEvent = { actorID,  &actorModelState, getBattleModelState74(actorID) };
        gContext.eventBus.dispatch(SETUP_MODEL_ANIMATION, &modelEvent);
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


        dispatchAnimationInputs(&modelEvent);
        actorModelState.tableRelativeModelAnimIdx = modelAnimationIndex;
        auto playingAnimation = battleModel->animationsTable[modelAnimationIndex];
        if (playingAnimation) {
            //srLogWrite("Start of Anim Player Table Relative Idx: %i, animIdx: %i, playing Frame: %i, frames played: %i", actorModelState.tableRelativeModelAnimIdx, actorModelState.runningAnimIdx, actorModelState.currentPlayingFrame, actorModelState.playedAnimFrames);
            actorModelState.currentPlayingFrame = actorModelState.playedAnimFrames;
            if (!*byte_9ADEF8) {
                auto continuePlaying = 1;

                if (*byte_9ADEFC && *byte_9ADF04 && actorID == *byte_9ADF00) {
                    if (*byte_BF2A30) {
                        actorModelState.playedAnimFrames = playingAnimation->frameCount - 1;
                        actorModelState.currentPlayingFrame = actorModelState.playedAnimFrames;
                        //srLogWrite("Doing DAT frame count update");
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

            //srLogWrite("End of Anim Player Table Relative Idx: %i, animIdx: %i, playing Frame: %i, frames played: %i", actorModelState.tableRelativeModelAnimIdx, actorModelState.runningAnimIdx, actorModelState.currentPlayingFrame, actorModelState.playedAnimFrames);
            gContext.eventBus.dispatch(POST_MODEL_ANIMATION, &modelEvent);
        }
        return;

        srLogWrite("End of Anim Player Table Relative Idx: %i", actorModelState.tableRelativeModelAnimIdx);
    }
    srLogWrite("CRITICAL: Feteched nullptr for actor %d when executing animation!", actorID);
}

void dispatchAnimationInputs(ModelAnimationEvent* modelAnimEvent) {
    std::vector<SrEventContext> dispatchContext{ PLAYING_ANIMATION };
    u32* capturedInputMask = ((u32*)0x9A85D4);
    srLogWrite("INPUT MASK STATE DURING ANIM CAPTURE: %x", *capturedInputMask);
    if (checkInputReceived(32)) {
        gContext.eventBus.dispatch(MENU_INPUT_OK, modelAnimEvent, dispatchContext);
    }
    else if (checkInputReceived(64)) {
        gContext.eventBus.dispatch(MENU_INPUT_CANCEL, modelAnimEvent, dispatchContext);
    }
    else if (checkInputReceived(2)) {
        gContext.eventBus.dispatch(MENU_INPUT_R2, modelAnimEvent, dispatchContext);
    }
    else if (checkInputReceived(4)) {
        gContext.eventBus.dispatch(MENU_INPUT_L1, modelAnimEvent, dispatchContext);
    }
    else if (checkInputReceived(8)) {
        gContext.eventBus.dispatch(MENU_INPUT_R1, modelAnimEvent, dispatchContext);
    }
    else if (*capturedInputMask & 128u) {
        srLogWrite("CAPTURED DISPATCHED INPUT DURING ANIMATION");
        gContext.eventBus.dispatch(MENU_INPUT_SQUARE, modelAnimEvent, dispatchContext);
    }
    else if (*capturedInputMask & 16u) {
        srLogWrite("CAPTURED DISPATCHED INPUT DURING ANIMATION");
        gContext.eventBus.dispatch(MENU_INPUT_TRIANGLE, modelAnimEvent, dispatchContext);
    }
    previousFrameInputMask = *capturedInputMask;
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


typedef void(*SRPFN_COMPUTEENEMYCENTER)(u16, R3PointWord*);
#define gameComputeEnemyPartyCenter     ((SRPFN_COMPUTEENEMYCENTER)0x5BFEC9)
void srComputeEnemyPartyCenter(u16 targetMask, R3PointWord* ret) {
    gameComputeEnemyPartyCenter(targetMask, ret);
}

u32 getPreviousInputMask() {
    return previousFrameInputMask;
}
