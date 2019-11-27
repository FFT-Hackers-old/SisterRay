#include "animation_script_parser.h"
#include "../../impl.h"

void __cdecl runAnimationScript(__int16 actor_id, void** ptrToScriptTable) {
    GameAnimationScriptContext** ptrToScriptContext = (GameAnimationScriptContext**)0x8FE2AC;

    if (!*BATTLE_PAUSED_GLOBAL) {
        GameAnimationScriptContext* scriptContextPtr = *ptrToScriptContext;
        byte_9ADEF8 = 0;
        scriptContextPtr->isScriptActive = 1;    
        scriptContextPtr->scriptPtr = (u8*)ptrToScriptTable[gBigAnimBlock[actor_id].animScriptIndex];
        switch (gBigAnimBlock[actor_id].animScriptIndex) {
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
            gBigAnimBlock[actor_id].field_25 |= 0x80u;
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

        if (gBigAnimBlock[actor_id].modelEffectFlags & 1) {
            resetScriptState(actor_id);
            gBigAnimBlock[actor_id].modelEffectFlags &= 0xFEu;
        }
        if (gBigAnimBlock[actor_id].isScriptExecuting) {
            gBigAnimBlock[actor_id].playedAnimFrames = 0;
        LABEL_20:
            while (scriptContextPtr->isScriptActive) {
                scriptContextPtr->currentOpCode = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
                AnimScriptEvent animScriptEvent = {scriptContextPtr, scriptPtr, scriptContextPtr->currentOpCode, gBigAnimBlock};
                auto opcode = gContext.animScriptOpcodes.get_element(assembleOpCodeKey(scriptContextPtr->currentOpCode));
                auto control = opcode(animScriptEvent);
               
        if (!(smallModelDataArray[actor_id].field_C & 2))
        {
            ++smallModelDataArray[actor_id].field_36;
            if (gBigAnimBlock[actor_id].animScriptIndex == actorIdleAnimScripts[actor_id])// probably handles idle animations
            {
                v67 = smallModelDataArray[actor_id].field_33;
                if (v67 == 1)
                {
                    playModelAnimation(actor_id);
                    playModelAnimation(actor_id);
                }
                else if (v67 == 2)
                {
                    if (byte_BFD0E4 & 1)
                        playModelAnimation(actor_id);
                }
                else
                {
                    if (v67 == 3)
                    {
                        *(_DWORD*)&gBigAnimBlock[actor_id].field_74 = 0;
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
}
