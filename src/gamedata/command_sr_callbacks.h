#ifndef COMMAND_SR_CALLBACKS_H
#define COMMAND_SR_CALLBACKS_H

#include <SisterRay/SisterRay.h>
#include "command_callbacks.h"

#define ELEMENT_COUNT 16
void srSetupAction(CommandSetupEvent setupEvent);
void srLoadAbilityData();
void initializeLimitContext(DamageCalcStruct* damageContext);
void updatePlayerSpellData(DamageCalcStruct* damageContext, EnabledSpell* spellData, const AttackData& abilityData);
void setStatusInflictionData(DamageCalcStruct* damageContext, i32 statusInflictionByte, i32 inflictedStatusMask);
#endif // !COMMAND_CALLBACKS_H
