#ifndef AI_SCRIPTS_H
#define AI_SCRIPTS_H

#include <SisterRay/SisterRay.h>
#include <vector>

typedef const std::vector<u8> AIScript;

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
    AIScript customEvent[8];
} BattleAIData;

u8* getScriptPtr(const BattleAIData& AIData, u16 scriptType);
void updateScript(AIScript script, AIScript newData, u32 insertIndex);


#endif
