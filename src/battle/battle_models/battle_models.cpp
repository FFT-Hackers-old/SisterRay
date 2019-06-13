#include "battle_models.h"
#include "../../impl.h"

/*If an SR animation file is found, we should use the number of animations it specifies instead of what the aa file does*/

BattleModel* srCreateModel(u32 readTypeFlag, u32 weaponModelID, ModelAAHeader *aaHeader, UnkModelLoadStruct *loadStruct, FileContext *fileContext, char * filename) {
    BattleModel *modelData;
    char FF7Directory[204];
    u32 bytesRead;
    u32 animationsCount;

    if (readTypeFlag)
        noLGPgetDirectory(&fileContext->lgpContext, FF7Directory);
    else
        getFF7Directory(FF7Directory);

    modelData = nullptr;
    if (aaHeader) {
        if (gContext.battleAnimationScripts.contains(filename)) {
            animationsCount = gContext.battleAnimations.get_element(std::string(filename)).totalAnimationCount;
        }
        else {
            animationsCount = getSrAnimsCount(aaHeader, filename);
        }
        DaFileCounts daFileModelAnimcount = { aaHeader->modelAnimCount, aaHeader->weaponAnimAcount };
        modelData = allocateModelData(animationsCount, aaHeader->weaponCount);
        if (modelData) {
            if (aaHeader->loadBFileFlag) {
                if (gContext.battleAnimationScripts.contains(filename)) {
                    srLogWrite("Loading model animation scripts from SR registry for model %s", filename);
                }
                else {
                    modelData->animScriptStruct = srOpenABFile(&(fileContext->lgpContext), filename);
                }
            }
            if (aaHeader->initSkeletonFlag) {
                modelData->skeletonData = createSkeleton(0, 0, aaHeader->modelBoneCount, aaHeader->boneDataPtr, aaHeader, loadStruct, fileContext, filename);
            }
            if (weaponModelID < aaHeader->weaponCount) {
                //setSomeFileContext(6, fileContext);
                modelData->weaponModels[weaponModelID] = createSkeleton(1, weaponModelID, 0, aaHeader->boneDataPtr, aaHeader, loadStruct, fileContext, filename);
            }
            //srLoadAnimations(loadStruct->field_8, 0, modelData->animationsTable, &fileContext->lgpContext, filename, aaHeader, daFileModelAnimcount);
        }
    }

    if (readTypeFlag)
        noLGPClearContext(&fileContext->lgpContext, FF7Directory);
    else
        clearDirectory(FF7Directory);

    return modelData;
}
