#include "formulas.h"
#include "../impl.h"
#include "../party/stat_names.h"
#include "damage_callback_utils.h"
#include "formula_names.h"
#include "../party/stat_names.h"
#include "command_sr_callbacks.h"
#include "formula_callbacks.h"

void srNoDamage(DamageCalculationEvent* damageEvent) {
    auto& srDamageCtx = *damageEvent->srDamageContext;
    auto& gameDamageCtx = *damageEvent->damageContext;

    gameDamageCtx.abilityFlags1 |= 2u;
}

#define gameHandleOrientation ((PFNSR_VOIDSUB)0x5DE356)
void srAttackDamageBase(DamageCalculationEvent* damageEvent) {
    srLogWrite("Executing SrBase Physical Formula");
    auto& srDamageCtx = *damageEvent->srDamageContext;
    auto& gameDamageCtx = *damageEvent->damageContext;

    gameHandleOrientation();
    u16 baseDamage = 0;
    u16 actionPower = gameDamageCtx.abilityPower;
    auto& attackerState = srDamageCtx.attackerState;
    auto& targetState = srDamageCtx.targetState;


    auto atkLevel = gameDamageCtx.attackerLevel;
    i32 attackerStr;
    if (srDamageCtx.damageType == MAGICAL) {
        attackerStr = attackerState.battleStats->at(StatNames::MAGIC).activeValue;
    }
    else {
        attackerStr = attackerState.battleStats->at(StatNames::STRENGTH).activeValue;
    }

    srLogWrite("attackerStr: %u | attackerLvl: %f", attackerStr, atkLevel);

    auto lvlFactor = (1 + (atkLevel / 10.0f));
    auto strFactor = (1 + (attackerStr / 32.0f));
    srLogWrite("strFactor: %u | levelFactor: %f", strFactor, lvlFactor);
    baseDamage = actionPower * lvlFactor * strFactor;
    gameDamageCtx.currentDamage = baseDamage;
}

void srSpellDamageBase(DamageCalculationEvent* damageEvent) {
    auto& srDamageCtx = *damageEvent->srDamageContext;
    auto& gameDamageCtx = *damageEvent->damageContext;

    u16 baseDamage = 0;
    u16 actionPower = gameDamageCtx.abilityPower;
    auto& attackerState = srDamageCtx.attackerState;
    auto& targetState = srDamageCtx.targetState;


    auto atkLevel = gameDamageCtx.attackerLevel;
    i32 attackStat;
    if (srDamageCtx.damageType == MAGICAL) {
        attackStat = attackerState.battleStats->at(StatNames::MAGIC).activeValue;
    }
    else {
        attackStat = attackerState.battleStats->at(StatNames::STRENGTH).activeValue;
    }

    u16 lvlFactor = (1 + (atkLevel / 20.0f));
    u16 strFactor = (1 + (attackStat / 32.0f));
    baseDamage = (actionPower * 2) * lvlFactor * strFactor;


    gameDamageCtx.currentDamage = baseDamage;
}

void srAttackHitBase(DamageCalculationEvent* damageEvent) {
    auto hitModifiers = damageEvent->srDamageContext->hitFormulaModifiers;
    const auto& attackerState = damageEvent->srDamageContext->attackerState;
    auto attackerAcc = attackerState.battleStats->at(StatNames::WEAPON_ACCURACY);

    const auto& targetState = damageEvent->srDamageContext->targetState;
    auto targetEvade = targetState.battleStats->at(StatNames::EVADE);

    auto actionHitRate = damageEvent->damageContext->finalHitRate;
    auto delta = (actionHitRate) * (1 + (attackerAcc.activeValue / 100)) - targetEvade.activeValue;
    if (delta < 0)
        delta = 0;

    damageEvent->srDamageContext->hitChance_ = delta;
}

void srSpellHitBase(DamageCalculationEvent* damageEvent) {
    auto hitModifiers = damageEvent->srDamageContext->hitFormulaModifiers;

    const auto& attackerState = damageEvent->srDamageContext->attackerState;
    auto attackerAcc = attackerState.battleStats->at(StatNames::MAGIC_ACCURACY);

    const auto& targetState = damageEvent->srDamageContext->targetState;
    auto targetEvade = targetState.battleStats->at(StatNames::MEVADE);

    auto actionHitRate = damageEvent->damageContext->finalHitRate;
    auto delta = (actionHitRate) * (1 + (attackerAcc.activeValue / 100)) - targetEvade.activeValue;
    if (delta < 0)
        delta = 0;

    damageEvent->srDamageContext->hitChance_ = delta;
}

void initFormulas() {
    gContext.damageFormulas = SrFormulaRegistry();
    auto dmgFormula = DamageFormula();

    dmgFormula.baseFormulaCallback = srAttackDamageBase;
    gContext.damageFormulas.addElement(FormulaNames::PHYSICAL_BASE_DMG, dmgFormula);

    dmgFormula.baseFormulaCallback = srSpellDamageBase;
    gContext.damageFormulas.addElement(FormulaNames::MAGICAL_BASE_DMG, dmgFormula);


    gContext.hitFormulas = SrHitFormulaRegistry();
    auto hitFormula = HitFormula();

    hitFormula.baseFormulaCallback = srAttackHitBase;
    gContext.hitFormulas.addElement(FormulaNames::PHYSICAL_BASE_HIT, hitFormula);

    hitFormula.baseFormulaCallback = srSpellHitBase;
    gContext.hitFormulas.addElement(FormulaNames::MAGICAL_BASE_HIT, hitFormula);


    const auto& modName = std::string("srFF7Base");
    gContext.eventBus.addListener(POST_HIT_FORMULA, (SrEventCallback)applyRangeHitModifier, modName);
    gContext.eventBus.addListener(POST_HIT_FORMULA, (SrEventCallback)applyFuryModifier, modName);


    gContext.eventBus.addListener(POST_DAMAGE_FORMULA, (SrEventCallback)applyRangeModifier, modName);
    gContext.eventBus.addListener(POST_DAMAGE_FORMULA, (SrEventCallback)applyBarrierDamageModifier, modName);
    gContext.eventBus.addListener(POST_DAMAGE_FORMULA, (SrEventCallback)applySplitDamageModifier, modName);
    gContext.eventBus.addListener(POST_DAMAGE_FORMULA, (SrEventCallback)applyQuadModifier, modName);
    gContext.eventBus.addListener(POST_DAMAGE_FORMULA, (SrEventCallback)applyFuryModifier, modName);
}


