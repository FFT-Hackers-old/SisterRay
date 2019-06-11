#include "animation_scripts.h"
#include "../../impl.h"

std::string assembleAnimScriptKey(u16 idx) {
    return std::to_string(idx) + std::string(BASE_PREFIX);
}

// Initialize, enemy script related data so that we may reassemble in a way the game understands
SrBattleAnimScriptRegistry::SrBattleAnimScriptRegistry(std::unordered_set<u16> enemyModelIDs) {

    LGPContext lgpContext = { 0, 1, 2, 0x5E2198 }; //This is used by the game, the name mangler is at 0x5E2198 and called when opening an lgp file
    for (auto modelID : enemyModelIDs) {
        auto name = assembleEnemyModelKey(modelID);
        auto abFilePtr = srGetABFile(lgpContext, name.c_str());
        ModelABHeader* header = (ModelABHeader*)abFilePtr;

        SrModelScripts srModelScripts = SrModelScripts();
        srModelScripts.header = *header;

        u32* scriptPtrTable = (u32*)(((u8*)abFilePtr) + AB_PTR_TABLE_OFFSET);
        for (auto scriptIdx = 0; scriptIdx < BASE_ENEMY_SCRIPT_MAX; scriptIdx++) {
            u8* animScriptStart = ((u8*)abFilePtr) + scriptPtrTable[scriptIdx];
            u16 animScriptLength = scriptPtrTable[scriptIdx + 1] - scriptPtrTable[scriptIdx];
            auto animationScript = animScriptFromAB(animScriptStart, animScriptLength);
            SrAnimationScript srAnimScript = { animScriptLength, animationScript };
            srModelScripts.modelAnimScripts.insert(assembleAnimScriptKey(scriptIdx), srAnimScript);
        }
        add_element(name, srModelScripts)
    }

    for (auto name : characterModelNames) {
        auto abFilePtr = srGetABFile(lgpContext, name.c_str());
        ModelABHeader* header = (ModelABHeader*)abFilePtr;

        SrModelScripts srModelScripts = SrModelScripts();
        srModelScripts.header = *header;

        u32* scriptPtrTable = (u32*)(((u8*)abFilePtr) + AB_PTR_TABLE_OFFSET);
        for (auto scriptIdx = 0; scriptIdx < BASE_PARTY_SCRIPT_MAX; scriptIdx++) {
            u8* animScriptStart = ((u8*)abFilePtr) + scriptPtrTable[scriptIdx];
            u16 animScriptLength = scriptPtrTable[scriptIdx + 1] - scriptPtrTable[scriptIdx];
            auto animationScript = animScriptFromAB(animScriptStart, animScriptLength);
            SrAnimationScript srAnimScript = { animScriptLength, animationScript };
            srModelScripts.modelAnimScripts.insert(assembleAnimScriptKey(scriptIdx), srAnimScript);
        }
        add_element(name, srModelScripts)
    }

    for (auto name : specialModelNames) {
        auto abFilePtr = srGetABFile(lgpContext, name.c_str());
        ModelABHeader* header = (ModelABHeader*)abFilePtr;

        SrModelScripts srModelScripts = SrModelScripts();
        srModelScripts.header = *header;

        u32* scriptPtrTable = (u32*)(((u8*)abFilePtr) + AB_PTR_TABLE_OFFSET);
        for (auto scriptIdx = 0; scriptIdx < BASE_ENEMY_SCRIPT_MAX; scriptIdx++) {
            u8* animScriptStart = ((u8*)abFilePtr) + scriptPtrTable[scriptIdx];
            u16 animScriptLength = scriptPtrTable[scriptIdx + 1] - scriptPtrTable[scriptIdx];
            auto animationScript = animScriptFromAB(animScriptStart, animScriptLength);
            SrAnimationScript srAnimScript = { animScriptLength, animationScript };
            srModelScripts.modelAnimScripts.insert(assembleAnimScriptKey(scriptIdx), srAnimScript);
        }
        add_element(name, srModelScripts)
    }
}

AnimationScript animScriptFromAB(u8* animScriptStart, u16 animScriptLength) {
    auto animScript = AnimationScript();
    auto scriptPosition = 0;
    while (scriptPosition < animScriptLength) {
        animScript.push_back(*animScriptStart);
        scriptPosition++;
    }
    return animScript
}

NewActorAnimScripts::NewActorAnimScripts() {
    transformScript = (u32)&(transformationScript);
}

/*Replace the current animation script function for command index 0x23 with a new one*/
SISTERRAY_API void animationScriptTrampoline(u16 actor_id, u32 ptr_to_anim_scripts, u32 unk1, u32 unk2) {
    u32 ptr_to_new_animation;
    if ((gBigAnimBlock[actor_id].commandID == CMD_LIMIT) && (gBigAnimBlock[actor_id].commandAnimID == 0x3D)) {
        srLogWrite("Redirecting animation script execution for actor %i", actor_id);
        srLogWrite("Writing to address %p, base %p", &gBigAnimBlock[actor_id].commandAnimID, &gBigAnimBlock[0].commandAnimID);
        ptr_to_new_animation = (u32)&(actorAnimArray);
        srLogWrite("actor animation address:%p, passed address %p", &actorAnimArray, ptr_to_new_animation);

    }
    else {
        ptr_to_new_animation = (u32)ptr_to_anim_scripts;
    }

    oldRunAnimationScript(actor_id, (u32)ptr_to_new_animation, unk1, unk1);
}

void srExecuteAnimationScript(u16 actor_id, u32 ptr_to_anim_scripts, u32 unk1, u32 unk2) {

}
