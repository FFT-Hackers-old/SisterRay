#include "sr_battle_engine.h"
#include "../impl.h"


void initializeSrBattleEngine() {
    mogReplaceFunction(FF7_TRANSLATE_ENEMY_BDATA, &translateEnemyABData);
    mogReplaceFunction(FF7_TRANSLATE_PLAYER_BDATA, &translatePlayerABData);
    mogReplaceFunction(FF7_CREATE_MODEL, &srCreateModel);
}
