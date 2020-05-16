#ifndef BATTLE_ENGINE_INTERFACE_H
#define BATTLE_ENGINE_INTERFACE_H

#include <SisterRay/SisterRay.h>
#include "actions/action_interface.h"

u16 getBattleSpeed();
u16 getDexNormalization();
void setBattleSpeed(u8 speedFactor);
u32 getPreviousInputMask();
SISTERRAY_API void srEnqueueBattleAction(BattleQueueEntry queueEntry);
#endif
