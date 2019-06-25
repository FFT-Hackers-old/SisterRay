#ifndef BATTLE_MODELS_H
#define BATTLE_MODELS_H

#include <SisterRay/SisterRay.h>
#include "model_file_utils.h"
#include "model_skeleton.h"
#include "model_animations.h"

#define FF7_CREATE_MODEL    ((void*)0x5E149A)
#define BATTLE_MODEL_PTRS  ((BattleModel**)(0x0BFB2B8))

//Malloc call that creates this has an arg size of 0x94 + 32
typedef struct {
    void* animScriptStruct;            //0x00
    ModelSkeleton* skeletonData;       //0x04
    u32 totalAnimations;               //0x08
    BattleAnimation** animationsTable; //0x0C
    u32 limitCount;                    //0x10
    void* limitsTable;                 //0x14
    u32 weaponCount;                   //0x18
    ModelSkeleton** weaponModels;      //0x1C
    float matrixMeta[11];              //0x20
    float modelMatrix[16];             //initialized to identity element of GL_4(R) on model create
    float unk1;
    float unk2;
} BattleModel;

BattleModel* srCreateModel(u32 readTypeFlag, u32 weaponModelID, ModelAAHeader *aaHeader, UnkModelLoadStruct *loadStruct, FileContext *fileContext, char * filename);
BattleModel* srSetPlayerModel(i32 a1, i32 a2, u8 actorIndex, char *filename);

typedef BattleModel*(*PFNSRSUB5E02E9)(size_t, size_t);
#define allocateModelData  ((PFNSRSUB5E02E9)0x5E02E9)

#endif
