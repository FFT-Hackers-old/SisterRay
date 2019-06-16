#include "animation_scripts.h"
#include "../../impl.h"
#include "../../system/ff7memory.h"

std::string assembleAnimScriptKey(u16 idx) {
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

SrModelScripts createSrModelScripts(SrModelType modelType, const std::string archiveName, void* battleLGPBuffer) {
    LGPContext lgpContext = { 0, 1, 2, (PFNFF7MANGLER)0x5E2908 };
    LGPArchiveFile archiveFile = srOpenABFile(&lgpContext, archiveName.c_str(), (void*)battleLGPBuffer);
    u8* abFilePtr = (u8*)(archiveFile.buffer);
    ModelABHeader* header = (ModelABHeader*)abFilePtr;
    u16 animScriptLength;
    u16 animScriptCount;

    SrModelScripts srModelScripts = SrModelScripts();
    srModelScripts.header = *header;

    u8* headerEnd = abFilePtr + AB_PTR_TABLE_OFFSET;
    u32* scriptPtrTable = (u32*)(headerEnd);
    if (modelType == MODEL_TYPE_PLAYER) {
        animScriptCount = BASE_PARTY_SCRIPT_MAX;
    }
    else {
        animScriptCount = BASE_ENEMY_SCRIPT_MAX;
    }
    for (auto scriptIdx = 0; scriptIdx < animScriptCount; scriptIdx++) {
        u8* animScriptStart = ((u8*)abFilePtr) + scriptPtrTable[scriptIdx];
        srLogWrite("animation script at idx: %i loaded with characters: %x, %x, %x, %x, %x", scriptIdx, animScriptStart[0], animScriptStart[1], animScriptStart[2], animScriptStart[3], animScriptStart[4]);
        if ((modelType == MODEL_TYPE_ENEMY) && scriptIdx == 31) {
            animScriptLength = 32;
        }
        else if ((modelType == MODEL_TYPE_PLAYER) && scriptIdx == 73) {
            animScriptLength = 32;
        }

        int difference = (scriptPtrTable[scriptIdx + 1] > scriptPtrTable[scriptIdx]);
        if (difference > 0) {
            animScriptLength = scriptPtrTable[scriptIdx + 1] - scriptPtrTable[scriptIdx];
        }
        else {
            srLogWrite("length heurestic returning negative length, defaulting to 32", scriptIdx);
            animScriptLength = 32;
        }
        if (animScriptLength) {
            if (animScriptStart[0] == 0 && animScriptStart[1] == 0) {
                srLogWrite("Null script detected, redirecting to idle");
                u16 trueAnimScriptLength = 4;
                auto animationScript = animScriptFromAB((((u8*)abFilePtr) + scriptPtrTable[0]), animScriptLength, &trueAnimScriptLength);
                SrAnimationScript srAnimScript = { trueAnimScriptLength, animationScript };
                u8* scriptPtr = srAnimScript.animScript.data();
                srLogWrite("redirected script at idx: %i loaded with characters: %x, %x, %x, %x, %x", scriptIdx, scriptPtr[0], scriptPtr[1], scriptPtr[2], scriptPtr[3], scriptPtr[4]);
                srModelScripts.modelAnimScripts[assembleAnimScriptKey(scriptIdx)] = srAnimScript;
                continue;
            }
            u16 trueAnimScriptLength = animScriptLength;
            auto animationScript = animScriptFromAB(animScriptStart, animScriptLength, &trueAnimScriptLength);
            SrAnimationScript srAnimScript = { trueAnimScriptLength, animationScript };
            u8* scriptPtr = srAnimScript.animScript.data();
            srLogWrite("animation script at idx: %i loaded with characters: %x, %x, %x, %x, %x", scriptIdx, scriptPtr[0], scriptPtr[1], scriptPtr[2], scriptPtr[3], scriptPtr[4]);
            srModelScripts.modelAnimScripts[assembleAnimScriptKey(scriptIdx)] = srAnimScript;
        }
    }
    srModelScripts.scriptCount = animScriptCount;
    return srModelScripts;
}

// Initialize, enemy script related data so that we may reassemble in a way the game understands
SrBattleAnimScriptRegistry::SrBattleAnimScriptRegistry(std::unordered_set<u16> enemyModelIDs, void* battleLGPBuffer) : SrNamedResourceRegistry<SrModelScripts, std::string>() {
    LGPContext lgpContext = { 0, 1, 2, (PFNFF7MANGLER)0x5E2198 }; //This is used by the game, the name mangler is at 0x5E2198 and called when opening an lgp file
    srLogWrite("Loading animations for %i enemies", enemyModelIDs.size());
    for (auto modelID : enemyModelIDs) {
        auto name = assembleEnemyModelKey(modelID);
        srLogWrite("Loading model animation scripts from SR registry for model %s constructed from modelID: %i", name.c_str(), modelID);
        SrModelScripts srModelScripts = createSrModelScripts(MODEL_TYPE_ENEMY, name, battleLGPBuffer);
        add_element(name, srModelScripts);
    }
    for (auto name : characterModelNames) {
        srLogWrite("Loading model animation scripts from SR registry for model %s", name.c_str());
        SrModelScripts srModelScripts = createSrModelScripts(MODEL_TYPE_PLAYER, name, battleLGPBuffer);
        add_element(name, srModelScripts);
    }
    for (auto name : specialModelNames) {
        srLogWrite("Loading model animation scripts from SR registry for model %s", name.c_str());
        SrModelScripts srModelScripts = createSrModelScripts(MODEL_TYPE_PLAYER, name, battleLGPBuffer);
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

AnimationScript animScriptFromAB(u8* animScriptStart, u16 animScriptLength, u16* trueScriptLength) {
    auto animScript = AnimationScript();
    auto scriptPosition = 0;
    srLogWrite("reading script with suggested length %i, starting at %p", animScriptLength, animScriptStart);
    while (scriptPosition < animScriptLength) {
        srLogWrite("adding byte %X to script", animScriptStart[scriptPosition]);
        animScript.push_back(animScriptStart[scriptPosition]);
        if (animScriptStart[scriptPosition] == 0xEE || animScriptStart[scriptPosition] == 0xFF || ((animScriptStart[scriptPosition - 1] == 0xFE) && (animScriptStart[scriptPosition] == 0xC0))) {
            *trueScriptLength = scriptPosition + 1;
            srLogWrite("ACTUAL LENGTH: %i", *trueScriptLength);
            break;
        }
        scriptPosition++;
    }
    return animScript;
}

void initAnimationScripts(void* battleLGPBuffer) {
    auto enemyModelIDs = gContext.enemies.getUniqueModelIDs();
    gContext.battleAnimationScripts = SrBattleAnimScriptRegistry(enemyModelIDs, battleLGPBuffer);
    srLogWrite("Loaded model scripts for %lu unique models", (unsigned long)gContext.battleAnimations.resource_count());
}


void* srInitializeAnimScriptsData(const char* filename, ModelAAHeader* aaHeader) {
    auto animScriptsData = gContext.battleAnimationScripts.get_element(std::string(filename));
    auto bufferSize = gContext.battleAnimationScripts.getMemoryBufferSize(std::string(filename));
    u8* animScriptBuffer = (u8*)ff7allocateMemory(1, bufferSize, nullptr, 0);
    u8** tableBufferPtr = (u8**)(animScriptBuffer + (AB_PTR_TABLE_OFFSET)); //advance this ptr to the start of the animation data table
    srLogWrite("allocating a table for script count: %i with a total buffer size of %i", animScriptsData.scriptCount, bufferSize);
    u8* scriptBufferPtr = (u8*)(animScriptBuffer + (AB_PTR_TABLE_OFFSET + (animScriptsData.scriptCount * 4))); //advance this tpr to the start of the raw animation script data
    /*Copy the header data from the ba file*/
    memcpy(animScriptBuffer, (void*)&(animScriptsData.header), AB_PTR_TABLE_OFFSET);
    srLogWrite("Initial pointers state: head: %p, table: %p, scriptBuffer: %p", animScriptBuffer, tableBufferPtr, scriptBufferPtr);

    /*Initialize the actual form of the animation scripts data in memory from the ba file data*/
    auto scriptIdx = 0;
    for (auto animScriptElement : animScriptsData.modelAnimScripts) {
        tableBufferPtr[scriptIdx] = (u8*)(scriptBufferPtr - animScriptBuffer); // use the relative index like in the file for the time being
        srLogWrite("writing animation script: %s", animScriptElement.first.c_str());
        srLogWrite("script ptr value: %p for tableEntry at %p", tableBufferPtr[scriptIdx], &(tableBufferPtr[scriptIdx]));
        auto animScript = animScriptElement.second;
        memcpy((void*)scriptBufferPtr, (void*)animScript.animScript.data(), animScript.scriptLength);
        scriptBufferPtr += animScript.scriptLength;
        scriptIdx++;
    }
    srLogWrite("loaded all animations scripts for model %s -- total: %i", filename, scriptIdx);
    return animScriptBuffer;
}

NewActorAnimScripts::NewActorAnimScripts() {
    transformScript = (u32)&(transformationScript);
}

/*Replace the current animation script function for command index 0x23 with a new one*/
SISTERRAY_API void animationScriptTrampoline(u16 actor_id, u32 ptr_to_anim_scripts, u32 unk1, u32 unk2) {
    u32* modelDatPtrArray = (u32*)0x0BFB2B8;
    u32 ptr_to_new_animation;
    u8** tablePtr = (u8**)ptr_to_anim_scripts;
    srLogWrite("bigblock size:%i", (sizeof(BattleModelState)));
    u16* byteViewAnimBlock = (u16*)&(gBigAnimBlock[actor_id].actorID);
    srLogWrite("ANIMATION SCRIPT RUN CALLED for actor %i with command ID:%i and animScriptID:%i", actor_id, gBigAnimBlock[actor_id].commandID, gBigAnimBlock[actor_id].animScriptID);
    srLogWrite("scriptPosition:%i, scriptExecuting:%i", gBigAnimBlock[actor_id].currentScriptPosition, gBigAnimBlock[actor_id].isScriptExecuting);
    srLogWrite("Ptr to anim scripts: %p", tablePtr[gBigAnimBlock[actor_id].animScriptID]);
    srLogWrite("Addres of block head %p, address of positions: %p", &gBigAnimBlock[actor_id].actorID, &gBigAnimBlock[actor_id].restingPosition.xCoordinate);
    srLogWrite("coordinate position: %i, %i, %i", gBigAnimBlock[actor_id].restingPosition.xCoordinate, gBigAnimBlock[actor_id].restingPosition.yCoordinate, gBigAnimBlock[actor_id].restingPosition.zCoordinate);
    srLogWrite("coordinate position: %i, %i, %i", *(byteViewAnimBlock + ((6982/2)*actor_id) + (0x166/2)), *(byteViewAnimBlock + ((6982/2) * actor_id) + (0x168/2)), *(byteViewAnimBlock + ((6982/2) * actor_id) + (0x16A/2)));
    if ((gBigAnimBlock[actor_id].commandID == CMD_LIMIT) && (gBigAnimBlock[actor_id].animScriptID == 0x3D)) {
        srLogWrite("Redirecting animation script execution for actor %i", actor_id);
        srLogWrite("Writing to address %p, base %p", &gBigAnimBlock[actor_id].animScriptID, &gBigAnimBlock[0].animScriptID);
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
