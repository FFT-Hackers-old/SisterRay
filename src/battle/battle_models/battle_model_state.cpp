#include "battle_model_state_interface.h"


SISTERRAY_API BattleModelState* getBattleModelState(u8 actorID) {
    return &(BATTLE_MODEL_STATE_BIG_ARRAY[actorID]);
}


SISTERRAY_API BattleModelStateSmall* getBattleModelState74(u8 actorID) {
    return &(BATTLE_MODEL_STATE74_ARRAY[actorID]);
}


SISTERRAY_API ModelRotationData* getBattleModelRotationData(u8 actorID) {
    return &(BATTLE_MODEL_ROTATION_DATA[actorID]);
}

SISTERRAY_API void setModelVanish(u8 actorID) {
    getBattleModelState(actorID)->modelEffectFlags |= 2;
}

SISTERRAY_API void setModelAppaer(u8 actorID) {
    getBattleModelState(actorID)->modelEffectFlags |= 4;
}
