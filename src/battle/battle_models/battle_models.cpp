#include "battle_models.h"
#include "../../impl.h"

/*If an SR animation file is found, we should use the number of animations it specifies instead of what the aa file does*/

BattleModel* srCreateModel(u32 readTypeFlag, u32 weaponModelID, ModelAAHeader *aaHeader, UnkModelLoadStruct *loadStruct, FileContext *fileContext, char * filename) {
    BattleModel *modelData;
    char FF7Directory[204];
    u32 bytesRead;

    if (readTypeFlag)
        noLGPgetDirectory(&fileContext->lgpContext, FF7Directory);
    else
        getFF7Directory(FF7Directory);

    modelData = nullptr;
    if (aaHeader) {
        DaFileCounts daFileModelAnimcount = { aaHeader->modelAnimCount, aaHeader->weaponAnimAcount };
        u32 animationsCount = getSrAnimsCount(aaHeader, filename); //Look up in registry of animations per model
        modelData = allocateModelData(animationsCount, aaHeader->weaponCount);
        if (modelData) {
            if (aaHeader->loadBFileFlag) {
                modelData->animScriptStruct = srOpenABFile(&(fileContext->lgpContext), filename);
            }
            if (aaHeader->initSkeletonFlag) {
                modelData->skeletonData = createSkeleton(0, 0, aaHeader->modelBoneCount, aaHeader->boneDataPtr, aaHeader, loadStruct, fileContext, filename);
            }
            if (weaponModelID < aaHeader->weaponCount) {
                setSomeFileContext(6, fileContext);
                modelData->weaponModels[weaponModelID] = createSkeleton(1, weaponModelID, 0, aaHeader->boneDataPtr, aaHeader, loadStruct, fileContext, filename);
            }
            srLoadAnimations(loadStruct->field_8, 0, modelData->animationsTable, &fileContext->lgpContext, filename, aaHeader, daFileModelAnimcount);
        }
    }

    if (readTypeFlag)
        noLGPClearContext(&fileContext->lgpContext, FF7Directory);
    else
        clearDirectory(FF7Directory);

    return modelData;
}
