#include "animation_registry.h"
#include "../../impl.h"

std::string assembleAnimKey(u16 idx) {
    return std::to_string(idx) + std::string(BASE_PREFIX);
}

/*Constructor takes the IDs of all unique enemies which were loaded into the gContext enemies registry*/
SrBattleAnimationRegistry::SrBattleAnimationRegistry(std::unordered_set<u16> enemyModelIDs) : SrNamedResourceRegistry<SrModelAnimations, std::string>() {

    LGPContext lgpContext = { 0, 1, 2, (PFNFF7MANGLER)0x5E2198 }; //This is used by the game, the name mangler is at 0x5E2198 and called when opening an lgp file
    for (auto modelID : enemyModelIDs) {
        auto name = assembleEnemyModelKey(modelID);
        auto daFilePtr = (u32*)srOpenDAFile(&lgpContext, name.c_str());
        auto totalAnims = daFilePtr[0];
        u32* animDataStartPtr = &(daFilePtr[1]);

        SrModelAnimations modelAnims = SrModelAnimations();
        modelAnims.type = MODEL_TYPE_ENEMY;
        for (auto animationIdx = 0; animationIdx < totalAnims; animationIdx++) {
            auto animHeader = (DaAnimHeader*)animDataStartPtr;
            u32* frameDataPtr = animDataStartPtr + 3;

            auto currentAnimation = createAnimationFromDABuffer(1, animHeader->bonesCount, animHeader->framesCount, frameDataPtr);
            SrAnimation srAnim = { ((12 * animHeader->bonesCount + 24)) * animHeader->framesCount, currentAnimation };
            modelAnims.modelAnimations[assembleAnimKey(animationIdx)] = srAnim;

            animDataStartPtr = &(frameDataPtr[animHeader->compressedSize]);
        }
        modelAnims.totalAnimationCount = totalAnims;
        modelAnims.modelAnimationCount = modelAnims.totalAnimationCount;
        modelAnims.weaponsAnimationCount = 0;
        add_element(name, modelAnims);
    }

    /*Characters are keyed by the exact names the game looks for them by, making it easy for us to fetch their data*/
    for (auto name : characterModelNames) {
        auto daFilePtr = (u32*)srOpenDAFile(&lgpContext, name.c_str());
        auto totalAnims = daFilePtr[0];
        u32* animDataStartPtr = &(daFilePtr[1]);

        SrModelAnimations modelAnims = SrModelAnimations();
        modelAnims.type = MODEL_TYPE_PLAYER;
        for (auto animationIdx = 0; animationIdx < totalAnims; animationIdx++) {
            auto animHeader = (DaAnimHeader*)animDataStartPtr;
            u32* frameDataPtr = animDataStartPtr + 3;

            auto currentAnimation = createAnimationFromDABuffer(1, animHeader->bonesCount, animHeader->framesCount, frameDataPtr);
            SrAnimation srAnim = { (((12 * animHeader->bonesCount) + 24) * animHeader->framesCount), currentAnimation };
            if (animationIdx < BASE_WEAPON_OFFSET) {
                modelAnims.modelAnimations[assembleAnimKey(animationIdx)] = srAnim;
            }
            else {
                modelAnims.weaponAnimations[assembleAnimKey(animationIdx - BASE_WEAPON_OFFSET)] = srAnim;
            }

            animDataStartPtr = &(frameDataPtr[animHeader->compressedSize]);
        }
        modelAnims.totalAnimationCount = totalAnims;
        modelAnims.modelAnimationCount = 0x34;
        modelAnims.weaponsAnimationCount = totalAnims - 0x34;
        add_element(name, modelAnims);
    }

    /*These special models are player models, but they do not have weapons attached*/
    for (auto name : specialModelNames) {
        auto daFilePtr = (u32*)srOpenDAFile(&lgpContext, name.c_str());
        auto totalAnims = daFilePtr[0];
        u32* animDataStartPtr = &(daFilePtr[1]);

        SrModelAnimations modelAnims = SrModelAnimations();
        modelAnims.type = MODEL_TYPE_PLAYER;
        for (auto animationIdx = 0; animationIdx < totalAnims; animationIdx++) {
            auto animHeader = (DaAnimHeader*)animDataStartPtr;
            u32* frameDataPtr = animDataStartPtr + 3;

            auto currentAnimation = createAnimationFromDABuffer(1, animHeader->bonesCount, animHeader->framesCount, frameDataPtr);
            SrAnimation srAnim = { ((12 * animHeader->bonesCount) + 24) * animHeader->framesCount, currentAnimation };
            modelAnims.modelAnimations[assembleAnimKey(animationIdx)] = srAnim;

            animDataStartPtr = &(frameDataPtr[animHeader->compressedSize]);
        }

        modelAnims.totalAnimationCount = totalAnims;
        modelAnims.modelAnimationCount = 0x34;
        modelAnims.weaponsAnimationCount = totalAnims - 0x34;
        add_element(name, modelAnims);
    }
}


void initAnimations() {
    auto enemyModelIDs = gContext.enemies.getUniqueModelIDs();
    gContext.battleAnimations = SrBattleAnimationRegistry(enemyModelIDs);
    srLogWrite("Loaded model animation packs for %lu unique models", (unsigned long)gContext.battleAnimations.resource_count());
}
