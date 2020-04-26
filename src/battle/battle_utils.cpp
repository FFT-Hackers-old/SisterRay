#include "battle_utils.h"

u8 countTargets(u16 targetMask) {
    u8 count = 0;
    while (targetMask) {
        if (targetMask & 1)
            ++count;
        targetMask >>= 1;
    }
    return count;
}

u8 getBattleType() {
    u8* G_BATTLE_TYPE = (u8*)0xBFD0E0;
    return *G_BATTLE_TYPE;
}

u8 getTargetAllActive() {
    u8* G_TARGETING_ALL = (u8*)0xBF2E1C;
    return *G_TARGETING_ALL;
}

u16 getAnimatingActionTargetMask() {
    u16* G_TARGET_MASK = (u16*)0xBFD0F8;
    return *G_TARGET_MASK;
}


u8 getAnimatingActionTargetIdx() {
    u8* G_TARGET_IDX = (u8*)0xBFB198;
    return *G_TARGET_IDX;
}

u8 getActionActorIdx() {
    u8* G_ACTIVE_ACTOR = (u8*)0xBE1170;
    return *G_ACTIVE_ACTOR;
}

u8 getActorIdleAnimScript(u8 actorID) {
    u8* G_ACTOR_IDLE_SCRIPTS = (u8*)0xBF2DF8;
    return G_ACTOR_IDLE_SCRIPTS[actorID];
}


void setPoint(R3PointWord* point, i16 x, i16 y, i16 z) {
    point->x = x;
    point->y = y;
    point->z = z;
}

void setPoint(R3Point* point, float x, float y, float z) {
    point->x = x;
    point->y = y;
    point->z = z;
}

void nullGameMatrix(GameRotationMatrix* matrix) {
    memset(matrix, 0, sizeof(GameRotationMatrix));
}

void setDiagonal(GameRotationMatrix* matrix, i16 e11, i16 e22, i16 e33) {
    matrix->R3Submatrix[2][2] = e33;
    matrix->R3Submatrix[1][1] = e22;
    matrix->R3Submatrix[0][0] = e11;
}

typedef void(*SRPFN_SUB5BFEC9)(u16, R3PointWord*);
#define gameComputeTargetsCentroid      ((SRPFN_SUB5BFEC9)0x5BFEC9)
void computeTargetsCentroid(u16 targetMask, R3PointWord* ret) {
    gameComputeTargetsCentroid(targetMask, ret);
}


typedef GameRotationMatrix*(*SRPFN_SUB5BFAA0)(u32);
#define gameSetAnimationTransitionState     ((SRPFN_SUB5BFAA0)0x5BFAA0)
GameRotationMatrix* setAnimationTransitionState(u32 tranistionState) {
    return gameSetAnimationTransitionState(tranistionState);
}
