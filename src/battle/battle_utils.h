#ifndef BATTLE_UTILS_H
#define BATTLE_UTILS_H

#include <SisterRay/SisterRay.h>

u8 countTargets(u16 targetMask);
u8 getBattleType();
u8 getTargetAllActive();
u16 getAnimatingActionTargetMask();
u8 getAnimatingActionTargetIdx();
u8 getActionActorIdx();
u8 getActorIdleAnimScript(u8 actorID);

#endif
