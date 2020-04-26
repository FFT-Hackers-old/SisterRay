#include "animation_script_parser.h"
#include "../battle_models/battle_model_state_interface.h"
#include "../../impl.h"

void runAnimationScript(u8 actorID, u8** ptrToScriptTable) {
    AnimScriptContext** scriptCtxGlobal = (AnimScriptContext**)0x8FE2AC;
    auto& ownerModelState = *getBattleModelState(actorID);
    auto& scriptOwner74State = *getBattleModelState74(actorID);
    auto& scriptOwnerRotationData = *getBattleModelRotationData(actorID);
    u8* byte_9ADEF8 = (u8*)0x9ADEF8;
    if (ownerModelState.animScriptIndex != 0) {
        srLogWrite("running animation script with idx: %d for actor %d. current position: %i", ownerModelState.animScriptIndex, actorID, ownerModelState.currentScriptPosition);
    }
    if (!*BATTLE_PAUSED_GLOBAL) {
        auto& scriptCtx = *(*scriptCtxGlobal);
        *byte_9ADEF8 = 0;
        scriptCtx.isScriptActive = 1;
        scriptCtx.scriptPtr = ptrToScriptTable[ownerModelState.animScriptIndex];
        AnimScriptEvent srEvent = { actorID, scriptCtx.scriptPtr, &scriptCtx, scriptCtx.currentOpCode, getBattleModelState(actorID), ptrToScriptTable };
        gContext.eventBus.dispatch(ON_RUN_ANIMATION_SCRIPT, (void*)&srEvent);

        switch (ownerModelState.animScriptIndex) {
        case 46:
            scriptCtx.scriptPtr = (u8*)0x7C10E0; //script that displays poisons action
            break;
        case 47:
            scriptCtx.scriptPtr = (u8*)0x7C10F0;
            break;
        case 48:
            scriptCtx.scriptPtr = (u8*)0x7C10F8;
            break;
        case 49:
            scriptCtx.scriptPtr = (u8*)0x7C1120;
            break;
        case 50:
            scriptCtx.scriptPtr = (u8*)0x7C1130;
            break;
        case 52:
            scriptCtx.scriptPtr = (u8*)0x7C1118;
            break;
        case 53:
            scriptCtx.scriptPtr = (u8*)0x7C1170;
            break;
        case 54:
            scriptCtx.scriptPtr = (u8*)0x7C1160;
            break;
        case 55:
            scriptCtx.scriptPtr = (u8*)0x7C1150;
            break;
        case 56:
            scriptCtx.scriptPtr = (u8*)0x7C1140;
            break;
        case 57: {
            ownerModelState.field_25 |= 0x80u;
            scriptCtx.scriptPtr = (u8*)0x7C1110;
            break;
        }
        case 58:
            scriptCtx.scriptPtr = (u8*)0x7C1108;
            break;
        case 59:
            scriptCtx.scriptPtr = (u8*)0x7C110C;
            break;
        default:
            break;
        }

        if (ownerModelState.modelEffectFlags & 1) {
            ownerModelState.isScriptExecuting = 1;
            ownerModelState.currentScriptPosition = 0;
            ownerModelState.waitFrames = 0;
            ownerModelState.modelEffectFlags &= 0xFEu;
        }
        if (ownerModelState.isScriptExecuting) {
            ownerModelState.playedAnimFrames = 0;
            while (scriptCtx.isScriptActive) {

                srLogWrite("selecting opcode at position: %i", ownerModelState.currentScriptPosition);
                scriptCtx.currentOpCode = scriptCtx.scriptPtr[ownerModelState.currentScriptPosition++];
                srLogWrite("chosen opcode: %x, new script position: %i", scriptCtx.currentOpCode, ownerModelState.currentScriptPosition);
                if (scriptCtx.currentOpCode < 0x8E) {
                    if (ownerModelState.animScriptIndex != 0) {
                        srLogWrite("CURRENTLY EXECUTING ANIMATION %d for actor %d. Current position: %i", ownerModelState.runningAnimIdx, actorID, ownerModelState.currentScriptPosition);
                    }
                    ownerModelState.runningAnimIdx = scriptCtx.currentOpCode;
                    ownerModelState.field_74 = 0;
                    scriptOwner74State.field_36 = 0;
                    scriptOwnerRotationData.field_0 = 0;
                    scriptCtx.isScriptActive = 0;
                    srHandleAnimateModel(actorID);
                    return;
                }
                AnimScriptEvent animScriptEvent = { actorID, scriptCtx.scriptPtr, &scriptCtx, scriptCtx.currentOpCode, getBattleModelState(actorID), ptrToScriptTable };
                /*if (!gContext.animScriptOpcodes.contains(assembleOpCodeKey(scriptCtx.currentOpCode))) {
                    srLogWrite("ERROR: Trying to execute invalid opcode %x for actor %d", scriptCtx.currentOpCode, actorID);
                    continue;
                }*/
                //auto opcode = gContext.animScriptOpcodes.getElement(assembleOpCodeKey(scriptCtx.currentOpCode));
                auto opcode = gContext.animScriptOpcodes.getResource(scriptCtx.currentOpCode - 0x8E);

                if (ownerModelState.animScriptIndex != 0) {
                    srLogWrite("executing opcode %x", scriptCtx.currentOpCode);
                }
                gContext.eventBus.dispatch(RUN_ANIM_SCRIPT_OPCODE, &animScriptEvent);
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
                    if (ownerModelState.animScriptIndex != 0) {
                        srLogWrite("playing model animation from control sequence %d", ownerModelState.runningAnimIdx);
                    }
                    srHandleAnimateModel(actorID);
                    return;
                    break;
                }
                default: {
                    ownerModelState.runningAnimIdx = scriptCtx.currentOpCode;
                    ownerModelState.field_74 = 0;
                    scriptOwner74State.field_36 = 0;
                    scriptOwnerRotationData.field_0 = 0;
                    scriptCtx.isScriptActive = 0;
                    break;
                }
                }
            }
        }
        if (ownerModelState.animScriptIndex != 0) {
            srLogWrite("playing model animation %d", ownerModelState.runningAnimIdx);
        }
        srHandleAnimateModel(actorID);
    }
}
