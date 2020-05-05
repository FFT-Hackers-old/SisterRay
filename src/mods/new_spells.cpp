#include "new_spells.h"
#include "../battle/engine/action_spell_effects.h"
#include "../battle/battle_utils.h"
#include "../impl.h"
#include "../battle/engine/action_effects.h"
#include "../battle/battle_models/battle_model_state_interface.h"
#include "../gamedata/element_names.h"
#include "../gamedata/damage_callback_utils.h"

#define SPELL_MOD_NAME "SR_SPELLS"
void registerEruption() {
    auto eruption = SrActionData();
    eruption.attackName = "Eruption";
    eruption.attackDesc = "Ground erupts beneath the enemy party";
    eruption.baseData.attackPower = 56;
    eruption.baseData.additionalEffect = 0xFF;
    eruption.baseData.additionalEffectModifier = 0xFF;
    eruption.baseData.animationEffectID = 0xFF;
    eruption.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES | TGT_FLAG_START_MULTIPLE;
    eruption.baseData.abilityHitRate = 0x64;
    eruption.baseData.cameraMovementSingle = 0x0024;
    eruption.baseData.cameraMovementMultiple = 0x0024;
    eruption.baseData.impactEffectID = 0xFF;
    eruption.baseData.impactSoundID = 0xFFFF;
    eruption.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(eruption, 0, SPELL_MOD_NAME);
    addElementToAction(SPELL_MOD_NAME, 0, ElementNames::FIRE.c_str());
    addElementToAction(SPELL_MOD_NAME, 0, ElementNames::EARTH.c_str());
    addActionToCommand(BASE_PREFIX, 2, SPELL_MOD_NAME, 0);
    setEffectCallback(SPELL_MOD_NAME, 0, (SRPFNSPELLEFFECTCALLBACK)prepareEruption);
}

#define gameLoadIfrit   ((PFNSR_VOIDSUB)0x592720)
void prepareEruption(u16 actioNTargetMask, u8 casterIdx) {
    R3PointWord* G_POINT_BCC790 = (R3PointWord*)0xBCC790;
    R3PointWord* G_POINT_BCC760 = (R3PointWord*)0xBCC760;
    R3PointWord* G_POINT_BCC6B8 = (R3PointWord*)0xBCC6B8;
    R3PointDWord* G_POINT_BCC6F0 = (R3PointDWord*)0xBCC6F0;
    GameRotationMatrix* G_IFRIT_MAT_BCC708 = (GameRotationMatrix*)0xBCC708;
    GameRotationMatrix* G_IFRIT_MAT_BCBFA0 = (GameRotationMatrix*)0xBCBFA0;
    void* unk_8B86A8 = (void*)0x8B86A8;
    void* unk_8B99B0 = (void*)0x8B99B0;
    i64* qword_BCBFB6 = (i64*)0xBCBFB6;

    memcpy(unk_8B86A8, unk_8B99B0, 0x1308u);

    setPoint(G_POINT_BCC790, 0, 0, 0);
    nullGameMatrix(G_IFRIT_MAT_BCC708);
    setDiagonal(G_IFRIT_MAT_BCC708, -768, 768, -768);
    G_IFRIT_MAT_BCC708->position[1] = -1000;

    setPoint(G_POINT_BCC760, 0, 0, 0);
    G_POINT_BCC6F0->x = 1024;
    G_POINT_BCC6F0->y = 1024;
    G_POINT_BCC6F0->z = 256;
    setPoint(G_POINT_BCC6B8, 256, 2048, 0);

    nullGameMatrix(G_IFRIT_MAT_BCBFA0);
    setDiagonal(G_IFRIT_MAT_BCBFA0, 1024, 1024, 1024);
    G_IFRIT_MAT_BCBFA0->position[0] = 0;
    G_IFRIT_MAT_BCBFA0->position[1] = -2500;
    G_IFRIT_MAT_BCBFA0->position[2] = 1500;

    *qword_BCBFB6 = 6446745908796LL;
    gameLoadIfrit();
    setupEruptionMain(actioNTargetMask, casterIdx);
}


#define gameIfritMain      ((PFNSR_VOIDSUB)0x593A95)

typedef R3PointWord* (*SRPFN_SUB661000)(u16);
#define gameGet3DPoint  ((SRPFN_SUB661000)0x661000)

typedef void(*SRPFN_SUB662AD8)(R3PointWord*, GameRotationMatrix*);
#define gameRotateMatrix      ((SRPFN_SUB662AD8)0x662AD8)

typedef void(*SRPFN_SUB5929F6)(void*, GameRotationMatrix*, u8);
#define pushIfritCameraHandler            ((SRPFN_SUB5929F6)0x5929F6)
void setupEruptionMain(u16 actionTargetMask, u8 casterIdx) {
    R3PointWord* G_POINT_BCC790 = (R3PointWord*)0xBCC790;
    u8* byte_BFCB64 = (u8*)0xBFCB64;
    u16* EFFECT_TARGET_MASK = (u16*)0xBCC6B0;
    auto point = gameGet3DPoint(0);
    GameRotationMatrix* G_IFRIT_MAT_BCC768 = (GameRotationMatrix*)0xBCC768;
    G_IFRIT_MAT_BCC768->position[0] = 0;
    G_IFRIT_MAT_BCC768->position[1] = 0;

    srLogWrite("SPELL EFFECT ERUPTION TARGET MASK: %x", actionTargetMask);
    *EFFECT_TARGET_MASK = actionTargetMask;
    srLogWrite("computing target Mask Centroid for mask %x", *EFFECT_TARGET_MASK);
    computeTargetsCentroid(actionTargetMask, point);
    G_IFRIT_MAT_BCC768->position[2] = point->z - 600;
    gameRotateMatrix(G_POINT_BCC790, G_IFRIT_MAT_BCC768);
    srPushEffect100(srEruptionMain);
    *byte_BFCB64 |= 8u;
}


typedef void (*SRPFN_WORDMATMULTIPLY)(GameRotationMatrix*, GameRotationMatrix*, GameRotationMatrix*);
#define wordMatrixProduct  ((SRPFN_WORDMATMULTIPLY)0x661E85)

typedef void (*SRPFN_PUSHSOMEIFRITTHING)(u32, u32);
#define sub_5BFC65     ((SRPFN_PUSHSOMEIFRITTHING)0x5BFC65)

typedef void (*SRPFN_SUB5BFFF3)(u32, u8, u32);
#define sub_5BFFF3     ((SRPFN_SUB5BFFF3)0x5BFFF3)

#define sub_5940B2     ((PFNSREFFECTCALLBACK)0x5940B2)
#define sub_594C07     ((PFNSREFFECTCALLBACK)0x594C07)
#define sub_594D82     ((PFNSREFFECTCALLBACK)0x594D82)
#define moveIfritEntrance     ((PFNSREFFECTCALLBACK)0x596702)
#define sub_595A05     ((PFNSREFFECTCALLBACK)0x595A05)
#define sub_595791     ((PFNSREFFECTCALLBACK)0x595791)
void srEruptionMain() {
    u8* byte_BCC6A4 = (u8*)0xBCC6A4;
    R3PointWord* dword_BCC700 = (R3PointWord*)0xBCC700;
    u32* dword_C05EC8 = (u32*)0xC05EC8;
    u8* byte_C05EC8 = (u8*)0xC05EC8;
    u16* word_C05EC8 = (u16*)0xC05EC8;
    R3PointWord* G_POINT_BCC790 = (R3PointWord*)0xBCC790;
    u16* EFFECT_TARGET_MASK = (u16*)0xBCC6B0;

    GameRotationMatrix* G_IFRIT_MAT_BCC768 = (GameRotationMatrix*)0xBCC768;
    GameRotationMatrix* mat1 = (GameRotationMatrix*)0xBFCB30;
    GameRotationMatrix* ret = (GameRotationMatrix*)0xBCC6C8;

    if (!(*byte_BCC6A4 & 1)) {
        *byte_BCC6A4 |= 1u;
        dword_BCC700 = gameGet3DPoint(0);
    }
    wordMatrixProduct(mat1, G_IFRIT_MAT_BCC768, ret);

    auto ifritEffectCtx = getEffect100QueueTop();
    u16 currentFrame = ifritEffectCtx->wordArray[0];

    srLogWrite("Debug: In Volcano Effect Main: Current Frame: %i", ifritEffectCtx->wordArray[0]);
    if (*BATTLE_PAUSED_GLOBAL)
        return;

    switch (currentFrame) {
    case 0: {
        sub_5BFC65(1024, 4); //Pushes an ifrit Intro Effect, stores ptr to effect ctx globally
        word_C05EC8[1] = 128;
        byte_C05EC8[1] = 0;
        break;
    }
    case 4: {
        srLogWrite("Pushing frame 4 Volcano Effects");
        srPushEffect100(sub_5940B2);
        sub_5BFFF3(34, 64, 0);
        break;
    }
    case 14: {
        srLogWrite("Pushing frame 14 Volcano Effects");
        srPushEffect100(sub_594C07);
        srPushEffect100(sub_594D82);
        srPushEffect100(moveIfritEntrance);
        break;
    }
    case 43: {
        srPushEffect100(sub_595A05);
        //srPushEffect100(sub_595791);
        break;
    }
    case 47: {
        srDisplayDamageEvents(*EFFECT_TARGET_MASK);
        break;
    }
    case 75: {
        ifritEffectCtx->handlerIndex = 0xFFFF;
        flushSrEffects100();
    }
    }
    ++ifritEffectCtx->wordArray[0];
}
