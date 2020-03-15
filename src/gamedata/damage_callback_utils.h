#ifndef DAMAGE_CALLBACKS_UTILS_H
#define DAMAGE_CALLBACKS_UTILS_H

#include <SisterRay/SisterRay.h>
#include "command_callbacks.h"
#include "statuses.h"
#include <vector>
#include <string>

bool actorIsDamageImmune(ActorBattleVars battleVars, ActorBattleState srActorState, bool isMagic);
bool srActorHasStatus(const ActorBattleState& srActorState, std::string statusName);

typedef u32(*SRPFN_GAMEGETRANDOMBIT)(u16);
#define getRandomMaskBit ((SRPFN_GAMEGETRANDOMBIT)0x5C80B5)

#endif 
