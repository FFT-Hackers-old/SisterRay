#ifndef MODEL_SKELETON_H
#define MODEL_SKELETON_H

#include <SisterRay/SisterRay.h>
#include "model_file_utils.h"

ModelSkeleton* srCreateSkeleton(i32 isWeapon, i32 weaponModelID, i32 boneCount, AABone* boneConnectionPtr, ModelAAHeader* aaHeader, LGPLoadCtx* loadCtx, FileContext* fileCtx, char* modelName);
#endif // !MODEL_SKELETON_H
