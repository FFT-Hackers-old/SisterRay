#include "animation_script_parser.h"
#include "../battle_models/battle_model_state_interface.h"
#include "../../impl.h"

void runAnimationScript(u16 actorID, u8** ptrToScriptTable) {
    AnimScriptContext* ptrToScriptContext = (AnimScriptContext*)0x8FE2AC;
    auto& scriptOwnerModelState = *getBattleModelState(actorID);
    auto& scriptOwner74State = *getBattleModelState74(actorID);
    auto& scriptOwnerRotationData = *getBattleModelRotationData(actorID);
    u8* byte_9ADEF8 = (u8*)byte_9ADEF8;

    if (!*BATTLE_PAUSED_GLOBAL) {
        auto& scriptContext = *ptrToScriptContext;
        *byte_9ADEF8 = 0;
        scriptContext.isScriptActive = 1;
        scriptContext.scriptPtr = ptrToScriptTable[scriptOwnerModelState.animScriptIndex];
        AnimScriptEvent srEvent = { actorID, scriptContext.scriptPtr, &scriptContext, scriptContext.currentOpCode, getBattleModelState(actorID), ptrToScriptTable };
        gContext.eventBus.dispatch(ON_RUN_ANIMATION_SCRIPT, (void*)&srEvent);

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
            while (scriptContext.isScriptActive) {
                scriptContext.currentOpCode = scriptContext.scriptPtr[scriptOwnerModelState.currentScriptPosition++];
                if (scriptContext.currentOpCode < 0x8E) {
                    scriptOwnerModelState.runningAnimIdx = scriptContext.currentOpCode;
                    scriptOwnerModelState.field_74 = 0;
                    scriptOwner74State.field_36 = 0;
                    scriptOwnerRotationData.field_0 = 0;
                    scriptContext.isScriptActive = 0;
                    srHandleAnimateModel(actorID);
                }
                AnimScriptEvent animScriptEvent = { actorID, scriptContext.scriptPtr, &scriptContext, scriptContext.currentOpCode, getBattleModelState(actorID), ptrToScriptTable };
                auto opcode = gContext.animScriptOpcodes.get_element(assembleOpCodeKey(scriptContext.currentOpCode));
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
                    scriptOwnerModelState.runningAnimIdx = scriptContext.currentOpCode;
                    scriptOwnerModelState.field_74 = 0;
                    scriptOwner74State.field_36 = 0;
                    scriptOwnerRotationData.field_0 = 0;
                    scriptContext.isScriptActive = 0;
                    srHandleAnimateModel(actorID);
                    break;
                }
                }
            }
        }
    }
}
