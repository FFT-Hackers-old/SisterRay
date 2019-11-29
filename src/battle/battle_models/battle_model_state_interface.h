#ifndef BATTLE_MODEL_STATE_INTERFACE_H
#define BATTLE_MODEL_STATE_INTERFACE_H

#include <SisterRay/SisterRay.h>

SISTERRAY_API BattleModelState* getBattleModelState(u8 actorID);

SISTERRAY_API BattleModelStateSmall* getBattleModelState74(u8 actorID);


SISTERRAY_API ModelRotationData* getBattleModelRotationData(u8 actorID);
#endif // !BATTLE_MODEL_STATE_INTERFACE_H
