#include "ai_scripts.h"


u8* getScriptPtr(const BattleAIData& AIData, u16 scriptType) {
    switch (scriptType) {
        case 0:
            return AIData.initScript.data();
        case 1:
            return AIData.mainScript.data();
        case 2:
            return AIData.genCounter.data();
        case 3:
            return AIData.deathCounter.data();
        case 4:
            return AIData.physCounter.data();
        case 5:
            return AIData.magCounter.data();
        case 6:
            return AIData.endBattle.data();
        case 7:
            return AIData.preActionScript.data;
    }
}
