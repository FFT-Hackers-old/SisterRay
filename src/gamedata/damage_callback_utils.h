#ifndef DAMAGE_CALLBACKS_UTILS_H
#define DAMAGE_CALLBACKS_UTILS_H

#include <SisterRay/SisterRay.h>
#include "command_callbacks.h"
#include "statuses.h"
#include <vector>
#include <string>
#include "../party/battle_stats.h"
#include "../battle/battle_actors.h"
#include "formulas.h"

struct SrDamageContext_ {
    std::vector<std::string> attackElements;
    std::unordered_map<std::string, SrActorStat> attackStats;
    std::vector<StatusInfliction> statusToInflict; //vector of statuses to inflict
    std::vector<std::string> toAddStatuses;
    std::vector<std::string> toToggleStatuses;
    std::vector<std::string> toRemoveStatuses;
    std::vector<std::string> wereInflictedStatuses;
    ActorBattleState attackerState;
    u8 attackerRow;
    ActorBattleState targetState;
    u8 targetRow;
    bool useActionDamageLimits;
    u16 hpDamageLimit;
    u16 mpDamageLimit;
    u8 hitChance_;
    DamageType damageType;
    u16 damageFormula;
    std::unordered_set<DamageModifiers> dmgFormulaModifiers;
    u16 hitFormula;
    std::unordered_set<HitModifiers> hitFormulaModifiers;
};

bool actorIsDamageImmune(ActorBattleVars battleVars, ActorBattleState srActorState, bool isMagic);
bool srActorHasStatus(const ActorBattleState& srActorState, std::string statusName);
bool srActorHasStatus(const std::vector<ActiveStatus>& activeStatuses, std::string statusName);
void setActionDidHit(DamageCalculationEvent* event, bool didHit);
bool checkActionDidHit(DamageCalculationEvent* dmgEvent);
void setDisplayBarrier(DamageCalculationEvent* event);
void setDisplayMBarrier(DamageCalculationEvent* event);
bool checkDisplayBarrier(const DamageCalculationEvent* srDamageEvent);
bool checkDisplayMBarrier(const DamageCalculationEvent* srDamageEvent);
bool isHitFormulaModifierActive(DamageCalculationEvent* dmgEvent, HitModifiers modifier);
bool isDmgFormulaModifierActive(DamageCalculationEvent* dmgEvent, DamageModifiers modifier);
bool actionIsReflectable(const DamageCalculationEvent* srDamageEvent);
bool actionOnlyHitsDead(const DamageCalculationEvent* srDamageEvent);
void setStatusWasChanged(DamageCalculationEvent* srDamageEvent, bool wasChanged);
bool wasStatusChanged(const DamageCalculationEvent* srDamageEvent);

typedef u32(*SRPFN_GAMEGETRANDOMBIT)(u16);
#define getRandomMaskBit ((SRPFN_GAMEGETRANDOMBIT)0x5C80B5)

#endif 
