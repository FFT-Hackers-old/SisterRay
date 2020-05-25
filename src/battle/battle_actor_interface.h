#ifndef BATTLE_ACTOR_INTERFACE_H
#define BATTLE_ACTOR_INTERFACE_H

#include <SisterRay/SisterRay.h>

SISTERRAY_API SrBattleStat* getBattleActorStat(ActorBattleState* battleState, const char* statName);
#endif // !BATTLE_ACTOR_INTEFACE_H
