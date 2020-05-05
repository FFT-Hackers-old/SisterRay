#include "animation_registry.h"
#include "../../impl.h"
#include "../../system/ff7memory.h"
#include "../../files/lgp_loader.h"
#include "battle_models_api.h"
#include <unordered_map>

//Might just changed to a vector data structure
const std::string assembleAnimKey(u16 idx) {
    return std::string(BASE_PREFIX) + std::to_string(idx);
}

/*Creates an SrModleAnimation from a da file archive. It is NOT responsible for the underlying animation, so you must free this if you need*/
SrModelAnimations createSrModelAnimations(SrModelType modelType, const std::string archiveName, bool hasWeapon, u8* battleLGP, u8* magicLGP) {
    LGPContext lgpContext = { 0, 1, 2, (PFNFF7MANGLER)0x5E2908 };
    LGPArchiveFile archiveFile = srOpenDAFile(&lgpContext, archiveName.c_str(), (void*)battleLGP);
    u32* daFilePtr = (u32*)(archiveFile.buffer);
    //u32* daFilePtr = (u32*)srOpenDAFile(&lgpContext, archiveName.c_str());
    srLogWrite("daFilePtr successfully opened at %p, first bytes: %x, %x, %x, %x, %x, size: %i", daFilePtr, daFilePtr[0], daFilePtr[1], daFilePtr[2], daFilePtr[3], daFilePtr[4], archiveFile.size);
    auto totalAnims = daFilePtr[0];
    u32* animDataStartPtr = &(daFilePtr[1]);

    SrModelAnimations modelAnims = SrModelAnimations();
    modelAnims.type = modelType;
    auto trueModelCount = totalAnims - 0x34;
    for (u32 animationIdx = 0; animationIdx < totalAnims; animationIdx++) {
        auto animHeader = (DaAnimHeader*)animDataStartPtr;
        u8* frameDataPtr = (u8*)(animDataStartPtr + 3);
        auto currentAnimation = createAnimationFromDABuffer(1, animHeader->bonesCount, animHeader->framesCount, (u32*)frameDataPtr);
        u32 animSize = ((12 * (animHeader->bonesCount - 1)) + 24) * animHeader->framesCount;
        SrAnimation srAnim = { animSize, currentAnimation };
        if (hasWeapon) {
            if (animationIdx < trueModelCount) {
                modelAnims.modelAnimations.addElement(assembleAnimKey(animationIdx), srAnim);
            }
            else if (animationIdx >= BASE_WEAPON_OFFSET) {
                modelAnims.weaponAnimations.addElement(assembleAnimKey(animationIdx - BASE_WEAPON_OFFSET), srAnim);
            }
        }
        else {
            modelAnims.modelAnimations.addElement(assembleAnimKey(animationIdx), srAnim);
        }
        animDataStartPtr = (u32*)&(frameDataPtr[animHeader->compressedSize]);
    }

    if (hasWeapon) {
        modelAnims.totalAnimationCount = 2 * trueModelCount;
        modelAnims.modelAnimationCount = trueModelCount;
        modelAnims.weaponsAnimationCount = trueModelCount;
        return modelAnims;
    }
    modelAnims.totalAnimationCount = totalAnims;
    modelAnims.modelAnimationCount = modelAnims.totalAnimationCount;
    modelAnims.weaponsAnimationCount = 0;
    return modelAnims;
}

/*Constructor takes the IDs of all unique enemies which were loaded into the gContext enemies registry*/
SrBattleAnimationRegistry::SrBattleAnimationRegistry(std::unordered_set<u16> enemyModelIDs, void* battleLGPBuffer, void* magicLGPBuffer) : SrNamedResourceRegistry<SrModelAnimations, std::string>() {
    auto battlelgpHeader = (LGPHeader*)battleLGPBuffer;
    //srLogWrite("lgpArchive loaded, name: %s, filecount:%i", battlelgpHeader->name, battlelgpHeader->fileCount);
    //srLogWrite("Loading animations for %i enemies", enemyModelIDs.size());
    for (auto modelID : enemyModelIDs) {
        auto name = assembleEnemyModelKey(modelID);
        SrModelAnimations modelAnims = createSrModelAnimations(MODEL_TYPE_ENEMY, name, false, (u8*)battleLGPBuffer, (u8*)magicLGPBuffer);
        addElement(name, modelAnims);
    }
    for (auto name : characterModelNames) {
        SrModelAnimations modelAnims = createSrModelAnimations(MODEL_TYPE_PLAYER, name, true, (u8*)battleLGPBuffer, (u8*)magicLGPBuffer);
        addElement(name, modelAnims);
    }
    for (auto name : specialModelNames) {
        SrModelAnimations modelAnims = createSrModelAnimations(MODEL_TYPE_PLAYER, name, false, (u8*)battleLGPBuffer, (u8*)magicLGPBuffer);
        addElement(name, modelAnims);
    }

}


void initAnimations(void* battleLGPBuffer, void* magicLGPBuffer) {
    auto enemyModelIDs = gContext.enemies.getUniqueModelIDs();
    gContext.battleAnimations = SrBattleAnimationRegistry(enemyModelIDs, battleLGPBuffer, magicLGPBuffer);
    srLogWrite("Loaded model animation packs for %lu unique models", (unsigned long)gContext.battleAnimations.resourceCount());
}


void srInitializeAnimationsTable(void** animationDataTable, u16 tableSize, const char* filename, ModelAAHeader* aaHeader) {
    auto animationData = gContext.battleAnimations.getElement(std::string(filename));
    auto totalAnims = animationData.totalAnimationCount;
    auto weaponsAnimsCount = animationData.weaponsAnimationCount;
    auto modelAnimsCount = animationData.modelAnimationCount;
    auto& animations = animationData.modelAnimations;
    auto& weaponAnimations = animationData.weaponAnimations;

    u32 tableIdx = 0;
    /*Copy every model animation, in order, into the buffer*/
    for (auto srAnim : animations) {
        u32 rawBufferSize = srAnim.rawBufferSize;
        void* newAnimBuffer = srCreateBattleAnimation(srAnim.animationData->frameCount, srAnim.animationData->BonesCount, srAnim.animationData->unkDword);
        BattleAnimation* headerView = (BattleAnimation*)newAnimBuffer;
        auto frameDataTable = headerView->frameDataView;
        memcpy(headerView->rawAnimationDataBuffer, (void*)srAnim.animationData->rawAnimationDataBuffer, rawBufferSize);
        animationDataTable[tableIdx] = newAnimBuffer;
        tableIdx++;
        if (tableIdx > tableSize) {
            srLogWrite("ERROR: Assigning animation to invalid index for model %s", filename);
        }
    }
    srLogWrite("MODEL %s initialized with %i animations in table, last index: %i", filename, tableIdx - 1);
    srLogWrite("MODEL %s weapon initialization beginning, first index: %i", filename, tableIdx);
    if (aaHeader->weaponCount) {
        for (auto srAnim : weaponAnimations) {
            u32 rawBufferSize = srAnim.rawBufferSize;
            void* newAnimBuffer = srCreateBattleAnimation(srAnim.animationData->frameCount, srAnim.animationData->BonesCount, srAnim.animationData->unkDword);
            BattleAnimation* headerView = (BattleAnimation*)newAnimBuffer;
            auto frameDataTable = headerView->frameDataView;
            memcpy(headerView->rawAnimationDataBuffer, (void*)srAnim.animationData->rawAnimationDataBuffer, rawBufferSize);
            animationDataTable[tableIdx] = newAnimBuffer;
            tableIdx++;
            if (tableIdx > tableSize) {
                srLogWrite("ERROR: Assigning animation to invalid index for model %s", filename);
            }
        }
    }
    srLogWrite("initialized %i total animations for model %s", tableIdx, filename);
}


SISTERRAY_API void addModelAnimation(const char* modName, u16 modIdx, const char* modelName, const SrAnimation animation) {
    if (!gContext.battleAnimations.contains(modelName)) {
        srLogWrite("MODEL: %s not found in registry, default constructing", modelName);
        auto modelAnimations = SrModelAnimations(); //construct the model in place if it is not there
        modelAnimations.totalAnimationCount = 0;
        modelAnimations.modelAnimationCount = 0;
        modelAnimations.weaponsAnimationCount = 0;
        modelAnimations.type = MODEL_TYPE_ENEMY;
        gContext.battleAnimations.addElement(modelName, modelAnimations);
    }
    auto& modelAnimations = gContext.battleAnimations.getElement(modelName); //construct the model in place if it is not there
    auto name = std::string(modName) + std::to_string(modIdx);
    srLogWrite("Adding animation to model %s with bones: %i and frame count: %i", modelName, animation.animationData->BonesCount, animation.animationData->frameCount);
    modelAnimations.modelAnimations.addElement(name, animation);
    modelAnimations.modelAnimationCount++;
    modelAnimations.totalAnimationCount++;
}


SISTERRAY_API void addPlayerModelAnimation(const char* modName, u16 modIdx, const char* modelName, const SrAnimation animation, const SrAnimation weaponAnimation) {
    auto& modelAnimations = gContext.battleAnimations.getElement(modelName);
    auto name = std::string(modName) + std::to_string(modIdx);
    modelAnimations.modelAnimations.addElement(name, animation);
    modelAnimations.weaponAnimations.addElement(name, weaponAnimation);
    modelAnimations.modelAnimationCount++;
    modelAnimations.weaponsAnimationCount++;
    modelAnimations.totalAnimationCount += 2;
    srLogWrite("Adding animation for mod %s, idx: %i, trueIdx: %i",  modName, modIdx, modelAnimations.modelAnimations.getResourceIndex(name));
}


SISTERRAY_API u16 getSrPlayerAnimationIdx(const char* modName, u16 relativeModIdx, const char* modelName) {
    auto& modelAnimations = gContext.battleAnimations.getElement(modelName);
    auto name = std::string(modName) + std::to_string(relativeModIdx);
    srLogWrite("Fetching animation with True Idx: %i for model %s", modelAnimations.modelAnimations.getResourceIndex(name), modelName);
    return modelAnimations.modelAnimations.getResourceIndex(name);
}


std::vector<std::unordered_map<std::string, SrAnimation>> loadModelAnimationFromDAFile(const char* modelName, void* daFileBuffer, bool hasWeapon) {
    u32* daFilePtr = (u32*)(daFileBuffer);
    //u32* daFilePtr = (u32*)srOpenDAFile(&lgpContext, archiveName.c_str());
    srLogWrite("daFilePtr successfully opened at %p, first bytes: %x, %x, %x, %x, %x", daFilePtr, daFilePtr[0], daFilePtr[1], daFilePtr[2], daFilePtr[3], daFilePtr[4]);
    auto totalAnims = daFilePtr[0];
    u32* animDataStartPtr = &(daFilePtr[1]);

    std::vector<std::unordered_map<std::string, SrAnimation>> parsedAnimations;
    for (u32 animationIdx = 0; animationIdx < totalAnims; animationIdx++) {
        auto animHeader = (DaAnimHeader*)animDataStartPtr;
        u8* frameDataPtr = (u8*)(animDataStartPtr + 3);
        srLogWrite("Loading animation with bones: %i, frame count: %i", animHeader->bonesCount, animHeader->framesCount);
        auto currentAnimation = createAnimationFromDABuffer(1, animHeader->bonesCount, animHeader->framesCount, (u32*)frameDataPtr);
        u32 animSize = ((12 * (animHeader->bonesCount - 1)) + 24) * animHeader->framesCount;
        SrAnimation srAnim = { animSize, currentAnimation };
        std::unordered_map<std::string, SrAnimation> animMap;
        if (hasWeapon) {
            if (animationIdx < 8) {
                animMap["BASE"] = srAnim;
                parsedAnimations.push_back(animMap);
            }
            else {
                parsedAnimations[animationIdx - 8]["WPN"] = srAnim;
            }
        }
        else {
            animMap["BASE"] = srAnim;
            parsedAnimations.push_back(animMap);
        }
        animDataStartPtr = (u32*)&(frameDataPtr[animHeader->compressedSize]);
    }
    return parsedAnimations;
}
