#ifndef DAMAGE_CALLBACKS_UTILS_H
#define DAMAGE_CALLBACKS_UTILS_H

#include <SisterRay/SisterRay.h>
#include "command_callbacks.h"
#include "statuses.h"
#include <vector>
#include <string>

bool actorIsDamageImmune(ActorBattleVars battleVars, ActorBattleState srActorState, bool isMagic);
bool actorPosessesStatus(const ActorBattleVars& battleVars, const ActorBattleState& srActorState, std::string statusName);

#endif 
