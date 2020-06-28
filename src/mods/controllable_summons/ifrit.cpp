#include "ifrit.h"
#include "../../battle/battle_engine_api.h"
#include "../../events/event_bus_interface.h"
#include "../../impl.h"

#define gameLoadIfrit   ((PFNSR_VOIDSUB)0x592720)
void prepareIfrit(u16 actioNTargetMask, u8 casterIdx) {
    R3PointWord* G_POINT_BCC790 = (R3PointWord*)0xBCC790;
    R3PointWord* G_IFRIT_POSITION = (R3PointWord*)0xBCC760;
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

    setPoint(G_IFRIT_POSITION, 0, 0, 0);
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
    setupIfritMain(actioNTargetMask, casterIdx);
    srLogWrite("DEBUG: IFRIT EFFECT PREPARED");
}


#define gameIfritMain      ((PFNSR_VOIDSUB)0x593A95)

typedef R3PointWord* (*SRPFN_SUB661000)(u16);
#define gameGetHeapBuffer  ((SRPFN_SUB661000)0x661000)

typedef void(*SRPFN_SUB662AD8)(R3PointWord*, GameRotationMatrix*);
#define gameRotateMatrix      ((SRPFN_SUB662AD8)0x662AD8)

typedef void(*SRPFN_SUB5929F6)(void*, GameRotationMatrix*, u8);
#define pushIfritCameraHandler            ((SRPFN_SUB5929F6)0x5929F6)
void setupIfritMain(u16 actionTargetMask, u8 casterIdx) {
    R3PointWord* G_POINT_BCC790 = (R3PointWord*)0xBCC790;
    u8* byte_BFCB64 = (u8*)0xBFCB64;
    u16* word_BCC6B0 = (u16*)0xBCC6B0;
    auto targetMaskCenter = gameGetHeapBuffer(0);

    GameRotationMatrix* G_IFRIT_MODEL_MAT_BCC768 = (GameRotationMatrix*)0xBCC768;
    G_IFRIT_MODEL_MAT_BCC768->position[0] = 0;
    G_IFRIT_MODEL_MAT_BCC768->position[1] = 0;

    *word_BCC6B0 = actionTargetMask;
    computeTargetsCentroid(actionTargetMask, targetMaskCenter);

    //G_IFRIT_MAT_BCC768->position[2] = point->z + 600;
    G_IFRIT_MODEL_MAT_BCC768->position[2] = getBattleModelState(casterIdx)->restingPosition.z;
    if (targetMaskCenter->z >= getBattleModelState(casterIdx)->restingPosition.z) {
        G_POINT_BCC790->y = 0;
    }
    else {
        G_POINT_BCC790->y = 2048;
    }
    gameRotateMatrix(G_POINT_BCC790, G_IFRIT_MODEL_MAT_BCC768);
    srPushEffect100(srIfritMain);

    void* entranceDataBuffer = (void*)0x8C00C0;
    void* cameraDataBuffer = (void*)0x8C0114;
    void* cameraDataBufferB = (void*)0x8C01A8;
    pushIfritCameraHandler(entranceDataBuffer, G_IFRIT_MODEL_MAT_BCC768, casterIdx);
    /*else
        pushIfritCameraHandler(cameraDataBufferB, G_IFRIT_MAT_BCC768, casterIdx);*/

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

void srIfritMain() {
    u8* byte_BCC6A4 = (u8*)0xBCC6A4;
    R3PointWord* dword_BCC700 = (R3PointWord*)0xBCC700;
    u32* dword_C05EC8 = (u32*)0xC05EC8;
    u8* byte_C05EC8 = (u8*)0xC05EC8;
    u16* word_C05EC8 = (u16*)0xC05EC8;
    R3PointWord* G_POINT_BCC790 = (R3PointWord*)0xBCC790;
    u8* stru_BCC6E8 = (u8*)0xBCC6E8;

    GameRotationMatrix* G_IFRIT_MAT_BCC768 = (GameRotationMatrix*)0xBCC768;
    GameRotationMatrix* mat1 = (GameRotationMatrix*)0xBFCB30;
    GameRotationMatrix* ret = (GameRotationMatrix*)0xBCC6C8;

    if (!(*byte_BCC6A4 & 1)) {
        *byte_BCC6A4 |= 1u;
        dword_BCC700 = gameGetHeapBuffer(0);
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
        srPlayActionSoundEffect(34, 64, 0);
        break;
    }
    case 14: {
        srLogWrite("Pushing frame 14 Ifrit Effects");
        auto gameRotationMatrix = setAnimationTransitionState(1);
        gameRotationMatrix->R3Submatrix[0][2] = G_IFRIT_MAT_BCC768->position[0];
        gameRotationMatrix->R3Submatrix[1][0] = G_IFRIT_MAT_BCC768->position[1];
        gameRotationMatrix->R3Submatrix[1][1] = G_IFRIT_MAT_BCC768->position[2];
        gameRotationMatrix->R3Submatrix[2][2] = G_POINT_BCC790->y ^ 0x800;
        srPushEffect100(sub_594C07);
        srPushEffect100(sub_594D82);
        srPushEffect100(moveIfritEntrance);
        break;
    }
    case 44: {
        srPlayActionSoundEffect(34, 64, 1);
        pushRing();
        break;
    }
    case 45: {
        pushRing();
        break;
    }
    case 46: {
        pushRing();
        break;
    }
    case 47: {
        pushRing();
        break;
    }
    case 48: {
        pushRing();
        break;
    }
    case 90: {
        srLogWrite("IFIRT EFFECT FRAME 90 REACHED");
        setAnimationTransitionState(2);
        ifritEffectCtx->handlerIndex = 0xFFFF;
        flushSrEffects100();
        //srPushEffect100(descendIfritEffect);
        break;
    }
    }
    ++ifritEffectCtx->wordArray[0];
}


static const auto ifritFireDisk = (PFNSR_VOIDSUB)0x596B7E;
void pushRing() {
    GameRotationMatrix* G_IFRIT_MODEL_MAT_BCC768 = (GameRotationMatrix*)0xBCC768;
    R3PointWord* G_IFRIT_POSITION = (R3PointWord*)0xBCC760;
    auto fireRingCtx = (FireRingCtx*)getEffectContext100(srPushEffect100(ifritFireDisk));
    fireRingCtx->displayPoint.x = 0;
    fireRingCtx->displayPoint.y = ((rand() % 1000) - 3500);
    fireRingCtx->displayPoint.z = 0;
    srLogWrite("MODEL POSITION AT PUSH TIME: %i, %i, %i", G_IFRIT_POSITION->x, G_IFRIT_POSITION->y, G_IFRIT_POSITION->z);
    srLogWrite("PUSHING FIRE RING LOC: %i, %i, %i", fireRingCtx->displayPoint.x, fireRingCtx->displayPoint.y, fireRingCtx->displayPoint.z);
    fireRingCtx->euler.x = 0;
    fireRingCtx->euler.y = rand() & 0xFFF;
    fireRingCtx->euler.z = 0;
    srLogWrite("PUSHING FIRE RING EULER: %i, %i, %i", fireRingCtx->euler.x, fireRingCtx->euler.y, fireRingCtx->euler.z);
    fireRingCtx->field_14 = ((rand() >> 31) ^ abs(rand()) & 0x7F) - (rand() >> 31) + 384; //whodafuqknow
}

void descendIfritEffect() {
    auto descendEffectCtx = getEffect100QueueTop();
    u16 currentFrame = descendEffectCtx->wordArray[0];

    R3PointWord* G_IFRIT_POSITION = (R3PointWord*)0xBCC760;
    G_IFRIT_POSITION->y = -2000 - (-2000 * currentFrame / 30);
    descendEffectCtx->wordArray[0]++;
    srLogWrite("descend Handler current Frame: %i", currentFrame);
    if (currentFrame >= 30u) {
        srLogWrite("IFRIT: Descend Handler ending");
        //setAnimationTransitionState(2);
        descendEffectCtx->handlerIndex = 0xFFFF;
        flushSrEffects100();
    }
}


// HELLFIRE EFFECT HANDLERS
void prepareHellfire(u16 actioNTargetMask, u8 casterIdx) {
    R3PointWord* G_POINT_BCC790 = (R3PointWord*)0xBCC790;
    R3PointWord* G_IFRIT_POSITION = (R3PointWord*)0xBCC760;
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

    setPoint(G_IFRIT_POSITION, 0, 0, 0);
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
    setupHellfireMain(actioNTargetMask, casterIdx);
    srLogWrite("DEBUG: IFRIT EFFECT PREPARED");
}

void setupHellfireMain(u16 actionTargetMask, u8 casterIdx) {
    R3PointWord* G_POINT_BCC790 = (R3PointWord*)0xBCC790;
    u8* byte_BFCB64 = (u8*)0xBFCB64;
    u16* G_IFRIT_TARGET_MASK = (u16*)0xBCC6B0;
    auto targetMaskCenter = gameGetHeapBuffer(0);

    GameRotationMatrix* G_IFRIT_MODEL_MAT_BCC768 = (GameRotationMatrix*)0xBCC768;
    G_IFRIT_MODEL_MAT_BCC768->position[0] = 0;
    G_IFRIT_MODEL_MAT_BCC768->position[1] = 0;

    *G_IFRIT_TARGET_MASK = actionTargetMask;
    computeTargetsCentroid(actionTargetMask, targetMaskCenter);

    //G_IFRIT_MAT_BCC768->position[2] = point->z + 600;
    G_IFRIT_MODEL_MAT_BCC768->position[2] = getBattleModelState(casterIdx)->restingPosition.z;
    if (targetMaskCenter->z >= getBattleModelState(casterIdx)->restingPosition.z) {
        G_POINT_BCC790->y = 0;
    }
    else {
        G_POINT_BCC790->y = 2048;
    }
    gameRotateMatrix(G_POINT_BCC790, G_IFRIT_MODEL_MAT_BCC768);
    srPushEffect100(srHellfireEffectMain);

    void* cameraDataBuffer = (void*)0x8C0114;
    void* cameraDataBufferB = (void*)0x8C01A8;
    pushIfritCameraHandler(cameraDataBuffer, G_IFRIT_MODEL_MAT_BCC768, casterIdx);
    /*else
        pushIfritCameraHandler(cameraDataBufferB, G_IFRIT_MAT_BCC768, casterIdx);*/

    *byte_BFCB64 |= 8u;
}


static const auto bigFireBall = (PFNSR_VOIDSUB)0x5968E3;
static const auto fireBreathPuff = (PFNSR_VOIDSUB)0x596DA5;
static const auto fireBallTrail = (PFNSR_VOIDSUB)0x595092;
static const auto heatFlash = (PFNSR_VOIDSUB)0x595A05;
void srHellfireEffectMain() {
    u8* byte_BCC6A4 = (u8*)0xBCC6A4;
    R3PointWord* dword_BCC700 = (R3PointWord*)0xBCC700;
    R3PointWord* G_POINT_BCC790 = (R3PointWord*)0xBCC790;
    u8* stru_BCC6E8 = (u8*)0xBCC6E8;
    u16* G_IFRIT_TARGET_MASK = (u16*)0xBCC6B0;

    GameRotationMatrix* G_IFRIT_MAT_BCC768 = (GameRotationMatrix*)0xBCC768;
    GameRotationMatrix* G_EFFECT_MAT = (GameRotationMatrix*)0xBFCB30;
    GameRotationMatrix* ret = (GameRotationMatrix*)0xBCC6C8;
    GameRotationMatrix ifritBreathMatrix;
    R3PointWordH* G_BREATH_EULER = (R3PointWordH*)0x8C03E0;

    if (!(*byte_BCC6A4 & 1)) {
        *byte_BCC6A4 |= 1u;
        dword_BCC700 = gameGetHeapBuffer(0);
    }
    wordMatrixProduct(G_EFFECT_MAT, G_IFRIT_MAT_BCC768, ret);

    auto ifritEffectCtx = getEffect100QueueTop();
    u16 currentFrame = ifritEffectCtx->wordArray[0];

    if (*BATTLE_PAUSED_GLOBAL)
        return;

    switch (currentFrame) {
    case 0: {
        srPlayActionSoundEffect(34, 0x40u, 2);
        srFlushActionSoundEffect(428, 0);
        srFlushActionSoundEffect(429, 0);
        srFlushActionSoundEffect(430, 0);
        break;
    }
    case 0x32: {
        srPushEffect100(bigFireBall);
        break;
    }
    case 0x44: {
        srPushEffect100(fireBallTrail);
        srPlayActionSoundEffect(34, 0x40u, 3);
        srFlushActionSoundEffect(431, 1);
        srFlushActionSoundEffect(432, 1);
        srFlushActionSoundEffect(433, 1);
        break;
    }
    case 0x58: {
        srPushEffect100(srFireWaves);
        break;
    }
    case 0x69: {
        srDisplayDamageEvents(*G_IFRIT_TARGET_MASK);
        srPushEffect100(heatFlash);
        break;
    }
    case 0x78: {
        srFlushActionSoundEffect(434, 2);
        srFlushActionSoundEffect(435, 2);
        srFlushActionSoundEffect(436, 2);
        srFlushActionSoundEffect(437, 3);
        srFlushActionSoundEffect(438, 3);
        srFlushActionSoundEffect(439, 3);
        ifritEffectCtx->handlerIndex = 0xFFFF;
        break;
    }
    }

    if ((currentFrame > 0x15) && (currentFrame < 0x32)) {
        IfritBreathCtx* G_BREATH_CTX = (IfritBreathCtx*)0xBCC700;
        srCopyWeaponMatrix(1, 5, &ifritBreathMatrix);
        srSetTrailRotationMatrix(G_EFFECT_MAT, &ifritBreathMatrix, &G_BREATH_CTX->rotationMatrix);
        srCopyRotMatToGlobal(&G_BREATH_CTX->rotationMatrix);
        srCopyTranslationToGlobal(&G_BREATH_CTX->rotationMatrix);
        R3PointWord relative = { 0, 0x96, 0 };
        srComputeAbsolutePosition(&relative, &G_BREATH_CTX->position, &G_BREATH_CTX->field_30);

        for (auto idx = 0; idx < 2; ++idx) {
            auto breathCtx = (BreathPuffCtx*)getEffectContext100(srPushEffect100(fireBreathPuff));
            setPoint(&(breathCtx->position), G_BREATH_CTX->position.x, G_BREATH_CTX->position.y, G_BREATH_CTX->position.z);
            setPoint(&(breathCtx->velocity), G_BREATH_EULER[idx].x + rand() % 10 - 5, G_BREATH_EULER[idx].y + rand() % 10 - 5, G_BREATH_EULER[idx].z + rand() % 10 - 5);
            breathCtx->identityScale = ((rand() >> 31) ^ abs(rand()) & 0x1FF) - (rand() >> 31) + 512;
        }
    }
    ++ifritEffectCtx->wordArray[0];
}



static const auto fireWave = (PFNSR_VOIDSUB)0x59588C;
void srFireWaves() {
    auto waveCount = 5;
    auto wavesCtx = getEffect100QueueTop();
    if (*BATTLE_PAUSED_GLOBAL)
        return;

    for (auto waveIdx = 0; waveIdx < waveCount; ++waveIdx) {
        auto waveCtx = getEffectContext100(srPushEffect100(fireWave));
        waveCtx->wordArray[1] = rand() % 6000 - 3000;
        waveCtx->wordArray[3] = rand() % 10000;
        waveCtx->wordArray[5] = ((rand() & 0xFFFF) ^ (abs(rand()) & 4096)) - (rand() & 0xFFFF) + 6144;
        waveCtx->wordArray[6] = ((rand() >> 31) ^ abs(rand() & 4096)) - (rand() >> 31) + 6144;
    }
    if (++wavesCtx->wordArray[0] >= 9) {
        wavesCtx->contol = 0xFFFF;
    }
}
