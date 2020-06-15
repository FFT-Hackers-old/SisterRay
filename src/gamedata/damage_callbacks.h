#ifndef DAMAGE_CALLBACKS_H
#define DAMAGE_CALLBACKS_H

#include <SisterRay/SisterRay.h>
#include "command_callbacks.h"
#include "statuses.h"
#include <vector>
#include <string>

void calculateDamage(DamageCalculationEvent* srDamageEvent, u8 attackerID, u8 targetID);
void runFormulas(DamageCalculationEvent* srDamageEvent);
void handleCover(DamageCalculationEvent* srDamageEvent);
void handleSteal(DamageCalculationEvent* srDamageEvent);
void handleDamageSuccessful(DamageCalculationEvent* srDamageEvent);
void setImpactEvents(DamageCalculationEvent* srDamageEvent);
void handleDeathImpactSetup(DamageCalculationEvent* srDamageEvent);
void setTargetContext(u8 targetID, DamageCalculationEvent* srDamageEvent);
void attemptStatusInfliction(DamageCalculationEvent* srDamageEvent);
bool didActionInflictionSucceed(StatusInfliction infliction, DamageCalculationEvent* srDamageEvent);
void handleStatusInfliction(DamageCalculationEvent* srDamageEvent);
void handleMorph(DamageCalculationEvent* srDamageEvent);
void handleHealEffects(DamageCalculationEvent* srDamageEvent);
void handleLimitFill(DamageCalculationEvent* srDamageEvent);
void calculateHealAmounts(DamageCalculationEvent* srDamageEvent);
void handleDamage(DamageCalculationEvent* srDamageEvent);
void handleDamageReaction(DamageCalculationEvent* srDamageEvent);
void applyDamageCaps(DamageCalculationEvent* srDamageEvent);
void applyReflect(DamageCalculationEvent* srDamageEvent);
void nullifyAction(DamageCalculationEvent* srDamageEvent);
void applyElementalModifiers(DamageCalculationEvent* setupEvent);

#endif 
