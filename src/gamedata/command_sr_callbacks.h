#ifndef COMMAND_SR_CALLBACKS_H
#define COMMAND_SR_CALLBACKS_H

#include <SisterRay/SisterRay.h>
#include "command_callbacks.h"
#include "statuses.h"
#include <vector>
#include <string>

struct SrDamageContext_ {
    std::vector<std::string> attackElements;
    std::unordered_map<std::string, SrActorStat> attackStats;
    std::vector<StatusInfliction> statusToInflict; //vector of statuses to inflict
    std::vector<std::string> toAddStatuses;
    std::vector<std::string> toToggleStatuses;
    std::vector<std::string> toRemoveStatuses;
    std::vector<std::string> wereInflictedStatuses;
    ActorBattleState* attackerState;
    ActorBattleState* targetState;
    bool useActionDamageLimits;
    u16 hpDamageLimit;
    u16 mpDamageLimit;
};

#define ELEMENT_COUNT 16
void srSetupAction(CommandSetupEvent setupEvent);
void srLoadAbilityData();
void initializeLimitContext(DamageCalcStruct* damageContext);
void updatePlayerSpellData(DamageCalcStruct* damageContext, EnabledSpell* spellData, const AttackData& abilityData);
void setStatusInflictionData(DamageCalcStruct* damageContext, i32 statusInflictionByte, i32 inflictedStatusMask);
#endif // !COMMAND_CALLBACKS_H
