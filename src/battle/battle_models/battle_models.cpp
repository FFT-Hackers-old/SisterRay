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


typedef void(*SRPFNSUB5DFDC0)(UnkModelLoadStruct*);
#define  clearSomeStruct       ((SRPFNSUB5DFDC0)0x5DFDC0)

typedef void(*SRPFNSUB67455E)(FileContext*);
#define initializeFileContext  ((SRPFNSUB67455E)0x67455E)

typedef void(*SRPFNSUB5E0200)(BattleModel*);
#define freeOldModel           ((SRPFNSUB5E0200)0x5E0200)

typedef BattleModel*(*SRPFNSUB5E1AD2)(i32, i32, UnkModelLoadStruct*, FileContext*, char*);
#define getBattleModel         ((SRPFNSUB5E1AD2)0x5E1AD2)

/*This now saves the model that was set keyed by actor index for later use*/
BattleModel* srSetPlayerModel(i32 a1, i32 a2, u8 actorIndex, char *filename) {
    srLogWrite("Inside srSetPlayerModel");
    UnkModelLoadStruct structBuffer;
    UnkModelLoadStruct *unkStructPtr; 
    FileContext fileContext; 

    BattleModel* modelDataPtr = nullptr;
    unkStructPtr = &structBuffer;
    clearSomeStruct(&structBuffer);               // 0x44 in size
    initializeFileContext(&fileContext);
    unkStructPtr->field_10 = a1;
    unkStructPtr->field_14 = a2;

    gContext.party.get_element(getPartyKey(actorIndex)).modelName = std::string(filename);
    //The following code runs if a model is being swapped
    if (BATTLE_MODEL_PTRS[actorIndex]) {
        freeOldModel(BATTLE_MODEL_PTRS[actorIndex]);
        BATTLE_MODEL_PTRS[actorIndex] = nullptr;
        for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
            if (BATTLE_MODEL_PTRS[actorIndex] == gBigAnimBlock[partyIdx].modelDataPtr)
                gBigAnimBlock[partyIdx].modelDataPtr = nullptr;
        }
    }
    //The following code sets the 7 models that are active for the battle
    modelDataPtr = getBattleModel(0, actorIndex, unkStructPtr, &fileContext, filename);
    if (modelDataPtr)
        BATTLE_MODEL_PTRS[actorIndex] = modelDataPtr;
    return modelDataPtr;
}
