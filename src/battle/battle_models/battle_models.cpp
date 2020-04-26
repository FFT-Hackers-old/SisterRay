#include "battle_models.h"
#include "../../impl.h"

typedef void(*PFNSRSUB674659)(u32, FileContext*);
#define setSomeFileContext  ((PFNSRSUB674659)0x674659)


SISTERRAY_API BattleModel* getActiveModelFromArray(u8 arrayIdx) {
    return BATTLE_MODEL_PTRS[arrayIdx];
}


/*BattleModel* srGetBattleModel(u32 a1, u8 modelPtrArrayIdx, UnkModelLoadStruct* loadCtx, FileContext* fileContext, char* filename) {
    BattleModel* battleModel = 0;
    setLoadStructData(a1, 2, loadCtx, fileContext);
    setFileContextData(fileContext);
    if (loadCtx->field_28 || !loadCtx->loadWeapon)// yTho
        battleModel = sub_5E0370((int)loadCtx, (int)fileContext, filename);
    if (loadCtx->loadWeapon) {
        gContext.weapons.getResource(gContext.party.getActivePartyCharacter(modelPtrArrayIdx).equippedWeapon).;
        if (actorData)
            u16 weaponModelID = actorData->equippedWeaponData.weaponModel & 0xF;
        else
            u16 weaponModelID = 0;

        auto modelAAHeader = (ModelAAHeader*)loadModelAAHeader(0, &fileContext->LGPContext, filename);
        if (modelAAHeader) {
            freeOldModel(battleModel);
            battleModel = srCreateModel(0, weaponModelID, modelAAHeader, loadCtx, fileContext, filename);
            sub_5E0EED(0, (void**)modelAAHeader);
        }
    }
    return battleModel;
}*/

BattleModel* srCreateModel(u32 readTypeFlag, u32 weaponModelID, ModelAAHeader *aaHeader, LGPLoadCtx *loadStruct, FileContext *fileContext, char * filename) {
    BattleModel *modelData;
    char FF7Directory[204];
    u32 animationsCount;
    srLogWrite("loading battle model: %s", filename);

    if (readTypeFlag)
        noLGPgetDirectory(&fileContext->lgpContext, FF7Directory);
    else
        getFF7Directory(FF7Directory);

    modelData = nullptr;
    if (aaHeader) {
        if (gContext.battleAnimationScripts.contains(filename)) {
            animationsCount = gContext.battleAnimations.getElement(std::string(filename)).totalAnimationCount;
        }
        else {
            animationsCount = getSrAnimsCount(aaHeader, filename);
        }
        DaFileCounts daFileModelAnimcount = { aaHeader->modelAnimCount, aaHeader->weaponAnimAcount };
        srLogWrite("MODEL LOAD: allocating table space for %i animations for model %s", animationsCount, filename);
        modelData = allocateModelData(animationsCount, aaHeader->weaponCount);
        if (modelData) {
            if (aaHeader->loadBFileFlag) {
                if (gContext.battleAnimationScripts.contains(filename)) {
                    modelData->animScriptStruct = srInitializeAnimScriptsData(filename, aaHeader);
                }
                else {
                    modelData->animScriptStruct = srGameLoadABFile(&fileContext->lgpContext, filename);
                }
            }
            if (aaHeader->initSkeletonFlag) {
                modelData->skeletonData = createSkeleton(0, 0, aaHeader->modelBoneCount, aaHeader->boneDataPtr, aaHeader, loadStruct, fileContext, filename);
            }
            if (weaponModelID < aaHeader->weaponCount) {
                setSomeFileContext(6, fileContext);
                modelData->weaponModels[weaponModelID] = createSkeleton(1, weaponModelID, 0, aaHeader->boneDataPtr, aaHeader, loadStruct, fileContext, filename);
            }
            if (gContext.battleAnimationScripts.contains(filename)) {
                srInitializeAnimationsTable((void**)modelData->animationsTable, animationsCount, filename, aaHeader);
            }
            else {
                srGameInitAnimations(loadStruct->lgpTableIdx, 0, (void**)modelData->animationsTable, &fileContext->lgpContext, filename);
            }
        }
    }

    if (readTypeFlag)
        noLGPClearContext(&fileContext->lgpContext, FF7Directory);
    else
        clearDirectory(FF7Directory);

    return modelData;
}


typedef void(*SRPFNSUB5DFDC0)(LGPLoadCtx*);
#define  clearLoadCtx       ((SRPFNSUB5DFDC0)0x5DFDC0)

typedef void(*SRPFNSUB67455E)(FileContext*);
#define initializeFileContext  ((SRPFNSUB67455E)0x67455E)

typedef void(*SRPFNSUB5E0200)(BattleModel*);
#define freeOldModel           ((SRPFNSUB5E0200)0x5E0200)

typedef BattleModel*(*SRPFNSUB5E1AD2)(i32, i32, LGPLoadCtx*, FileContext*, char*);
#define getBattleModel         ((SRPFNSUB5E1AD2)0x5E1AD2)

/*This now saves the model that was set keyed by actor index for later use*/
BattleModel* srSetPlayerModel(i32 a1, i32 a2, u8 modelArrayIdx, char *filename) {
    srLogWrite("Inside srSetPlayerModel");
    LGPLoadCtx structBuffer;
    LGPLoadCtx *unkStructPtr; 
    FileContext fileContext; 

    BattleModel* modelDataPtr = nullptr;
    unkStructPtr = &structBuffer;
    clearLoadCtx(&structBuffer);               // 0x44 in size
    initializeFileContext(&fileContext);
    unkStructPtr->field_10 = a1;
    unkStructPtr->field_14 = a2;
    if (modelArrayIdx < 3) {
        getSrPartyMember(modelArrayIdx).srPartyMember->modelName = std::string(filename);
    }
    //The following code runs if a model is being swapped
    if (BATTLE_MODEL_PTRS[modelArrayIdx]) {
        freeOldModel(BATTLE_MODEL_PTRS[modelArrayIdx]);
        BATTLE_MODEL_PTRS[modelArrayIdx] = nullptr;
        for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
            if (BATTLE_MODEL_PTRS[modelArrayIdx] == BATTLE_MODEL_STATE_BIG_ARRAY[partyIdx].modelDataPtr)
                BATTLE_MODEL_STATE_BIG_ARRAY[partyIdx].modelDataPtr = nullptr;
        }
    }
    //The following code sets the 7 models that are active for the battle
    modelDataPtr = getBattleModel(0, modelArrayIdx, unkStructPtr, &fileContext, filename);
    if (modelDataPtr)
        BATTLE_MODEL_PTRS[modelArrayIdx] = modelDataPtr;
    return modelDataPtr;
}


typedef BattleModel* (*SRPFN_SUB5E3900)(LGPLoadCtx*, FileContext*, char*);
#define sub_5E3900       ((SRPFN_SUB5E3900)0x5E3900)

BattleModel* srSetSummonModel(u32 a1, char* filename) {
    LGPLoadCtx loadCtx; 
    FileContext fileCtx; 

    BattleModel* battleModel = nullptr;
    if (filename != nullptr){
        clearLoadCtx(&loadCtx);
        initializeFileContext(&fileCtx);
        setSomeFileContext(a1, &fileCtx);
        initLoadCtx(0, 5, &loadCtx, &fileCtx);
        u32* fileCtxHead = (u32*)&fileCtx;
        *fileCtxHead |= 0x100000u;
        auto v2 = *fileCtxHead;
        fileCtx.unk[3] |= 0x68u;
        *fileCtxHead = v2 | 0x2000000;
        if (loadCtx.field_28 || !loadCtx.loadWeapon)
            battleModel =  sub_5E3900(&loadCtx, &fileCtx, filename);
        if (loadCtx.loadWeapon) {
            auto modelAAHeader = loadModelAAHeader(1, &(fileCtx.lgpContext), filename);
            if (modelAAHeader) {
                freeOldModel(battleModel);
                battleModel = srCreateModel(1, 0, modelAAHeader, &loadCtx, &fileCtx, filename);
                destroyModelAAHeader(0, modelAAHeader);
            }
        }
    }
    return battleModel;
}
