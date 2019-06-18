#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <SisterRay/SisterRay.h>

void initFormationsRegistries();
u32 getCompressedFileSizes(const u8* sceneBlock, u16 byteSizes[16]);
void readBlock(FILE* filehandle, u32 blockSize, u8* dst);
void populateRegistries(const SceneLayout& sceneData, u16* formationIndex);
u16 findEndChar(const u8* sceneBlock);

#endif // !SCENE_LOADER_H
