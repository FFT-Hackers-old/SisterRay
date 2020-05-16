#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "types.h"

typedef enum {
    MODEL_TYPE_ENEMY,
    MODEL_TYPE_BACKGROUND,
    MODEL_TYPE_PLAYER
} SrModelType;

typedef struct {
    float rootTranslationX;
    float rootTranslationY;
    float rootTranslationZ;
    float rootRotationX;
    float rootRotationY;
    float rootRotationZ;
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
    AnimationFrame** frameDataView; //size here is frameCount, this is a ptr to a table of ptrs
} BattleAnimation;

typedef struct {
    u32 rawBufferSize; //This is used when memcopying to the buffers actually used in battle
    BattleAnimation* animationData;
} SrAnimation;

#endif
