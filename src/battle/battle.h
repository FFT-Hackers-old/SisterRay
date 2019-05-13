#ifndef BATTLE_H
#define BATTLE_H

#include <SisterRay/SisterRay.h>
typedef struct {
    u8 sizeScale;
    u8 idleAnim;
    u8 damagedAnim;
} scriptAnimDataCpy;

void initFormationsRegistries();
i16 loadBattleFormation(i32 formationIndex, i32(*modelAppearCallback)(void));
i32 srExecuteAIScript(i32 actorIndex, i32 scriptID, i32 unkInt);

u32 getCompressedFileSizes(const u8* sceneBlock, u16 byteSizes[16]);
void readBlock(const char* filename, u32 blockSize, u8* dst);
void populateRegistries(const SceneLayout& sceneData, u16* formationIndex);
u16 findEndChar(const u8* sceneBlock);

#endif
