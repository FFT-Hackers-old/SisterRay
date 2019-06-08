#ifndef BATTLE_ANIMATIONS_H
#define BATTLE_ANIMATIONS_H

#include <SisterRay/SisterRay.h>

typedef struct {
    u32 rootTranslationX;
    u32 rootTranslationY;
    u32 rootTranslationZ;
    u32 rootRotationX;
    u32 rootRotationY;
    u32 rootRotationZ;
} RootMovement;

typedef struct {
    float rotationX;
    float rotationY;
    float rotationZ;
} BoneMovement;

/*This provides a view of the actual allocated buffer*/
typedef struct {
    RootMovement* rootMovement;
    BoneMovement* boneData;
} AnimationFrame;

/*This is the in memory format of animations loaded from the DA models*/
typedef struct {
    u32 alwaysOne;
    u32 frameCount;
    u32 BonesCount;
    u32 unkDword;
    void* rawAnimationDataBuffer;
    AnimationFrame* frameDataView;
} BattleAnimation;

typedef BattleAnimation*(*PFNSRSUB691F03)(u32, u32, u32);
#define createBattleAnimation   ((PFNSRSUB691F03)0x691F03)

#endif
