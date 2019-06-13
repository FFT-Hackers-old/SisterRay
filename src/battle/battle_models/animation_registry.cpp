#include "animation_registry.h"
#include "../../impl.h"
#include "../../system/ff7memory.h"
#include "../../files/lgp_loader.h"

const std::string assembleAnimKey(u16 idx) {
    return std::string(BASE_PREFIX) + std::to_string(idx);
}

/*Creates an SrModleAnimation from a da file archive. It is NOT responsible for the underlying animation, so you must free this if you need*/
SrModelAnimations createSrModelAnimations(SrModelType modelType, const std::string archiveName, bool hasWeapon, u8* battleLGP) {
    LGPContext lgpContext = { 0, 1, 2, (PFNFF7MANGLER)0x5E2908 };
    char mangledName[32];
    lgpContext.mangler(archiveName.c_str(), &(mangledName[0]));
    srLogWrite("mangled name: %s", mangledName);
    LGPArchiveFile archiveFile = lgpArchiveRead(battleLGP, mangledName);
    u32* daFilePtr = (u32*)(archiveFile.buffer);
    //u32* daFilePtr = (u32*)srOpenDAFile(&lgpContext, archiveName.c_str());
    srLogWrite("daFilePtr successfully opened at %p, first bytes: %c, %c, %c, %c, %c, size: %i", daFilePtr, daFilePtr[0], daFilePtr[1], daFilePtr[2], daFilePtr[3], daFilePtr[4], archiveFile.size);
    auto totalAnims = daFilePtr[0];
    u32* animDataStartPtr = &(daFilePtr[1]);

    SrModelAnimations modelAnims = SrModelAnimations();
    modelAnims.type = modelType;
    for (auto animationIdx = 0; animationIdx < totalAnims; animationIdx++) {
        auto animHeader = (DaAnimHeader*)animDataStartPtr;
        u8* frameDataPtr = (u8*)(animDataStartPtr + 3);
        //srLogWrite("location of frame data at iteration %i: %p", animationIdx, frameDataPtr);
        auto currentAnimation = createAnimationFromDABuffer(1, animHeader->bonesCount, animHeader->framesCount, (u32*)frameDataPtr);
        u32 animSize = ((12 * animHeader->bonesCount + 24)) * animHeader->framesCount;
        //srLogWrite("size of animation: %i", animSize);
        //srLogWrite("loading animation %i, values:%i, %i, %i, %p, %p", animationIdx, currentAnimation->alwaysOne, currentAnimation->BonesCount, currentAnimation->frameCount, currentAnimation->frameDataView, currentAnimation->rawAnimationDataBuffer);
        SrAnimation srAnim = { animSize, currentAnimation };
        if (hasWeapon) {
            if (animationIdx < BASE_WEAPON_OFFSET) {
                modelAnims.modelAnimations[assembleAnimKey(animationIdx)] = srAnim;
            }
            else {
                modelAnims.weaponAnimations[assembleAnimKey(animationIdx - BASE_WEAPON_OFFSET)] = srAnim;
            }
        }
        else {
            modelAnims.modelAnimations[assembleAnimKey(animationIdx)] = srAnim;
        }
        //srLogWrite("Incrementing header pointer to %p by %i", animDataStartPtr, animHeader->compressedSize);
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
    ff7freeMemory(daFilePtr, nullptr, 0);
    return modelAnims;
}

/*Constructor takes the IDs of all unique enemies which were loaded into the gContext enemies registry*/
SrBattleAnimationRegistry::SrBattleAnimationRegistry(std::unordered_set<u16> enemyModelIDs) : SrNamedResourceRegistry<SrModelAnimations, std::string>() {
    auto battleLGP = readLGPArchive(BATTLE_LGP_PATH);
    auto battlelgpHeader = (LGPHeader*)battleLGP;
    srLogWrite("lgpArchive loaded, name: %s, filecount:%i", battlelgpHeader->name, battlelgpHeader->fileCount);
    srLogWrite("Loading animations for %i enemies", enemyModelIDs.size());
    for (auto modelID : enemyModelIDs) {
        auto name = assembleEnemyModelKey(modelID);
        srLogWrite("Loading model animations from SR registry for model %s constructed from modelID: %i", name.c_str(), modelID);
        SrModelAnimations modelAnims = createSrModelAnimations(MODEL_TYPE_ENEMY, name, false, (u8*)battleLGP);
        add_element(name, modelAnims);
    }
    for (auto name : characterModelNames) {

        srLogWrite("Loading model animation from SR registry for model %s", name.c_str());
        SrModelAnimations modelAnims = createSrModelAnimations(MODEL_TYPE_PLAYER, name, true, (u8*)battleLGP);
        add_element(name, modelAnims);
    }
    for (auto name : specialModelNames) {
        srLogWrite("Loading model animation from SR registry for model %s", name.c_str());
        SrModelAnimations modelAnims = createSrModelAnimations(MODEL_TYPE_PLAYER, name, false, (u8*)battleLGP);
        add_element(name, modelAnims);
    }
}


void initAnimations() {
    auto enemyModelIDs = gContext.enemies.getUniqueModelIDs();
    gContext.battleAnimations = SrBattleAnimationRegistry(enemyModelIDs);
    srLogWrite("Loaded model animation packs for %lu unique models", (unsigned long)gContext.battleAnimations.resource_count());
}
