#ifndef BATTLE_MODELS_H
#define BATTLE_MODELS_H

#include <SisterRay/SisterRay.h>
#include "model_file_utils.h"
#include "model_skeleton.h"
#include "model_animations.h"

#define FF7_CREATE_MODEL    ((void*)0x5E149A)
#define BATTLE_MODEL_PTRS  ((BattleModel**)(0x0BFB2B8))
typedef void(*SRPFNSUB5E0200)(BattleModel*);
#define freeOldModel           ((SRPFNSUB5E0200)0x5E0200)

BattleModel* srCreateModel(u32 readTypeFlag, u32 weaponModelID, ModelAAHeader *aaHeader, LGPLoadCtx *loadStruct, FileContext *fileContext, char * filename);
BattleModel* srSetPlayerModel(i32 a1, i32 a2, u8 actorIndex, char *filename);
BattleModel* srGetBattleModel(u32 a1, u8 modelPtrArrayIdx, LGPLoadCtx* loadCtx, FileContext* fileCtx, char* filename);
void srDestroyBattleModel(BattleModel* model);

typedef BattleModel*(*PFNSRSUB5E02E9)(size_t, size_t);
#define gameAllocateBattleModel  ((PFNSRSUB5E02E9)0x5E02E9)
BattleModel* srSetSummonModel(u32 a1, char* filename);

#endif
