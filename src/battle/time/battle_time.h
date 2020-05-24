#ifndef BATTLE_TIME_H
#define BATTLE_TIME_H

#include <SisterRay/SisterRay.h>

void srDispathTimeEvents();
void srHandleFillATB();
void srHandleFillLimit();
void srUpdateActorTimers(u8 actorIdx);
void srSetActorTimers(ActorBattleState& battleActor);
void listenForEscape();
void handleActorTurn(u8 actorIdx, ActorBattleState& actorState);
#endif // !BATTLE_TIME_H
