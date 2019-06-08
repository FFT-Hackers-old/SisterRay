#include "model_animations.h"
#include "model_file_utils.h"

/*Will load every function from the LGP, and any additional animation files that were loaded for this model
  For player models, handle weapon loading/set data so that limits work correctly*/
/*void srLoadAnimations(u32 a1, u32 a2, void **animationDataTable, LGPContext *context, char *filename, const ModelAAHeader* const aaHeader, const DaFileCounts DaFileCounts) {
    char addressOfString[4]; // [esp+10h] [ebp-D4h]
    u32 *daArchiveFile; // [esp+14h] [ebp-D0h]
    u32 *daFilePtr; // [esp+18h] [ebp-CCh]
    char daFileNameBuffer[204]; // [esp+1Ch] [ebp-C8h]

    createDAFilename(a2, filename, &(daFileNameBuffer[0]));
    daArchiveFile = (u32*)ff7LoadModelFile(context, addressOfString, &(daFileNameBuffer[0]));// loads the da file
    if (daArchiveFile) {
        daFilePtr = daArchiveFile;
        u32* animDataStartPtr = &(daArchiveFile[1]);
        srReadDaFile(animDataStartPtr, DaFileCounts.daFileModelAnimCount, animationDataTable); //Read just the model animations from the DA file
        loadSrBattleModelAnimations(animationDataTable, DaFileCounts.daFileModelAnimCount, filename); //We load new animations, starting at the first free index
        customFree(daArchiveFile, (int)aCFf7SrcBattleB_46, 595);
    }
}

//Read x animations from the file, and return the byte index of the next starting position, mutates passed file ptr
void srReadDaFile(u32* animDataStartPtr, u32 animsToRead, void** animationDataTable) {
    DaAnimHeader* animHeader;

    if (animDataStartPtr) {
        for (auto animationIndex = 0; animationIndex < animsToRead; ++animationIndex) {
            animHeader = (DaAnimHeader*)animDataStartPtr;
            u32* frameDataPtr = animDataStartPtr + 3;
            animationDataTable[animationIndex] = loadAnimationFromDA(a1, animHeader->bonesCount, animHeader->framesCount, frameDataPtr);
            animDataStartPtr = &(frameDataPtr[animHeader->compressedSize]);
        }
    }
}

void loadsrBattleModelAnimations(void** animatioNDataTable, u32 startingIndex, char* filename) {
    return;
}*/
