#include "formula_callbacks.h"
#include "formulas.h"
#include "../impl.h"
#include "damage_callback_utils.h"
#include "status_names.h"

void applyBarrierDamageModifier(DamageCalculationEvent* srDamageEvent) {
    if (isDmgFormulaModifierActive(srDamageEvent, APPLY_BARRIERS)) {
        if (srActorHasStatus(srDamageEvent->srDamageContext->targetState, StatusNames::BARRIER)) {
            if (srDamageEvent->srDamageContext->damageType == PHYSICAL) {
                srDamageEvent->damageContext->currentDamage /= 2;
            }
        }
        else if (srActorHasStatus(srDamageEvent->srDamageContext->targetState, StatusNames::MBARRIER)) {
            if (srDamageEvent->srDamageContext->damageType == MAGICAL) {
                srDamageEvent->damageContext->currentDamage /= 2;
            }
        }
    }
}

void applySplitDamageModifier(DamageCalculationEvent* srDamageEvent) {
    if (!isDmgFormulaModifierActive(srDamageEvent, APPLY_SPLIT))
        return;

    if (srDamageEvent->damageContext->targetCount > 1) {
        if (srDamageEvent->damageContext->abilityTargetingFlags & 0x80)
            srDamageEvent->damageContext->currentDamage = (srDamageEvent->damageContext->currentDamage * 2) / 3;
    }
}

void applyQuadModifier(DamageCalculationEvent* srDamageEvent) {
    if (srDamageEvent->damageContext->quadCount) {
        srDamageEvent->damageContext->currentDamage /= 2;
    }
}

void applySadnessModifier(DamageCalculationEvent* srDamageEvent) {
    if (srActorHasStatus(srDamageEvent->srDamageContext->targetState, StatusNames::SADNESS)) {
        srDamageEvent->damageContext->currentDamage *= 0.8f;
    }
}

void applyRangeModifier(DamageCalculationEvent* srDamageEvent) {
    if (!isDmgFormulaModifierActive(srDamageEvent, APPLY_RANGE)) 
        return;

    i16 attackerRow = srDamageEvent->srDamageContext->attackerRow;
    i16 targetRow = -srDamageEvent->srDamageContext->targetRow;
    i16 row_difference = (attackerRow - targetRow) - 2;
    i16 penalty = .1 * row_difference;

    if (srDamageEvent->damageContext->abilityTargetingFlags & TGT_SHORT_RANGE) {
        srDamageEvent->damageContext->currentDamage *= (1 - penalty);
    }
}

void applyFuryModifier(DamageCalculationEvent* srDamageEvent) {
    if (srActorHasStatus(srDamageEvent->srDamageContext->targetState, StatusNames::FURY)) {
        srDamageEvent->srDamageContext->hitChance_ *= 0.8f;
    }
}

void applyRangeHitModifier(DamageCalculationEvent* srDamageEvent) {
    if (!isHitFormulaModifierActive(srDamageEvent, APPLY_RANGE_HIT))
        return;

    i16 attackerRow = srDamageEvent->srDamageContext->attackerRow;
    i16 targetRow = -srDamageEvent->srDamageContext->targetRow;
    i16 row_difference = (attackerRow - targetRow) - 2;
    i16 penalty = .1 * row_difference;
    if (srDamageEvent->damageContext->abilityTargetingFlags & TGT_SHORT_RANGE) {
        srDamageEvent->srDamageContext->hitChance_ *= (1 - penalty);
    }
}
