#include "rendering.h"
#include "../camera/camera.h"
#include "../effects/action_effects.h"
#include "../battle_models/battle_models_api.h"
#include "../battle_utils.h"
#include "../../impl.h"

void srDispatchGeneralActionEffects(bool isEnabled) {
    PFNSR_INTSUB sub_42DAE5 = (PFNSR_INTSUB)0x42DAE5;
    sub_42DAE5(isEnabled);
}

void srRenderSpecial() {
    PFNSR_VOIDSUB sub_42E52E = (PFNSR_VOIDSUB)0x42E52E;
    sub_42E52E();
}

void setBlendColor(u8 r, u8 g, u8 b) {

}

bool checkShouldAdvanceCamera() {
    typedef bool(*PFNSR_RETSUB)();
    PFNSR_RETSUB sub_66C3BF = (PFNSR_RETSUB)0x66C3BF;
    return sub_66C3BF();
}

void runPartyAnimationScripts() {

}

void runEnemyAnimationScripts() {

}

void drawEnemyActors() {

}

void drawBattleActors() {

}

void syncActorIdles() {

}

void animateAllActors(bool doBlend) {

}

void animateSpellActors(bool doBlend) {

}

void pushAuxillaryEffects() {

}

void releaseEffectThreads() {

}

void srDispatchEffects() {
    u32* G_DO_UPDATE_RENDER_STATE = (u32*)0x9AD1AC;
    if (*G_DO_UPDATE_RENDER_STATE) {
        dispatchEffects60();
    }
    dispatchEffects100();
}

void srAnimateCamera() {
    srRunCameraScripts(getCameraAction(), getBattleModelState(getActionActorIdx())->animationEffect);
    if (checkShouldAdvanceCamera())
        srAdvanceBattleCamera(getCameraAction());
}


void srRenderStateUpdate() {
    u32* G_DO_UPDATE_RENDER_STATE = (u32*)0x9AD1AC;
    u8* byte_BFCDFC = (u8*)0xBFCDFC;
    u8* byte_BFD0F4 = (u8*)0xBFD0F4;
    u16* word_BF2154 = (u16*)0xBF2154;
    u8* G_ENABLE_GEN_DISPATCH = (u8*)0xBFD094;
    u16* G_NOT_DEFEATED_ACTORS = (u16*)0x9A8896;

    srRenderSpecial();
    if (*byte_BFD0F4)
        --*byte_BFD0F4;
    switch (*byte_BFCDFC) {
    case 0:
    case 1:
    case 6:
        break;
    case 2: {
        if (*G_DO_UPDATE_RENDER_STATE) {
            runEnemyAnimationScripts();
            dispatchEffects10();
        }
        drawEnemyActors();
        animateAllActors(false);
        break;
    }
    default: {
        if (*G_DO_UPDATE_RENDER_STATE) {
            runPartyAnimationScripts();
            runEnemyAnimationScripts();
            srDispatchGeneralActionEffects((bool)*G_ENABLE_GEN_DISPATCH);
            dispatchEffects10();
        }
        releaseEffectThreads();
        drawBattleActors();
        animateAllActors(false);
        pushAuxillaryEffects();
        break;
    }
    }
    releaseEffectThreads();
    *word_BF2154 = *G_NOT_DEFEATED_ACTORS;
    srHandleSpecialCamera();
    if (*G_DO_UPDATE_RENDER_STATE) {
        srAnimateCamera();
    }
    syncActorIdles();
    animateSpellActors(false);
    setBlendColor(0, 0, 0);
    srDispatchEffects();
}
