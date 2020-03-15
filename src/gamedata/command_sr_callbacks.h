#ifndef COMMAND_SR_CALLBACKS_H
#define COMMAND_SR_CALLBACKS_H

#include <SisterRay/SisterRay.h>
#include "command_callbacks.h"
#include "statuses.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "../party/battle_stats.h"

#define ELEMENT_COUNT 16
void srSetupAction(CommandSetupEvent& setupEvent);
void srLoadAbilityData();
void initializeLimitContext(DamageCalcStruct* damageContext);
void updatePlayerSpellData(DamageCalcStruct* damageContext, EnabledSpell* spellData, const AttackData& abilityData);
void setStatusInflictionData(DamageCalcStruct* damageContext, i32 statusInflictionByte, i32 inflictedStatusMask);
void handleQuadraMagic(CommandSetupEvent& setupEvent);
void handleDrainEffects(CommandSetupEvent& setupEvent);
void handleTargeting(CommandSetupEvent& setupEvent);
bool didActionFail(CommandSetupEvent& setupEvent);
void handleFailedAction(CommandSetupEvent& setupEvent, u16 strIdx);
void handleActorDrain(CommandSetupEvent& setupEvent, u8 actorIdx, bool drainMP);
void setupAttackerAnimEvent(CommandSetupEvent& setupEvent, u16 targetMask);
void setSupportMateriaFlags(CommandSetupEvent& srSetupEvent);
void srPushSpecialAnimEvent(u16 animationScriptID, CommandSetupEvent& setupEvent);

#endif // !COMMAND_CALLBACKS_H
