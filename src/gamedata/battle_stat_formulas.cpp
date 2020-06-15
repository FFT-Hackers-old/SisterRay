#include "battle_stat_formulas.h"
#include "battle_stats.h"
#include "../party/party_member.h"
#include "stat_names.h"

using namespace StatNames;

i32 srHPFormula(StatFormulaCtx* ctx) {
    auto& stats = *ctx->stats;
    return stats[HP].baseValue + stats[VITALITY].statValue * 12;
}

i32 srMPFormula(StatFormulaCtx* ctx) {
    auto& stats = *ctx->stats;
    return stats[MP].baseValue + stats[SPIRIT].statValue * 1.5f;
}

i32 srAtkPwrFormula(StatFormulaCtx* ctx) {
    auto& stats = *ctx->stats;
    return stats[STRENGTH].baseValue;
}

i32 srAtkSpdFormula(StatFormulaCtx* ctx) {
    auto& stats = *ctx->stats;
    return (1 + (stats[StatNames::STRENGTH].statValue / 255.0f)) * (stats[AGILITY].statValue / 2.0f);
}

i32 srAccFormula(StatFormulaCtx* ctx) {
    auto& stats = *ctx->stats;
    return (1 + (stats[StatNames::FOCUS].statValue / 255.0f)) * (stats[DEXTERITY].statValue / 2.0);
}


i32 srCritRateFormula(StatFormulaCtx* ctx) {
    auto& stats = *ctx->stats;
    return (1 + (stats[StatNames::INSIGHT].statValue / 255.0f)) * (stats[DEXTERITY].statValue / 2.0f);
}


i32 srCritPwrFormula(StatFormulaCtx* ctx) {
    auto& stats = *ctx->stats;
    return (1 + (stats[StatNames::STRENGTH].statValue / 255.0f)) * (stats[INSIGHT].statValue / 2.0f);
}


i32 srStgPwrFormula(StatFormulaCtx* ctx) {
    auto& stats = *ctx->stats;
    return 40 + (stats[STRENGTH].baseValue / 4.25f);
}

i32 srStgDmgFormula(StatFormulaCtx* ctx) {
    auto& stats = *ctx->stats;
    i32 base = (1 + (stats[StatNames::INSIGHT].statValue / 255.0f)) * (stats[STRENGTH].statValue / 2.0f);
    return base / 2.5f;
}


// Defensive stat formulas

i32 srArmorFormula(StatFormulaCtx* ctx) {
    auto& stats = *ctx->stats;
    return (5 * (stats[VITALITY].statValue));
}

i32 srDefenseFormula(StatFormulaCtx* ctx) {
    auto& stats = *ctx->stats;
    return (1 + (stats[StatNames::DEXTERITY].statValue / 255.0f))* (stats[FOCUS].statValue / 2.0f);
}

i32 srEvadeFormula(StatFormulaCtx* ctx) {
    auto& stats = *ctx->stats;
    return (1 + (stats[StatNames::FOCUS].statValue / 255.0f)) * (stats[AGILITY].statValue / 2.0f);
}

i32 srBlockFormula(StatFormulaCtx* ctx) {
    auto& stats = *ctx->stats;
    auto defense = (stats[StatNames::FOCUS].statValue / 255.0f) * (stats[DEXTERITY].statValue / 2.0f);
    return  ((1 + (defense / 100.0f)) * 10) + 5;
}

i32 srBlockAmtFormula(StatFormulaCtx* ctx) {
    auto& stats = *ctx->stats;
    return (9 * (stats[STRENGTH].statValue));
}


i32 srParryFormula(StatFormulaCtx* ctx) {
    auto& stats = *ctx->stats;
    auto defense = (stats[StatNames::DEXTERITY].statValue / 255.0f) * (stats[FOCUS].statValue / 2.0f);
    return ((1 + (defense / 100.0f)) * 8) + 3;
}
