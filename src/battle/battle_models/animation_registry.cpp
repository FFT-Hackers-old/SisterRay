#include "animation_registry.h"
#include "../../impl.h"

const std::string assembleAnimKey(u16 idx) {
    return std::string(BASE_PREFIX) + std::to_string(idx);
}

/*Creates an SrModleAnimation from a da file archive. It is NOT responsible for the underlying animation, so you must free this if you need*/
SrModelAnimations createSrModelAnimations(SrModelType modelType, const std::string archiveName, bool hasWeapon) {
    LGPContext lgpContext = { 0, 1, 2, (PFNFF7MANGLER)0x5E2198 };
    u32* daFilePtr = (u32*)srOpenDAFile(&lgpContext, archiveName.c_str());
    auto totalAnims = daFilePtr[0];
    u32* animDataStartPtr = &(daFilePtr[1]);

    SrModelAnimations modelAnims = SrModelAnimations();
    modelAnims.type = modelType;
    for (auto animationIdx = 0; animationIdx < totalAnims; animationIdx++) {
        auto animHeader = (DaAnimHeader*)animDataStartPtr;
        u32* frameDataPtr = animDataStartPtr + 3;

        auto currentAnimation = createAnimationFromDABuffer(1, animHeader->bonesCount, animHeader->framesCount, frameDataPtr);
        SrAnimation srAnim = { ((12 * animHeader->bonesCount + 24)) * animHeader->framesCount, currentAnimation };
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

        animDataStartPtr = &(frameDataPtr[animHeader->compressedSize]);
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
    free(daFilePtr);
    return modelAnims;
}

/*Constructor takes the IDs of all unique enemies which were loaded into the gContext enemies registry*/
SrBattleAnimationRegistry::SrBattleAnimationRegistry(std::unordered_set<u16> enemyModelIDs) : SrNamedResourceRegistry<SrModelAnimations, std::string>() {
    for (auto modelID : enemyModelIDs) {
        auto name = assembleEnemyModelKey(modelID);
        SrModelAnimations modelAnims = createSrModelAnimations(MODEL_TYPE_ENEMY, name, false);
        add_element(name, modelAnims);
    }

    for (auto name : characterModelNames) {
        SrModelAnimations modelAnims = createSrModelAnimations(MODEL_TYPE_PLAYER, name, true);
        add_element(name, modelAnims);
    }

    for (auto name : specialModelNames) {
        SrModelAnimations modelAnims = createSrModelAnimations(MODEL_TYPE_PLAYER, name, false);
        add_element(name, modelAnims);
    }
}


void initAnimations() {
    auto enemyModelIDs = gContext.enemies.getUniqueModelIDs();
    gContext.battleAnimations = SrBattleAnimationRegistry(enemyModelIDs);
    srLogWrite("Loaded model animation packs for %lu unique models", (unsigned long)gContext.battleAnimations.resource_count());
}
