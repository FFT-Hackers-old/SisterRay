#include "ai_scripts.h"


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
    auto aiDataPtr = scriptBlock + enemyScriptsOffset;
    initAIScriptStruct(srAIData, aiDataPtr);
}

void initializeFormationAIScript(const u8* const scriptBlock, u32 formationIndex, BattleAIData& srAIData) {
    u16* sceneAIEnemyOffsets = (u16*)scriptBlock;
    auto formationIndexOffset = sceneAIEnemyOffsets[formationIndex];
    auto aiDataPtr = scriptBlock + formationIndexOffset;
    initAIScriptStruct(srAIData, aiDataPtr);
}

void initAIScriptStruct(BattleAIData& srAIData, const u8* const scriptBlockStart) {
    const u16* const wordReader = (const u16* const)scriptBlockStart;
    SceneAIOffsets scriptStarts = { wordReader[0], wordReader[1], wordReader[2], wordReader[3], wordReader[4], wordReader[5], wordReader[6], wordReader[7], wordReader[8] };
    auto scriptStart = scriptBlockStart + scriptStarts.initScript;

    /*This is a bit ugly, consider refactoring the AI data stucture later so a loop will work*/
    if (*scriptStart != 0xFFFFFFFF) {
        copyAIScript(srAIData.initScript, scriptStart);
    }

    scriptStart = scriptBlockStart + scriptStarts.mainScript;
    if (*scriptStart != 0xFFFFFFFF) {
        copyAIScript(srAIData.mainScript, scriptStart);
    }

    scriptStart = scriptBlockStart + scriptStarts.genCounter;
    if (*scriptStart != 0xFFFFFFFF) {
        copyAIScript(srAIData.genCounter, scriptStart);
    }

    scriptStart = scriptBlockStart + scriptStarts.physCounter;
    if (*scriptStart != 0xFFFFFFFF) {
        copyAIScript(srAIData.physCounter, scriptStart);
    }

    scriptStart = scriptBlockStart + scriptStarts.magCounter;
    if (*scriptStart != 0xFFFFFFFF) {
        copyAIScript(srAIData.magCounter, scriptStart);
    }

    scriptStart = scriptBlockStart + scriptStarts.preActionScript;
    if (*scriptStart != 0xFFFFFFFF) {
        copyAIScript(srAIData.preActionScript, scriptStart);
    }

    scriptStart = scriptBlockStart + scriptStarts.deathCounter;
    if (*scriptStart != 0xFFFFFFFF) {
        copyAIScript(srAIData.deathCounter, scriptStart);
    }

    scriptStart = scriptBlockStart + scriptStarts.endBattle;
    if (*scriptStart != 0xFFFFFFFF) {
        copyAIScript(srAIData.endBattle, scriptStart);
    }

    for (auto customEventIndex = 0; customEventIndex < 8; customEventIndex++) {
        scriptStart = scriptBlockStart + scriptStarts.customEventScripts[customEventIndex];
        if (*scriptStart != 0xFFFFFFFF) {
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
}
