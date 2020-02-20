#ifndef DAMAGE_CALLBACKS_H
#define DAMAGE_CALLBACKS_H
#endif // !1

#include <SisterRay/SisterRay.h>
#include "command_callbacks.h"
#include "statuses.h"
#include <vector>
#include <string>

void calculateDamage(DamageCalculationEvent* srDamageEvent, u8 attackerID, u8 targetID);
void runFormulas(DamageCalculationEvent* srDamageEvent);
void handleCover(DamageCalculationEvent* srDamageEvent);
void setupGameDamageAndImpactEvents(DamageCalculationEvent* srDamageEvent);
void handleDeathImpactSetup(DamageCalculationEvent* srDamageEvent);
void setTargetContext(u8 targetID, DamageCalculationEvent* srDamageEvent);
void attemptStatusInfliction(DamageCalculationEvent* srDamageEvent);
bool didInflictionSucceed(StatusInfliction infliction, DamageCalculationEvent* srDamageEvent);
void handleStatusInfliction(CommandSetupEvent* setupEvent, DamageCalculationEvent* srDamageEvent);
void handleDamage(DamageCalculationEvent* srDamageEvent);
void handleReactions(DamageCalculationEvent* srDamageEvent);
void applyDamageCaps(DamageCalculationEvent* srDamageEvent);
void applyReflect(DamageCalculationEvent* srDamageEvent);
void nullifyAction(DamageCalculationEvent* srDamageEvent);
void applyElementalModifiers(DamageCalculationEvent* setupEvent);
