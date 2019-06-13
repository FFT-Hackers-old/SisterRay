#include "animation_scripts.h"
#include "../../impl.h"
#include "../../system/ff7memory.h"

std::string assembleAnimScriptKey(u16 idx) {
    return std::to_string(idx) + std::string(BASE_PREFIX);
}

SrModelScripts createSrModelScripts(const std::string archiveName) {
    LGPContext lgpContext = { 0, 1, 2, (PFNFF7MANGLER)0x5E2908 };
    auto abFilePtr = srOpenABFile(&lgpContext, archiveName.c_str());
    ModelABHeader* header = (ModelABHeader*)abFilePtr;

    SrModelScripts srModelScripts = SrModelScripts();
    srModelScripts.header = *header;

    u32* scriptPtrTable = (u32*)(((u8*)abFilePtr) + AB_PTR_TABLE_OFFSET);
    for (auto scriptIdx = 0; scriptIdx < BASE_ENEMY_SCRIPT_MAX; scriptIdx++) {
        u8* animScriptStart = ((u8*)abFilePtr) + scriptPtrTable[scriptIdx];
        u16 animScriptLength = scriptPtrTable[scriptIdx + 1] - scriptPtrTable[scriptIdx];
        auto animationScript = animScriptFromAB(animScriptStart, animScriptLength);
        SrAnimationScript srAnimScript = { animScriptLength, animationScript };
        srModelScripts.modelAnimScripts[assembleAnimScriptKey(scriptIdx)] = srAnimScript;
    }
    ff7freeMemory(abFilePtr, nullptr, 0);
    return srModelScripts;
}

// Initialize, enemy script related data so that we may reassemble in a way the game understands
SrBattleAnimScriptRegistry::SrBattleAnimScriptRegistry(std::unordered_set<u16> enemyModelIDs) : SrNamedResourceRegistry<SrModelScripts, std::string>() {

    LGPContext lgpContext = { 0, 1, 2, (PFNFF7MANGLER)0x5E2198 }; //This is used by the game, the name mangler is at 0x5E2198 and called when opening an lgp file
    srLogWrite("Loading animations for %i enemies", enemyModelIDs.size());
    for (auto modelID : enemyModelIDs) {
        auto name = assembleEnemyModelKey(modelID);
        srLogWrite("Loading model animation scripts from SR registry for model %s constructed from modelID: %i", name.c_str(), modelID);
        SrModelScripts srModelScripts = createSrModelScripts(name);
        add_element(name, srModelScripts);
    }

    for (auto name : characterModelNames) {
        srLogWrite("Loading model animation scripts from SR registry for model %s constructed from modelID: %i", name.c_str());
        SrModelScripts srModelScripts = createSrModelScripts(name);
        add_element(name, srModelScripts);
    }

    for (auto name : specialModelNames) {
        srLogWrite("Loading model animation scripts from SR registry for model %s constructed from modelID: %i", name.c_str());
        SrModelScripts srModelScripts = createSrModelScripts(name);
        add_element(name, srModelScripts);
    }
}

u32 SrBattleAnimScriptRegistry::getMemoryBufferSize(const std::string& name){
    auto scriptData = get_element(name);
    auto scriptCount = 0;
    auto scriptLength = 0;
    for (auto element : scriptData.modelAnimScripts) {
        auto script = element.second;
        scriptLength += script.scriptLength;
        scriptCount++;
    }
    srLogWrite("model %s has %i animations with a total length of %i", name.c_str(), scriptCount, scriptLength);
    return AB_PTR_TABLE_OFFSET + 4 * scriptCount + scriptLength;
}

AnimationScript animScriptFromAB(u8* animScriptStart, u16 animScriptLength) {
    auto animScript = AnimationScript();
    auto scriptPosition = 0;
    while (scriptPosition < animScriptLength) {
        animScript.push_back(*animScriptStart);
        scriptPosition++;
    }
    return animScript;
}

void initAnimationScripts() {
    auto enemyModelIDs = gContext.enemies.getUniqueModelIDs();
    gContext.battleAnimationScripts = SrBattleAnimScriptRegistry(enemyModelIDs);
    srLogWrite("Loaded model scripts for %lu unique models", (unsigned long)gContext.battleAnimations.resource_count());
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
