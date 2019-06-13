#include "animation_registry.h"
#include "../../impl.h"
#include "../../system/ff7memory.h"

const std::string assembleAnimKey(u16 idx) {
    return std::string(BASE_PREFIX) + std::to_string(idx);
}

/*Creates an SrModleAnimation from a da file archive. It is NOT responsible for the underlying animation, so you must free this if you need*/
SrModelAnimations createSrModelAnimations(SrModelType modelType, const std::string archiveName, bool hasWeapon) {
    LGPContext lgpContext = { 0, 1, 2, (PFNFF7MANGLER)0x5E2908 };
    //srLogWrite("lgp context initialized");
    u32* daFilePtr = (u32*)srOpenDAFile(&lgpContext, archiveName.c_str());
    //srLogWrite("daFilePtr successfully opened");
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
    srLogWrite("Loading animations for %i enemies", enemyModelIDs.size());
    for (auto modelID : enemyModelIDs) {
        auto name = assembleEnemyModelKey(modelID);
        srLogWrite("Loading model animations from SR registry for model %s constructed from modelID: %i", name.c_str(), modelID);
        SrModelAnimations modelAnims = createSrModelAnimations(MODEL_TYPE_ENEMY, name, false);
        add_element(name, modelAnims);
    }
    for (auto name : characterModelNames) {

        srLogWrite("Loading model animation from SR registry for model %s", name.c_str());
        SrModelAnimations modelAnims = createSrModelAnimations(MODEL_TYPE_PLAYER, name, true);
        add_element(name, modelAnims);
    }
    for (auto name : specialModelNames) {
        srLogWrite("Loading model animation from SR registry for model %s", name.c_str());
        SrModelAnimations modelAnims = createSrModelAnimations(MODEL_TYPE_PLAYER, name, false);
        add_element(name, modelAnims);
    }
}


void initAnimations() {
    auto enemyModelIDs = gContext.enemies.getUniqueModelIDs();
    gContext.battleAnimations = SrBattleAnimationRegistry(enemyModelIDs);
    srLogWrite("Loaded model animation packs for %lu unique models", (unsigned long)gContext.battleAnimations.resource_count());
}
