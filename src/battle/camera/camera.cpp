#include "camera.h"
#include "../../impl.h"

typedef struct {
    u8 isScriptActive;
    u8 currentOpCode;
    u8* scriptPtr;
    u32 scriptPosition;
} SrScriptContext;

auto G_POSITION_CTX = SrScriptContext();
auto G_LOOKAT_CTX = SrScriptContext();

//Runs a Camera Script
void srRunCameraScripts(u32 cameraScriptIdx, u32 spellEffectCtx) {
    auto& posCtx = G_POSITION_CTX;
    auto& lookatCtx = G_LOOKAT_CTX;
    posCtx.isScriptActive = 1;
    runPosCamScript(posCtx7);
    runLookAtCamScript(lookatCtx);
}

void runPosCamScript(SrScriptContext& posCtx) {
    while (posCtx.isScriptActive) {
        srLogWrite("selecting opcode at position: %i", ownerModelState.currentScriptPosition);
        posCtx.currentOpCode = posCtx.scriptPtr[posCtx.scriptPosition++];
        srLogWrite("chosen opcode: %x, new script position: %i", posCtx.currentOpCode, ownerModelState.currentScriptPosition);
        CamScriptEvent posScriptEvent = { actorID, scriptCtx.scriptPtr, &scriptCtx, scriptCtx.currentOpCode, getBattleModelState(actorID), ptrToScriptTable };
        /*if (!gContext.animScriptOpcodes.contains(assembleOpCodeKey(scriptCtx.currentOpCode))) {
            srLogWrite("ERROR: Trying to execute invalid opcode %x for actor %d", scriptCtx.currentOpCode, actorID);
            continue;
        }*/
        //auto opcode = gContext.animScriptOpcodes.getElement(assembleOpCodeKey(scriptCtx.currentOpCode));
        auto opcode = gContext.animScriptOpcodes.getResource(posCtx.currentOpCode - 0x8E);

        if (ownerModelState.animScriptIndex != 0) {
            srLogWrite("executing opcode %x", posCtx.currentOpCode);
        }
        gContext.eventBus.dispatch(RUN_CAMERA_POSITION_SCRIPT_OPCODE, &posScriptEvent);
        auto control = opcode(&posScriptEvent);
        switch (control) {
        case RUN_NEXT: {
            continue;
            break;
        }
        case BREAK: {
            return;
            break;
        }
        }
    }

}

void runLookAtCamScript(SrScriptContext& lookAtCtx) {
    while (lookAtCtx.isScriptActive) {
        lookAtCtx.currentOpCode = lookAtCtx.scriptPtr[lookAtCtx.scriptPosition++];
        srLogWrite("chosen opcode: %x, new script position: %i", lookAtCtx.currentOpCode, ownerModelState.currentScriptPosition);
        CamScriptEvent posScriptEvent = { actorID, scriptCtx.scriptPtr, &scriptCtx, scriptCtx.currentOpCode, getBattleModelState(actorID), ptrToScriptTable };
        /*if (!gContext.animScriptOpcodes.contains(assembleOpCodeKey(scriptCtx.currentOpCode))) {
            srLogWrite("ERROR: Trying to execute invalid opcode %x for actor %d", scriptCtx.currentOpCode, actorID);
            continue;
        }*/
        //auto opcode = gContext.animScriptOpcodes.getElement(assembleOpCodeKey(scriptCtx.currentOpCode));
        auto opcode = gContext.animScriptOpcodes.getResource(lookAtCtx.currentOpCode - 0x8E);

        if (ownerModelState.animScriptIndex != 0) {
            srLogWrite("executing opcode %x", lookAtCtx.currentOpCode);
        }
        gContext.eventBus.dispatch(RUN_CAMERA_POSITION_SCRIPT_OPCODE, &posScriptEvent);
        auto control = opcode(&posScriptEvent);
        switch (control) {
        case RUN_NEXT: {
            continue;
            break;
        }
        case BREAK: {
            return;
            break;
        }
        }
    }
}

void srAdvanceBattleCamera(u32 cameraScriptIdx) {
    PFNSR_WORDSUB gameAdvanceBattleCamera = (PFNSR_WORDSUB)0x5C20CE;
    gameAdvanceBattleCamera(cameraScriptIdx);
}

void srHandleSpecialCamera() {

}

u16 getCameraAction() {
    u16* G_CAMERA_ACTION = (u16*)0xC05EB8;
    return *G_CAMERA_ACTION;
}
