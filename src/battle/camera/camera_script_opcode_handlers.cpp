#include "camera_script_opcode_handlers.h"
#include "../battle_actor_interface.h"
#include "../battle_models/battle_model_state_interface.h"

const static R3PointWord* GLOBAL_CAMERA_POSITION = (R3PointWord*)0xBF2158;
const static R3PointWord* GLOBAL_CAMERA_LOOKAT = (R3PointWord*)0xBFB1A0;

u8 readOpCodeArg8(const u8* scriptPtr, ScriptContext* ctx, CamPointContext* camScriptCtx) {
    auto argPosition = scriptPtr + camScriptCtx->currentPosition;
    auto byteReader = (u8*)argPosition;
    auto arg = *byteReader;
    camScriptCtx->currentPosition += 1;
    return arg;
}

u16 readOpCodeArg16(const u8* scriptPtr, ScriptContext* ctx, CamPointContext* camScriptCtx) {
    auto argPosition = scriptPtr + camScriptCtx->currentPosition;
    auto wordReader = (u16*)argPosition;
    auto arg = *wordReader;
    camScriptCtx->currentPosition += 2;
    return arg;
}

u32 readOpCodeArg32(const u8* scriptPtr, ScriptContext* ctx, CamPointContext* camScriptCtx) {
    auto argPosition = scriptPtr + camScriptCtx->currentPosition;
    auto wordReader = (u32*)argPosition;
    auto arg = *wordReader;
    camScriptCtx->currentPosition += 4;
    return arg;
}

//Dispatches a Sound on Channel 1 based on a 16-bit argument read from the script
OpCodeControlSequence pOpCodeD5(CamScriptEvent* srEvent) {
    auto& gameGlobals = *srEvent->gameGlobals;
    gameGlobals.arg1 = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptCtx, srEvent->camScriptCtx);
    return RUN_NEXT;
}

OpCodeControlSequence pOpCodeD6(CamScriptEvent* srEvent) {
    u8* G_DAMAGE_DISPLAY_TARGET_IDS = (u8*)0xC05E68;
    auto& gameGlobals = *srEvent->gameGlobals;
    gameGlobals.arg8 = G_DAMAGE_DISPLAY_TARGET_IDS[readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptCtx, srEvent->camScriptCtx)];
    gameGlobals.arg6 = getBattleModelState(gameGlobals.arg8)->bData0x12[readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptCtx, srEvent->camScriptCtx)];
    return RUN_NEXT;
}

OpCodeControlSequence pOpCodeD7(CamScriptEvent* srEvent) {
    auto& gameGlobals = *srEvent->gameGlobals;
    gameGlobals.arg1 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptCtx, srEvent->camScriptCtx);
    gameGlobals.arg2 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptCtx, srEvent->camScriptCtx);
    auto& camCtx = *srEvent->camScriptCtx;
    return RUN_NEXT;
}

OpCodeControlSequence pOpCodeD8(CamScriptEvent* srEvent) {
    auto& camCtx = *srEvent->camScriptCtx;
    return RUN_NEXT;
}

//Copy the Global Camera Position to the camera event context
OpCodeControlSequence pOpCodeD9(CamScriptEvent* srEvent) {
    auto& camCtx = *srEvent->camScriptCtx;
    camCtx.point = *GLOBAL_CAMERA_POSITION;
    return RUN_NEXT;
}

OpCodeControlSequence pOpCodeDA(CamScriptEvent* srEvent) {
    auto& camCtx = *srEvent->camScriptCtx;
    camCtx.field_D = 0;
    return RUN_NEXT;
}


OpCodeControlSequence pOpCodeDB(CamScriptEvent* srEvent) {
    auto& camCtx = *srEvent->camScriptCtx;
    camCtx.field_D = 1;
    return RUN_NEXT;
}


OpCodeControlSequence pOpCodeDC(CamScriptEvent* srEvent) {
    auto& camCtx = *srEvent->camScriptCtx;
    camCtx.field_C = 1;
    return RUN_NEXT;
}

// Handles decrementing the wait frames before continuing the script
OpCodeControlSequence pOpCodeF4(CamScriptEvent* srEvent) {
    auto& camCtx = *srEvent->camScriptCtx;
    if (camCtx.framesToWait) {
        --camCtx.framesToWait;
        --camCtx.currentPosition;
        srEvent->scriptCtx->isScriptActive = 0;
    }
    return BREAK;
}

//Sets frames to wait before executing next script command, needs to be modified for 60 fps
OpCodeControlSequence pOpCodeF5(CamScriptEvent* srEvent) {
    auto& camCtx = *srEvent->camScriptCtx;
    srEvent->camScriptCtx->framesToWait = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptCtx, srEvent->camScriptCtx);
    return RUN_NEXT;
}

//Sets the camera Ctx position from read script values
OpCodeControlSequence pOpCodeF9(CamScriptEvent* srEvent) {
    auto& camCtx = *srEvent->camScriptCtx;
    camCtx.point.x = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptCtx, srEvent->camScriptCtx);
    camCtx.point.y = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptCtx, srEvent->camScriptCtx);
    camCtx.point.z = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptCtx, srEvent->camScriptCtx);
    srEvent->camScriptCtx->framesToWait = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptCtx, srEvent->camScriptCtx);
    return RUN_NEXT;
}
