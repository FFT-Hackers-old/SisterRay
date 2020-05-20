#include "battle_models.h"
#include "battle_model_state_interface.h"
#include "../../impl.h"

SISTERRAY_API BattleModel* getActiveModelFromArray(u8 arrayIdx) {
    return BATTLE_MODEL_PTRS[arrayIdx];
}

SISTERRAY_API void setActiveModelFromArray(u8 arrayIdx, BattleModel* battleModel) {
    BATTLE_MODEL_PTRS[arrayIdx] = battleModel;
}

void srDestroyBattleModel(BattleModel* model) {
   freeOldModel(model);
}

typedef BattleModel* (*SRPFN_SUB5E3900)(LGPLoadCtx*, FileContext*, char*);
#define sub_5E3900       ((SRPFN_SUB5E3900)0x5E3900)

typedef BattleModel* (*SRPFN_SUB5E3900)(LGPLoadCtx*, FileContext*, char*);
#define gameLoadBattleBackground       ((SRPFN_SUB5E3900)0x5E0370)

BattleModel* srGetBattleModel(u32 a1, u8 modelPtrArrayIdx, LGPLoadCtx* loadCtx, FileContext* fileCtx, char* filename) {
    BattleModel* battleModel = 0;
    initLoadCtx(a1, 2, loadCtx, fileCtx);
    setFileContext(fileCtx);
    if (loadCtx->field_28 || !loadCtx->loadWeapon)// yTho
        battleModel = gameLoadBattleBackground(loadCtx, fileCtx, filename);
    if (loadCtx->loadWeapon) {
        u16 weaponModelID = 0;
        if (modelPtrArrayIdx < 3) {
            auto weaponIdx = gContext.party.getActivePartyCharacter(modelPtrArrayIdx).equipment[0].equippedIdx;
            weaponModelID = gContext.weapons.getResource(weaponIdx).weaponModelID;
        }
        auto modelAAHeader = loadModelAAHeader(0, &fileCtx->lgpContext, filename);
        if (modelAAHeader) {
            srDestroyBattleModel(battleModel);
            battleModel = srCreateModel(0, weaponModelID, modelAAHeader, loadCtx, fileCtx, filename);
            destroyModelAAHeader(0, modelAAHeader);
        }
    }
    return battleModel;
}

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
        u16 weaponCount = aaHeader->weaponCount;
        modelData = gameAllocateBattleModel(animationsCount, weaponCount);
        if (modelData) {
            if (aaHeader->loadBFileFlag) {
                if (gContext.battleAnimationScripts.contains(filename)) {
                    srLogWrite("Creating Animation Scripts for model %s from registry", filename);
                    modelData->animScriptStruct = srInitializeAnimScriptsData(filename, aaHeader);
                }
                else {
                    srLogWrite("Creating Animation Scripts for model %s from da file", filename);
                    modelData->animScriptStruct = srGameLoadABFile(&fileContext->lgpContext, filename);
                }
            }
            if (aaHeader->initSkeletonFlag) {
                modelData->skeletonData = srCreateSkeleton(0, 0, aaHeader->modelBoneCount, aaHeader->boneDataPtr, aaHeader, loadStruct, fileContext, filename);
            }
            if (weaponModelID < weaponCount) {
                setSomeFileContext(6, fileContext);
                modelData->weaponModels[weaponModelID] = srCreateSkeleton(1, weaponModelID, 0, aaHeader->boneDataPtr, aaHeader, loadStruct, fileContext, filename);
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

/*typedef BattleModel*(*SRPFNSUB5E1AD2)(i32, i32, LGPLoadCtx*, FileContext*, char*);
#define getBattleModel         ((SRPFNSUB5E1AD2)0x5E1AD2)*/

/*This now saves the model that was set keyed by actor index for later use*/
BattleModel* srSetPlayerModel(i32 a1, i32 a2, u8 modelArrayIdx, char *filename) {
    srLogWrite("Inside srSetPlayerModel");
    LGPLoadCtx lgpLoadCtx; 
    FileContext fileContext; 

    BattleModel* newBattleModel = nullptr;
    clearLoadCtx(&lgpLoadCtx);               // 0x44 in size
    initializeFileContext(&fileContext);
    lgpLoadCtx.field_10 = a1;
    lgpLoadCtx.field_14 = a2;
    if (modelArrayIdx < 3) {
        getSrPartyMember(modelArrayIdx).srPartyMember->modelName = std::string(filename);
    }
    //The following code runs if a model is being swapped
    auto previousModel = getActiveModelFromArray(modelArrayIdx);
    if (previousModel) {
        srDestroyBattleModel(previousModel);
        setActiveModelFromArray(modelArrayIdx, nullptr);
        for (auto partyIdx = 0; partyIdx < 3; ++partyIdx) {
            if (previousModel == getBattleModelState(partyIdx)->modelDataPtr)
                getBattleModelState(partyIdx)->modelDataPtr = nullptr;
        }
    }
    //The following code sets the 7 models that are active for the battle
    newBattleModel = srGetBattleModel(0, modelArrayIdx, &lgpLoadCtx, &fileContext, filename);
    if (newBattleModel)
        setActiveModelFromArray(modelArrayIdx, newBattleModel);
    return newBattleModel;
}

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
                srDestroyBattleModel(battleModel);
                battleModel = srCreateModel(1, 0, modelAAHeader, &loadCtx, &fileCtx, filename);
                destroyModelAAHeader(0, modelAAHeader);
            }
        }
    }
    return battleModel;
}
