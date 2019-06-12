#include "ai_scripts.h"
#include "../impl.h"


const u8* getScriptPtr(const BattleAIData& AIData, u16 scriptType) {
    switch (scriptType) {
        case 0:
            if (!AIData.initScript.empty())
                return AIData.initScript.data();
            break;
        case 1:
            if (!AIData.mainScript.empty())
                return AIData.mainScript.data();
            break;
        case 2:
            if (!AIData.genCounter.empty())
                return AIData.genCounter.data();
            break;
        case 3:
            if (!AIData.deathCounter.empty())
                return AIData.deathCounter.data();
            break;
        case 4:
            if (!AIData.physCounter.empty())
                return AIData.physCounter.data();
            break;
        case 5:
            if (!AIData.magCounter.empty())
                return AIData.magCounter.data();
            break;
        case 6:
            if (!AIData.endBattle.empty())
                return AIData.endBattle.data();
            break;
        case 7:
            if (!AIData.preActionScript.empty())
                return AIData.preActionScript.data();
            break;
    }
    return nullptr;
}

/*These functions take a script block from scene.bin and parse it into an SR internal format*/
void initializeBattleAIData(const u8* const scriptBlock, u32 sceneEnemyIndex, BattleAIData& srAIData) {
    u16* sceneAIEnemyOffsets = (u16*)scriptBlock;
    auto enemyScriptsOffset = sceneAIEnemyOffsets[sceneEnemyIndex];
    if (enemyScriptsOffset != 0xFFFF) {
        auto aiDataPtr = scriptBlock + enemyScriptsOffset;
        initAIScriptStruct(srAIData, aiDataPtr);
    }
}

void initializeFormationAIScript(const u8* const scriptBlock, u32 formationIndex, BattleAIData& srAIData) {
    u16* sceneAIEnemyOffsets = (u16*)scriptBlock;
    auto formationIndexOffset = sceneAIEnemyOffsets[formationIndex];
    auto aiDataPtr = scriptBlock + formationIndexOffset;
    initAIScriptStruct(srAIData, aiDataPtr);
}

void initAIScriptStruct(BattleAIData& srAIData, const u8* const scriptBlockStart) {
    const u16* const wordReader = (const u16* const)scriptBlockStart;
    SceneAIOffsets scriptStarts = { wordReader[0], wordReader[1], wordReader[2], wordReader[3], wordReader[4], wordReader[5], wordReader[6], wordReader[7] };
    memcpy(&(scriptStarts.customEventScripts[0]), (const void*)(scriptBlockStart + 16), 16);

    /*This is a bit ugly, consider refactoring the AI data stucture later so a loop will work*/
    const u8* scriptStart;
    if (scriptStarts.initScript != 0xFFFF) {
        scriptStart = scriptBlockStart + scriptStarts.initScript;
        copyAIScript(srAIData.initScript, scriptStart);
    }

    if (scriptStarts.mainScript != 0xFFFF) {
        scriptStart = scriptBlockStart + scriptStarts.mainScript;
        copyAIScript(srAIData.mainScript, scriptStart);
    }

    if (scriptStarts.genCounter != 0xFFFF) {
        scriptStart = scriptBlockStart + scriptStarts.genCounter;
        copyAIScript(srAIData.genCounter, scriptStart);
    }

    if (scriptStarts.physCounter != 0xFFFF) {
        scriptStart = scriptBlockStart + scriptStarts.physCounter;
        copyAIScript(srAIData.physCounter, scriptStart);
    }

    if (scriptStarts.magCounter != 0xFFFF) {
        scriptStart = scriptBlockStart + scriptStarts.magCounter;
        copyAIScript(srAIData.magCounter, scriptStart);
    }

    if (scriptStarts.preActionScript != 0xFFFF) {
        scriptStart = scriptBlockStart + scriptStarts.preActionScript;
        copyAIScript(srAIData.preActionScript, scriptStart);
    }

    if (scriptStarts.deathCounter != 0xFFFF) {
        scriptStart = scriptBlockStart + scriptStarts.deathCounter;
        copyAIScript(srAIData.deathCounter, scriptStart);
    }

    if (scriptStarts.endBattle != 0xFFFF) {
        scriptStart = scriptBlockStart + scriptStarts.endBattle;
        copyAIScript(srAIData.endBattle, scriptStart);
    }

    for (auto customEventIndex = 0; customEventIndex < 8; customEventIndex++) {
        if (scriptStarts.customEventScripts[customEventIndex] != 0xFFFF) {
            scriptStart = scriptBlockStart + scriptStarts.customEventScripts[customEventIndex];
            copyAIScript(srAIData.customEventScripts[customEventIndex], scriptStart);
        }
    }

}

void copyAIScript(AIScript& aiScript, const u8* const scriptStart) {
    auto scriptPosition = scriptStart;
    while (*scriptPosition != 0x73) {
        aiScript.push_back(*scriptPosition);
        scriptPosition++;
    }
    aiScript.push_back(0x73);
}
