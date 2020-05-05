#include "model_skeleton.h"


typedef ModelSkeleton* (*PFNSRSUB5E11CC)(i32, i32, i32, AABone*, ModelAAHeader*, LGPLoadCtx*, FileContext*, char*);
#define gameCreateSkeleton  ((PFNSRSUB5E11CC)0x5E11CC)


ModelSkeleton* srCreateSkeleton(i32 isWeapon, i32 weaponModelID, i32 boneCount, AABone* boneConnectionPtr, ModelAAHeader* aaHeader, LGPLoadCtx* loadCtx, FileContext* fileCtx, char* modelName) {
    return gameCreateSkeleton(isWeapon, weaponModelID, boneCount, boneConnectionPtr, aaHeader, loadCtx, fileCtx, modelName);
}
