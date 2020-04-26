#include "summon_effects.h"
#include "../battle_utils.h"
#include "../battle_models/battle_model_state_interface.h"
#include "../battle_models/battle_models_api.h"
#include "../battle_models/battle_models.h"
#include "../battle_models/model_names.h"
#include "../../impl.h"



void summonEffectMainDispatcher(u16 actionTargetMask, u8 casterIdx) {
    srPushEffect100(summonMainEffect);
}

typedef void(*SRPFNSUB_SETSUMMONMODEL)(BattleModel*, void*);
#define setSummonModel           ((SRPFNSUB_SETSUMMONMODEL)0x429398)
#define sub_429322               ((PFNSR_VOIDSUB)0x429322)
#define convertSummonBData       ((PFNSR_VOIDSUB)0x5C1DF0)
#define sub_748F8F               ((PFNSR_INTSUB)0x748F8f)
void summonMainEffect() {
    u8* byte_BFB2EC = (u8*)0xBFB2EC;
    u8* byte_BE2C89 = (u8*)0xBE2C89;
    u8* byte_BE4775 = (u8*)0xBE4775;
    u8* byte_BE6261 = (u8*)0xBE6261;
    u8* byte_BF1EB0 = (u8*)0xBF1EB0;
    u8* G_SCRIPT_WAIT_FRAMES = (u8*)0xBFD0F0;
    u32* dword_7C10D8 = (u32*)0x7C10D8;
    u32** dword_9ADFD8 = (u32**)0x9ADFD8;

    auto summonEffecCtx = getEffect100QueueTop();
    u16 currentFrame = summonEffecCtx->wordArray[0];

    *byte_BF1EB0 = 0;
    switch (currentFrame) {
    case 0: {
        ++summonEffecCtx->wordArray[0];
        summonEffecCtx->wordArray[1] = 14;
        *G_SCRIPT_WAIT_FRAMES = 14;
        sub_748F8F(41);
        u8 caster = getActionActorIdx();
        if (caster < 3) {
            for (u8 partyIdx = 0; partyIdx < 3; ++partyIdx)
                setModelVanish(partyIdx);
        }
        else {
            for (u8 enemyIdx = 4; enemyIdx < 10; ++enemyIdx)
                setModelVanish(enemyIdx);
        }
        if (getBattleType() == 4) {
            if (caster < 3) {
                for (u8 partyIdx = 0; partyIdx < 3; ++partyIdx)
                    setModelVanish(partyIdx);
            }
            else {
                for (u8 enemyIdx = 4; enemyIdx < 10; ++enemyIdx)
                    setModelVanish(enemyIdx);
            }
        }
        break;
    }
    case 15: {
        //gBigAnimBlock[0].field_25 |= 2u;
        *byte_BFB2EC = 1;
        *byte_BE2C89 |= 2u;
        *byte_BE4775 |= 2u;
        ++summonEffecCtx->wordArray[0];
        if (!*dword_7C10D8)
            sub_429322();
        auto spellEffectIdx = getBattleModelState(getActionActorIdx())->animationEffect;
        auto modelName = getSummonModelNameFromEffectIdx(spellEffectIdx);
        std::vector<char> chars(modelName.begin(), modelName.end());
        char* cModelName = &(chars[0]);
        auto summonModel = srSetSummonModel(6, cModelName);
        srLogWrite("LOADED IFRIT SUMMON MODEL AT: %p", summonModel);
        setSummonModel(summonModel, *dword_9ADFD8);
        pushSummonEffectCallback(spellEffectIdx, getAnimatingActionTargetMask(), getActionActorIdx());
        convertSummonBData();
        *byte_BE6261 |= 0x10u;
        summonEffecCtx->handlerIndex = 0xFFFF;
        break;
    }
    }
    ++summonEffecCtx->wordArray[0];
}

void pushSummonEffectCallback(u8 animationEffectIdx, u16 targetMask, u8 actorIdx) {
    SRPFNSPELLEFFECTCALLBACK callback = nullptr;
    switch (animationEffectIdx) {
    case 0: {
        break;
    }
    case 1: {
        break;
    }
    case 2: {
        prepareIfrit(targetMask, actorIdx);
        break;
    }
    }
}


#define gameLoadIfrit   ((PFNSR_VOIDSUB)0x592720)
void prepareIfrit(u16 actioNTargetMask, u8 casterIdx) {
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
    G_IFRIT_MAT_BCC708->Colummn4[1] = -1000;

    setPoint(G_POINT_BCC760, 0, 0, 0);
    G_POINT_BCC6F0->x = 1024;
    G_POINT_BCC6F0->y = 1024;
    G_POINT_BCC6F0->z = 256;
    setPoint(G_POINT_BCC6B8, 256, 2048, 0);

    nullGameMatrix(G_IFRIT_MAT_BCBFA0);
    setDiagonal(G_IFRIT_MAT_BCBFA0, 1024, 1024, 1024);
    G_IFRIT_MAT_BCBFA0->Colummn4[0] = 0;
    G_IFRIT_MAT_BCBFA0->Colummn4[1] = -2500;
    G_IFRIT_MAT_BCBFA0->Colummn4[2] = 1500;

    *qword_BCBFB6 = 6446745908796LL;
    gameLoadIfrit();
    setupIfritMain(actioNTargetMask, casterIdx);
    srLogWrite("DEBUG: IFRIT EFFECT PREPARED");
}


#define gameIfritMain      ((PFNSR_VOIDSUB)0x593A95)

typedef R3PointWord*(*SRPFN_SUB661000)(u16);
#define gameGet3DPoint  ((SRPFN_SUB661000)0x661000)

typedef void(*SRPFN_SUB662AD8)(R3PointWord*, GameRotationMatrix*);
#define gameRotateMatrix      ((SRPFN_SUB662AD8)0x662AD8)

typedef void(*SRPFN_SUB5929F6)(void*, GameRotationMatrix*, u8);
#define pushIfritMoveHandler            ((SRPFN_SUB5929F6)0x5929F6)
void setupIfritMain(u16 actionTargetMask, u8 casterIdx) {
    R3PointWord* G_POINT_BCC790 = (R3PointWord*)0xBCC790;
    u8* byte_BFCB64 = (u8*)0xBFCB64;
    u32* dword_BCC6B0 = (u32*)0xBCC6B0;
    auto point = gameGet3DPoint(0);
    GameRotationMatrix* G_IFRIT_MAT_BCC768 = (GameRotationMatrix*)0xBCC768;
    G_IFRIT_MAT_BCC768->Colummn4[0] = 0;
    G_IFRIT_MAT_BCC768->Colummn4[1] = 0;

    *dword_BCC6B0 = actionTargetMask;
    computeTargetsCentroid(actionTargetMask, point);
    if (point->z >= getBattleModelState(casterIdx)->restingPosition.z) {
        G_IFRIT_MAT_BCC768->Colummn4[2] = point->z - 5000;
    }
    else {
        G_IFRIT_MAT_BCC768->Colummn4[2] = point->z + 5000;
        G_POINT_BCC790->y = 2048;
    }
    gameRotateMatrix(G_POINT_BCC790, G_IFRIT_MAT_BCC768);
    srPushEffect100(srIfritMain);

    void* unk_8C00C0 = (void*)0x8C00C0;
    void* unk_8C01A8 = (void*)0x8C01A8;
    if (rand() & 0x100)
        pushIfritMoveHandler(unk_8C00C0, G_IFRIT_MAT_BCC768, casterIdx);
    else
        pushIfritMoveHandler(unk_8C01A8, G_IFRIT_MAT_BCC768, casterIdx);

    *byte_BFCB64 |= 8u;
}

std::string getSummonModelNameFromEffectIdx(u8 animationEffectIdx) {
    switch (animationEffectIdx) {
    case 0: {
        return BattleModelNames::CHOCOMOG;
        break;
    }
    case 1: {
        return BattleModelNames::SHIVA;
        break;
    }
    case 2: {
        return BattleModelNames::IFRIT;
        break;
    }
    }
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
#define sub_596702     ((PFNSREFFECTCALLBACK)0x596702)

void srIfritMain() {
    u8* byte_BCC6A4 = (u8*)0xBCC6A4;
    R3PointWord* dword_BCC700 = (R3PointWord*)0xBCC700;
    u32* dword_C05EC8 = (u32*)0xC05EC8;
    u8* byte_C05EC8 = (u8*)0xC05EC8;
    u16* word_C05EC8 = (u16*)0xC05EC8;
    R3PointWord* G_POINT_BCC790 = (R3PointWord*)0xBCC790;

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

    srLogWrite("Debug: In Ifrit Effect Main: Current Frame: %i", ifritEffectCtx->wordArray[0]);
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
        srLogWrite("Pushing frame 4 Ifrit Effects");
        srPushEffect100(sub_5940B2);
        sub_5BFFF3(34, 64, 0);
        break;
    }
    case 14: {
        srLogWrite("Pushing frame 14 Ifrit Effects");
        auto gameRotationMatrix = setAnimationTransitionState(1);
        gameRotationMatrix->R3Submatrix[0][2] = G_IFRIT_MAT_BCC768->Colummn4[0];
        gameRotationMatrix->R3Submatrix[1][0] = G_IFRIT_MAT_BCC768->Colummn4[1];
        gameRotationMatrix->R3Submatrix[1][1] = G_IFRIT_MAT_BCC768->Colummn4[2];
        gameRotationMatrix->R3Submatrix[2][2] = G_POINT_BCC790->y ^ 0x800;
        srPushEffect100(sub_594C07);
        srPushEffect100(sub_594D82);
        srPushEffect100(sub_596702);
        break;
    }
    case 44: {
        setAnimationTransitionState(2);
        ifritEffectCtx->handlerIndex = 0xFFFF;
        //byte_BF1EB0 = 1;
        break;
    }
    }
    ++ifritEffectCtx->wordArray[0];
}
