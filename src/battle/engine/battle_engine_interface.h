#ifndef BATTLE_ENGINE_INTERFACE_H
#define BATTLE_ENGINE_INTERFACE_H

#include <SisterRay/SisterRay.h>
#include "action_interface.h"
#include "action_targeting.h"
#include "action_dispatch.h"

u16 getBattleSpeed();
u16 getDexNormalization();
void setBattleSpeed(u8 speedFactor);
u32 getPreviousInputMask();
SISTERRAY_API void srEnqueueBattleAction(BattleQueueEntry queueEntry);
#endif
