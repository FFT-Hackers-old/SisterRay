#include "model_animations.h"
#include "model_file_utils.h"

/*Will load every function from the LGP, and any additional animation files that were loaded for this model
  For player models, handle weapon loading/set data so that limits work correctly. Archive Index seems to always be 0, some flag always 1*/
void srLoadAnimations(u32 someFlag, u32 archiveIndex, void **animationDataTable, LGPContext *context, char *filename, const ModelAAHeader* const aaHeader, const DaFileCounts DaFileCounts) {
    i32 bytesRead; 
    u32 *daArchiveFile; 
    u32 *daFilePtr; 
    char daFileNameBuffer[204]; 

    createDAFilename(archiveIndex, filename, &(daFileNameBuffer[0]));
    daArchiveFile = (u32*)srLoadLGPFile(context, &bytesRead, &(daFileNameBuffer[0]));// loads the da file
    if (daArchiveFile) {
        daFilePtr = daArchiveFile;
        u32* animDataStartPtr = &(daArchiveFile[1]);
        srReadDaFile(someFlag, animDataStartPtr, DaFileCounts.daFileModelAnimCount, animationDataTable); //Read just the model animations from the DA file
        srLoadBattleModelAnimations(animationDataTable, DaFileCounts.daFileModelAnimCount, filename); //We load new animations, starting at the first free index
        free(daArchiveFile);
    }
}

//Read x animations from the file, and return the byte index of the next starting position, mutates passed file ptr
void srReadDaFile(u32 someFlag, u32* animDataStartPtr, u32 animsToRead, void** animationDataTable) {
    DaAnimHeader* animHeader;
    if (animDataStartPtr) {
        for (auto animationIndex = 0; animationIndex < animsToRead; ++animationIndex) {
            animHeader = (DaAnimHeader*)animDataStartPtr;
            u32* frameDataPtr = animDataStartPtr + 3;
            animationDataTable[animationIndex] = createAnimationFromDABuffer(someFlag, animHeader->bonesCount, animHeader->framesCount, frameDataPtr);
            animDataStartPtr = &(frameDataPtr[animHeader->compressedSize]);
        }
    }
}

void srLoadBattleModelAnimations(void** animatioNDataTable, u32 startingIndex, char* filename) {
    return;
}

u32 getSrAnimsCount(ModelAAHeader *aaHeader, const char * filename) {
    u32 srNewAnimsCount;
    u32 animCount = aaHeader->weaponAnimAcount + aaHeader->modelAnimCount;
    aaHeader->modelAnimCount += srNewAnimsCount;
    aaHeader->weaponAnimAcount += srNewAnimsCount;
    return animCount;
}


/*Use the games utility for decompressing */
BattleAnimation* createAnimationFromDABuffer(u32 unkArg, u32 boneCount, u32 frameCount, void* frameDataBuffer) {
    return loadAnimationFromDA(unkArg, boneCount, frameCount, frameDataBuffer);
}

BattleAnimation* srCreateBattleAnimation(u32 frameCount, u32 boneCount, u32 modelTypeData) {
    return createBattleAnimation(frameCount, boneCount, modelTypeData);
}
