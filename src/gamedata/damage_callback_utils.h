#ifndef DAMAGE_CALLBACKS_UTILS_H
#define DAMAGE_CALLBACKS_UTILS_H

#include <SisterRay/SisterRay.h>
#include "command_callbacks.h"
#include "statuses.h"
#include <vector>
#include <string>
#include "../party/battle_stats.h"
#include "../battle/battle_actors.h"
#include "damage_formulas.h"

struct SrDamageContext_ {
    std::vector<std::string> attackElements;
    std::unordered_map<std::string, SrActorStat> attackStats;
    std::vector<StatusInfliction> statusToInflict; //vector of statuses to inflict
    std::vector<std::string> toAddStatuses;
    std::vector<std::string> toToggleStatuses;
    std::vector<std::string> toRemoveStatuses;
    std::vector<std::string> wereInflictedStatuses;
    ActorBattleState attackerState;
    ActorBattleState targetState;
    bool useActionDamageLimits;
    u16 hpDamageLimit;
    u16 mpDamageLimit;
    DamageType damageType;
    DamageFormula damageFormula;
    HitFormula hitFormula;
};

bool actorIsDamageImmune(ActorBattleVars battleVars, ActorBattleState srActorState, bool isMagic);
bool srActorHasStatus(const ActorBattleState& srActorState, std::string statusName);

typedef u32(*SRPFN_GAMEGETRANDOMBIT)(u16);
#define getRandomMaskBit ((SRPFN_GAMEGETRANDOMBIT)0x5C80B5)

#endif 
