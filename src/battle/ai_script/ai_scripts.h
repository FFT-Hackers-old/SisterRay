#ifndef AI_SCRIPTS_H
#define AI_SCRIPTS_H

#include <SisterRay/SisterRay.h>
#include <vector>

typedef std::vector<u8> AIScript;

typedef struct {
    u16 initScript;
    u16 mainScript;
    u16 genCounter;
    u16 deathCounter;
    u16 physCounter;
    u16 magCounter;
    u16 endBattle;
    u16 preActionScript;
    u16 customEventScripts[8];
} SceneAIOffsets;

/*Struct which contains u8 ptrs to indexes in the underlying buffer*/
typedef struct {
    AIScript initScript;
    AIScript mainScript;
    AIScript genCounter;
    AIScript deathCounter;
    AIScript physCounter;
    AIScript magCounter;
    AIScript endBattle;
    AIScript preActionScript;
    AIScript customEventScripts[8];
} BattleAIData;

const u8* getScriptPtr(const BattleAIData& AIData, u16 scriptType);
void initializeBattleAIData(const u8* const scriptBlock, u32 sceneEnemyIndex, BattleAIData& srAIData);
void initializeFormationAIScript(const u8* const scriptBlock, u32 formationIndex, BattleAIData& srAIData);
void copyAIScript(AIScript& aiScript, const u8* const scriptStart);
void initAIScriptStruct(BattleAIData& srAIData, const u8* const scriptBlockStart);

#endif
