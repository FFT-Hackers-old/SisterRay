#include "animation_script_parser.h"
#include "../battle_models/battle_model_state_interface.h"
#include "../../impl.h"

void runAnimationScript(u16 actorID, void** ptrToScriptTable) {
    GameAnimationScriptContext** ptrToScriptContext = (GameAnimationScriptContext**)0x8FE2AC;
    auto scriptOwnerModelState = getBattleModelState(actorID);
    auto scriptOwner74State = getBattleModelState74(actorID);

    if (!*BATTLE_PAUSED_GLOBAL) {
        GameAnimationScriptContext* scriptContextPtr = *ptrToScriptContext;
        byte_9ADEF8 = 0;
        scriptContextPtr->isScriptActive = 1;
        scriptContextPtr->scriptPtr = (u8*)ptrToScriptTable[scriptOwnerModelState->animScriptIndex];
        switch (scriptOwnerModelState->animScriptIndex) {
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
        case 57:
            BATTLE_MODEL_STATE_BIG_ARRAY[actor_id].field_25 |= 0x80u;
            scriptContextPtr->scriptPtr = unk_7C1110;
            break;
        case 58:
            scriptContextPtr->scriptPtr = unk_7C1108;
            break;
        case 59:
            scriptContextPtr->scriptPtr = unk_7C110C;
            break;
        default:
            break;
        }

        if (scriptOwnerModelState.modelEffectFlags & 1) {
            resetScriptState(actor_id);
            scriptOwnerModelState.modelEffectFlags &= 0xFEu;
        }
        if (scriptOwnerModelState.isScriptExecuting) {
            scriptOwnerModelState.playedAnimFrames = 0;
            while (scriptContextPtr->isScriptActive) {
                scriptContextPtr->currentOpCode = scriptContextPtr->scriptPtr[scriptOwnerModelState.currentScriptPosition++];
                AnimScriptEvent animScriptEvent = { scriptContextPtr, scriptPtr, scriptContextPtr->currentOpCode, BATTLE_MODEL_STATE_BIG_ARRAY };
                auto opcode = gContext.animScriptOpcodes.get_element(assembleOpCodeKey(scriptContextPtr->currentOpCode));
                auto control = opcode(animScriptEvent);
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
                    scriptOwnerModelState->runningAnimIdx = scriptContextPtr->currentOpCode;
                    scriptOwnerModelState.field_74 = 0;
                    getBattleModelState74(actorID)->field_36 = 0;
                    dword_BE0E28[16 * actor_id] = 0;
                    scriptContextPtr->isScriptActive = 0;
                    break;
                }
                }
            }
        }
    }
}

void srPlayAnimation(u8 actorID) {
    auto scriptOwnerModelState = getBattleModelState(actorID);
    auto scriptOwner74State = getBattleModelState74(actorID);
    if (!(scriptOwner74State.field_C & 2)) {
        scriptOwner74State->field_36++;
        if (scriptOwnerModelState->animScriptIndex == actorIdleAnimScripts[actor_id])// probably handles idle animations
        {
            auto unkModeToggle = scriptOwner74State.field_33;
            if (unkModeToggle == 1) {
                playModelAnimation(actor_id);
                playModelAnimation(actor_id);
            }
            else if (unkModeTogle == 2) {
                if (byte_BFD0E4 & 1)
                    playModelAnimation(actor_id);
            }
            else {
                if (unkModeToggle == 3)
                {
                    scriptOwnerModelState->field_74 = 0;
                    dword_BE0E28[16 * actor_id] = 0;
                    byte_9ADEF8 = 1;
                }
                playModelAnimation(actor_id);
            }
        }
        else
        {
            playModelAnimation(actor_id);
        }
    }
}
