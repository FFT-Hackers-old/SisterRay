#include "battle_models.h"
#include "../../impl.h"

typedef void(*PFNSRSUB674659)(u32, FileContext*);
#define setSomeFileContext  ((PFNSRSUB674659)0x674659)

BattleModel* srCreateModel(u32 readTypeFlag, u32 weaponModelID, ModelAAHeader *aaHeader, UnkModelLoadStruct *loadStruct, FileContext *fileContext, char * filename) {
    BattleModel *modelData;
    char FF7Directory[204];
    u32 bytesRead;
    u32 animationsCount;
    srLogWrite("calling sr Create Model");
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
        srLogWrite("allocate memory for %i animations when loading model %s", animationsCount, filename);
        modelData = allocateModelData(animationsCount, aaHeader->weaponCount);
        srLogWrite("allocated block for new model at %p", modelData);
        if (modelData) {
            if (aaHeader->loadBFileFlag) {
                if (gContext.battleAnimationScripts.contains(filename)) {
                    //char abFileNameBuffer[204];
                    //int bytesRead;
                    //createABFilename(filename, &(abFileNameBuffer[0]));
                    //modelData->animScriptStruct = ff7LoadModelFile(&fileContext->lgpContext, &bytesRead, &(abFileNameBuffer[0]));
                    srLogWrite("Loading model animation scripts from SR registry for model %s", filename);
                    modelData->animScriptStruct = srInitializeAnimScriptsData(filename, aaHeader);
                    srLogWrite("allocated ab struct at %p", modelData->animScriptStruct);
                }
                else {
                    srLogWrite("ATTEMPTING TO INITIALIZE MODEL NOT WITHOUT SCRIPT DATA IN REGISTRY!");
                }
            }
            if (aaHeader->initSkeletonFlag) {
                modelData->skeletonData = createSkeleton(0, 0, aaHeader->modelBoneCount, aaHeader->boneDataPtr, aaHeader, loadStruct, fileContext, filename);
            }
            if (weaponModelID < aaHeader->weaponCount) {
                setSomeFileContext(6, fileContext);
                modelData->weaponModels[weaponModelID] = createSkeleton(1, weaponModelID, 0, aaHeader->boneDataPtr, aaHeader, loadStruct, fileContext, filename);
            }
            srLogWrite("attempting to initialize animation data for model %s", filename);
            srInitializeAnimationsTable((void**)modelData->animationsTable, animationsCount, filename, aaHeader);
        }
    }

    if (readTypeFlag)
        noLGPClearContext(&fileContext->lgpContext, FF7Directory);
    else
        clearDirectory(FF7Directory);

    srLogWrite("returning srcreateModel");
    return modelData;
}
