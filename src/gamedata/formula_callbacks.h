#ifndef DAMAGE_FORMULAS_CALLBACKS_H
#define DAMAGE_FORMULAS_CALLBACKS_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include <unordered_set>
#include "formulas.h"


void applyBarrierDamageModifier(DamageCalculationEvent* srDamageEvent);
void applySplitDamageModifier(DamageCalculationEvent* srDamageEvent);
void applyQuadModifier(DamageCalculationEvent* srDamageEvent);
void applySadnessModifier(DamageCalculationEvent* srDamageEvent);
void srApplyRangeModifier(DamageCalculationEvent* srDamageEvent);

#endif // !DAMAGE_FORMULAS_H
