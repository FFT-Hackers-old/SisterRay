#ifndef BATTLE_ANIMATIONS_H
#define BATTLE_ANIMATIONS_H

#include <SisterRay/SisterRay.h>
#include "model_file_utils.h"
#include <unordered_map>
#include "../../sr_named_registry.h"


u32 getSrAnimsCount(ModelAAHeader *aaHeader, const char * filename);
void srLoadBattleModelAnimations(void** animatioNDataTable, u32 startingIndex, char* filename);
void srReadDaFile(u32 someFlag, u32* animDataStartPtr, u32 animsToRead, void** animationDataTable);

BattleAnimation* srCreateBattleAnimation(u32 frameCount, u32 boneCount, u32 modelTypeData);
typedef BattleAnimation*(*PFNSRSUB691F03)(u32, u32, u32);
#define createBattleAnimation   ((PFNSRSUB691F03)0x691F03)

/*unk arg seems to always be 1 on model loads*/
BattleAnimation* createAnimationFromDABuffer(u32 unkArg, u32 boneCount, u32 frameCount, void* frameDataBuffer);
typedef BattleAnimation*(*PFNSRSUB5E7DE4)(u32, u32, u32, void*);
#define loadAnimationFromDA     ((PFNSRSUB5E7DE4)0x5E7DE4)

#endif
