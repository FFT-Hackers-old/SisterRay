#include "animation_script_parser.h"
#include "../battle_models/battle_model_state_interface.h"
#include "../../impl.h"

void runAnimationScript(u16 actorID, void** ptrToScriptTable) {
    GameAnimationScriptContext** ptrToScriptContext = (GameAnimationScriptContext**)0x8FE2AC;
    auto& scriptOwnerModelState = *getBattleModelState(actorID);
    auto& scriptOwner74State = *getBattleModelState74(actorID);
    auto& scriptOwnerRotationData = *getBattleModelRotationData(actorID);
    u8* byte_9ADEF8 = (u8*)byte_9ADEF8;

    if (!*BATTLE_PAUSED_GLOBAL) {
        GameAnimationScriptContext* scriptContextPtr = *ptrToScriptContext;
        *byte_9ADEF8 = 0;
        scriptContextPtr->isScriptActive = 1;
        scriptContextPtr->scriptPtr = (u8*)ptrToScriptTable[scriptOwnerModelState.animScriptIndex];

        /*switch (scriptOwnerModelState->animScriptIndex) {
        case 46:
            scriptContextPtr->scriptPtr = unk_7C10E0;
            break;
        case 47:
            scriptContextPtr->scriptPtr = unk_7C10F0;
            break;
        case 48:
            scriptContextPtr->scriptPtr = unk_7C10F8;
            break;
        case 49:
            scriptContextPtr->scriptPtr = unk_7C1120;
            break;
        case 50:
            scriptContextPtr->scriptPtr = unk_7C1130;
            break;
        case 52:
            scriptContextPtr->scriptPtr = unk_7C1118;
            break;
        case 53:
            scriptContextPtr->scriptPtr = unk_7C1170;
            break;
        case 54:
            scriptContextPtr->scriptPtr = unk_7C1160;
            break;
        case 55:
            scriptContextPtr->scriptPtr = unk_7C1150;
            break;
        case 56:
            scriptContextPtr->scriptPtr = unk_7C1140;
            break;
        case 57: {
            getBattleModelState(actorID)->field_25 |= 0x80u;
            scriptContextPtr->scriptPtr = unk_7C1110;
            break;
        }
        case 58:
            scriptContextPtr->scriptPtr = unk_7C1108;
            break;
        case 59:
            scriptContextPtr->scriptPtr = unk_7C110C;
            break;
        default:
            break;
        }*/

        if (scriptOwnerModelState.modelEffectFlags & 1) {
            scriptOwnerModelState.isScriptExecuting = 1;
            scriptOwnerModelState.currentScriptPosition = 0;
            scriptOwnerModelState.waitFrames = 0;
            scriptOwnerModelState.modelEffectFlags &= 0xFEu;
        }
        if (scriptOwnerModelState.isScriptExecuting) {
            scriptOwnerModelState.playedAnimFrames = 0;
            while (scriptContextPtr->isScriptActive) {
                scriptContextPtr->currentOpCode = scriptContextPtr->scriptPtr[scriptOwnerModelState.currentScriptPosition++];
                AnimScriptEvent animScriptEvent = { scriptContextPtr, scriptContextPtr->scriptPtr, scriptContextPtr->currentOpCode, BATTLE_MODEL_STATE_BIG_ARRAY };
                auto opcode = gContext.animScriptOpcodes.get_element(assembleOpCodeKey(scriptContextPtr->currentOpCode));
                auto control = opcode(&animScriptEvent);
                switch (control) {
                case RUN_NEXT: {
                    continue;
                    break;
                }
                case BREAK: {
                    return;
                    break;
                }
                case PLAY_ANIM: {
                    scriptOwnerModelState.runningAnimIdx = scriptContextPtr->currentOpCode;
                    scriptOwnerModelState.field_74 = 0;
                    scriptOwner74State.field_36 = 0;
                    scriptOwnerRotationData.field_0 = 0;
                    scriptContextPtr->isScriptActive = 0;
                    srHandleAnimateModel(actorID);
                    break;
                }
                }
            }
        }
    }
}

void srHandleAnimateModel(u16 actorID) {
    u8* byte_9ADEF8 = (u8*)0x9ADEF8;
    u8* byte_BFD0E4 = (u8*)0xBFD0E4;
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
