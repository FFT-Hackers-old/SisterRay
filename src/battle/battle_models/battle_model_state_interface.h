#ifndef BATTLE_MODEL_STATE_INTERFACE_H
#define BATTLE_MODEL_STATE_INTERFACE_H

#include <SisterRay/SisterRay.h>

SISTERRAY_API BattleModelState* getBattleModelState(u8 actorID);
SISTERRAY_API BattleModelStateSmall* getBattleModelState74(u8 actorID);
SISTERRAY_API BattleModel* getActiveModelFromArray(u8 arrayIdx);

SISTERRAY_API ModelRotationData* getBattleModelRotationData(u8 actorID);
SISTERRAY_API void setModelVanish(u8 actorID);
SISTERRAY_API void setModelAppaer(u8 actorID);
#endif // !BATTLE_MODEL_STATE_INTERFACE_H
