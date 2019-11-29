#include "battle_model_state_interface.h"


SISTERRAY_API BattleModelState* getBattleModelState(u8 actorID) {
    return &(BATTLE_MODEL_STATE_BIG_ARRAY[actorID]);
}


SISTERRAY_API BattleModelStateSmall* getBattleModelState74(u8 actorID) {
    return &(BATTLE_MODEL_STATE74_ARRAY[actorID]);
}
