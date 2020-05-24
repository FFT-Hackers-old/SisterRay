#include "battle_stat_formulas.h"
#include "../party/party_member.h"
#include "stat_names.h"

using namespace StatNames;

i32 srHPFormula(SrPartyData* actor) {
    return actor->stats[HP].baseValue + actor->stats[VITALITY].statValue * 12;
}

i32 srMPFormula(SrPartyData* actor) {
    return actor->stats[MP].baseValue + actor->stats[SPIRIT].statValue * 1.5f;
}

i32 srAtkPwrFormula(SrPartyData* actor) {
    return actor->stats[STRENGTH].baseValue;
}

i32 srAtkSpdFormula(SrPartyData* actor) {
    return (1 + (actor->stats[StatNames::STRENGTH].statValue / 255.0f)) * (actor->stats[AGILITY].statValue / 2.0f);
}

i32 srAccFormula(SrPartyData* actor) {
    return (1 + (actor->stats[StatNames::FOCUS].statValue / 255.0f)) * (actor->stats[DEXTERITY].statValue / 2.0);
}


i32 srCritRateFormula(SrPartyData* actor) {
    return (1 + (actor->stats[StatNames::INSIGHT].statValue / 255.0f)) * (actor->stats[DEXTERITY].statValue / 2.0f);
}


i32 srCritPwrFormula(SrPartyData* actor) {
    return (1 + (actor->stats[StatNames::STRENGTH].statValue / 255.0f)) * (actor->stats[INSIGHT].statValue / 2.0f);
}


i32 srStgPwrFormula(SrPartyData* actor) {
    return 40 + (actor->stats[STRENGTH].baseValue / 4.25f);
}

i32 srStgDmgFormula(SrPartyData* actor) {
    i32 base = (1 + (actor->stats[StatNames::INSIGHT].statValue / 255.0f)) * (actor->stats[STRENGTH].statValue / 2.0f);
    return base / 2.5f;
}


// Defensive stat formulas

i32 srArmorFormula(SrPartyData* actor) {
    return (5 * (actor->stats[VITALITY].statValue));
}

i32 srDefenseFormula(SrPartyData* actor) {
    return (1 + (actor->stats[StatNames::DEXTERITY].statValue / 255.0f))* (actor->stats[FOCUS].statValue / 2.0f);
}

i32 srEvadeFormula(SrPartyData* actor) {
    return (1 + (actor->stats[StatNames::FOCUS].statValue / 255.0f))* (actor->stats[AGILITY].statValue / 2.0f);
}

i32 srBlockFormula(SrPartyData* actor) {
    auto defense = (actor->stats[StatNames::FOCUS].statValue / 255.0f) * (actor->stats[DEXTERITY].statValue / 2.0f);
    return  ((1 + (defense / 100.0f)) * 10) + 5;
}

i32 srBlockAmtFormula(SrPartyData* actor) {
    return (9 * (actor->stats[STRENGTH].statValue));
}


i32 srParryFormula(SrPartyData* actor) {
    auto defense = (actor->stats[StatNames::DEXTERITY].statValue / 255.0f) * (actor->stats[FOCUS].statValue / 2.0f);
    return ((1 + (defense / 100.0f)) * 8) + 3;
}
