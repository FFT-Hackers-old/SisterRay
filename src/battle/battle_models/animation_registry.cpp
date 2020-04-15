#include "animation_registry.h"
#include "../../impl.h"
#include "../../system/ff7memory.h"
#include "../../files/lgp_loader.h"
#include <unordered_map>

//Might just changed to a vector data structure
const std::string assembleAnimKey(u16 idx) {
    std::string numString;
    if (idx < 10) {
        numString = std::string("00") + std::to_string(idx);
    }
    else if (idx < 100) {
        numString = std::string("0") + std::to_string(idx);
    }
    else {

        numString = std::to_string(idx);
    }
    return numString + std::string(BASE_PREFIX);
}

/*Creates an SrModleAnimation from a da file archive. It is NOT responsible for the underlying animation, so you must free this if you need*/
SrModelAnimations createSrModelAnimations(SrModelType modelType, const std::string archiveName, bool hasWeapon, u8* battleLGP) {
    LGPContext lgpContext = { 0, 1, 2, (PFNFF7MANGLER)0x5E2908 };
    LGPArchiveFile archiveFile = srOpenDAFile(&lgpContext, archiveName.c_str(), (void*)battleLGP);
    u32* daFilePtr = (u32*)(archiveFile.buffer);
    //u32* daFilePtr = (u32*)srOpenDAFile(&lgpContext, archiveName.c_str());
    srLogWrite("daFilePtr successfully opened at %p, first bytes: %x, %x, %x, %x, %x, size: %i", daFilePtr, daFilePtr[0], daFilePtr[1], daFilePtr[2], daFilePtr[3], daFilePtr[4], archiveFile.size);
    auto totalAnims = daFilePtr[0];
    u32* animDataStartPtr = &(daFilePtr[1]);

    SrModelAnimations modelAnims = SrModelAnimations();
    modelAnims.type = modelType;
    for (u32 animationIdx = 0; animationIdx < totalAnims; animationIdx++) {
        auto animHeader = (DaAnimHeader*)animDataStartPtr;
        u8* frameDataPtr = (u8*)(animDataStartPtr + 3);
        auto currentAnimation = createAnimationFromDABuffer(1, animHeader->bonesCount, animHeader->framesCount, (u32*)frameDataPtr);
        u32 animSize = ((12 * (animHeader->bonesCount - 1)) + 24) * animHeader->framesCount;
        SrAnimation srAnim = { animSize, currentAnimation };
        if (hasWeapon) {
            if (animationIdx < BASE_WEAPON_OFFSET) {
                modelAnims.modelAnimations.addElement(assembleAnimKey(animationIdx), srAnim);
            }
            else {
                modelAnims.weaponAnimations.addElement(assembleAnimKey(animationIdx - BASE_WEAPON_OFFSET), srAnim);
            }
        }
        else {
            modelAnims.modelAnimations.addElement(assembleAnimKey(animationIdx), srAnim);
        }
        animDataStartPtr = (u32*)&(frameDataPtr[animHeader->compressedSize]);
    }
    if (hasWeapon) {
        modelAnims.totalAnimationCount = totalAnims;
        modelAnims.modelAnimationCount = 0x34;
        modelAnims.weaponsAnimationCount = totalAnims - 0x34;
        return modelAnims;
    }
    modelAnims.totalAnimationCount = totalAnims;
    modelAnims.modelAnimationCount = modelAnims.totalAnimationCount;
    modelAnims.weaponsAnimationCount = 0;
    return modelAnims;
}

/*Constructor takes the IDs of all unique enemies which were loaded into the gContext enemies registry*/
SrBattleAnimationRegistry::SrBattleAnimationRegistry(std::unordered_set<u16> enemyModelIDs, void* battleLGPBuffer) : SrNamedResourceRegistry<SrModelAnimations, std::string>() {
    auto battlelgpHeader = (LGPHeader*)battleLGPBuffer;
    //srLogWrite("lgpArchive loaded, name: %s, filecount:%i", battlelgpHeader->name, battlelgpHeader->fileCount);
    //srLogWrite("Loading animations for %i enemies", enemyModelIDs.size());
    for (auto modelID : enemyModelIDs) {
        auto name = assembleEnemyModelKey(modelID);
        SrModelAnimations modelAnims = createSrModelAnimations(MODEL_TYPE_ENEMY, name, false, (u8*)battleLGPBuffer);
        addElement(name, modelAnims);
    }
    for (auto name : characterModelNames) {
        SrModelAnimations modelAnims = createSrModelAnimations(MODEL_TYPE_PLAYER, name, true, (u8*)battleLGPBuffer);
        addElement(name, modelAnims);
    }
    for (auto name : specialModelNames) {
        SrModelAnimations modelAnims = createSrModelAnimations(MODEL_TYPE_PLAYER, name, false, (u8*)battleLGPBuffer);
        addElement(name, modelAnims);
    }
}


void initAnimations(void* battleLGPBuffer) {
    auto enemyModelIDs = gContext.enemies.getUniqueModelIDs();
    gContext.battleAnimations = SrBattleAnimationRegistry(enemyModelIDs, battleLGPBuffer);
    loadNewAnimations();
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
    auto& modelAnimations = gContext.battleAnimations.getElement(modelName);
    auto name = std::string(modName) + std::to_string(modIdx);
    modelAnimations.modelAnimations.addElement(name, animation);
    modelAnimations.totalAnimationCount++;
}


SISTERRAY_API void addPlayerModelAnimation(const char* modName, u16 modIdx, const char* modelName, const SrAnimation animation, const SrAnimation weaponAnimation) {
    auto& modelAnimations = gContext.battleAnimations.getElement(modelName);
    auto name = std::string(modName) + std::to_string(modIdx);
    modelAnimations.modelAnimations.addElement(name, animation);
    modelAnimations.weaponAnimations.addElement(name, weaponAnimation);
    modelAnimations.totalAnimationCount++;
}

std::vector<std::unordered_map<std::string, SrAnimation>> loadModelAnimationFromDAFile(const char* modelName, void* daFileBuffer, bool hasWeapon) {
    u32* daFilePtr = (u32*)(daFileBuffer);
    //u32* daFilePtr = (u32*)srOpenDAFile(&lgpContext, archiveName.c_str());
    srLogWrite("daFilePtr successfully opened at %p, first bytes: %x, %x, %x, %x, %x, size: %i", daFilePtr, daFilePtr[0], daFilePtr[1], daFilePtr[2], daFilePtr[3], daFilePtr[4]);
    auto totalAnims = daFilePtr[0];
    u32* animDataStartPtr = &(daFilePtr[1]);

    std::vector<std::unordered_map<std::string, SrAnimation>> parsedAnimations;

    for (u32 animationIdx = 0; animationIdx < totalAnims; animationIdx++) {
        auto animHeader = (DaAnimHeader*)animDataStartPtr;
        u8* frameDataPtr = (u8*)(animDataStartPtr + 3);
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

void loadNewAnimations() {
}
