#include "battle_stats.h"
#include "stat_names.h"
#include "../impl.h"
#include "status_names.h"
#include "element_names.h"
#include "battle_stat_formulas.h"


SrStatRegistry::SrStatRegistry(bool initResistances) : SrNamedResourceRegistry<SrStat, std::string>() {
    SrStat hp = { EncodedString::from_unicode(StatNames::HP_NAME.c_str()), 9999, 0, false, false, true, true, &srHPFormula };
    addElement(StatNames::HP, hp);
    SrStat mp = { EncodedString::from_unicode(StatNames::MP_NAME.c_str()), 999, 0, false, false, true, true, &srMPFormula };
    addElement(StatNames::MP, mp);

    SrStat str = { EncodedString::from_unicode(StatNames::STR_NAME.c_str()), 255, 0, false, true, true };
    addElement(StatNames::STRENGTH, str);
    SrStat vit = { EncodedString::from_unicode(StatNames::VIT_NAME.c_str()), 255, 0, false, true, true };
    addElement(StatNames::VITALITY, vit);
    SrStat dex = { EncodedString::from_unicode(StatNames::DEX_NAME.c_str()), 255, 0, false, true, true };
    addElement(StatNames::DEXTERITY, dex);
    SrStat agi = { EncodedString::from_unicode(StatNames::AGILITY_NAME.c_str()), 255, 0, false, true, true };
    addElement(StatNames::AGILITY, agi);
    SrStat mag = { EncodedString::from_unicode(StatNames::MAG_NAME.c_str()), 255, 0, false, true, true };
    addElement(StatNames::MAGIC, mag);
    SrStat spr = { EncodedString::from_unicode(StatNames::SPR_NAME.c_str()), 255, 0, false, true, true };
    addElement(StatNames::SPIRIT, spr);
    SrStat foc = { EncodedString::from_unicode(StatNames::FOCUS_NAME.c_str()), 255, 0, false, true, true };
    addElement(StatNames::FOCUS, foc);
    SrStat insight = { EncodedString::from_unicode(StatNames::INSIGHT_NAME.c_str()), 255, 0, false, true, true };
    addElement(StatNames::INSIGHT, insight);

    SrStat luck = { EncodedString::from_unicode(StatNames::LUCK_NAME.c_str()), 255, 0, false, true, true };
    addElement(StatNames::LUCK, luck);


    SrStat wpnAtk = { EncodedString::from_unicode(StatNames::WEAPON_ATTACK_NAME.c_str()), 255, 0, false };
    addElement(StatNames::WEAPON_ATTACK, wpnAtk);
    SrStat wpnHit = { EncodedString::from_unicode(StatNames::WEAPON_ACCURACY_NAME.c_str()), 255, 0, false };
    addElement(StatNames::WEAPON_ACCURACY, wpnHit);
    SrStat wpnMag = { EncodedString::from_unicode(StatNames::WEAPON_MAGIC_NAME.c_str()), 255, 0, false };
    addElement(StatNames::WEAPON_MAGIC, wpnMag);
    SrStat adef = { EncodedString::from_unicode(StatNames::ARMOR_DEFENSE_NAME.c_str()), 255, 0, false };
    addElement(StatNames::ARMOR_DEFENSE, adef);
    SrStat mdef = { EncodedString::from_unicode(StatNames::ARMOR_MDEFENSE_NAME.c_str()), 255, 0, false };
    addElement(StatNames::ARMOR_MDEFENSE, mdef);
    SrStat mev = { EncodedString::from_unicode(StatNames::MEVADE_NAME.c_str()), 255, 0, false };
    addElement(StatNames::MEVADE, mev);
    SrStat magHit = { EncodedString::from_unicode(StatNames::MAGIC_ACCURACY_NAME.c_str()), 255, 0, false };
    addElement(StatNames::MAGIC_ACCURACY, magHit);

    SrStat atkpwr = { EncodedString::from_unicode(StatNames::ATTACK_POWER_NAME.c_str()), 255, 0, false, false, false, true, &srAtkPwrFormula };
    addElement(StatNames::ATTACK_POWER, atkpwr);
    SrStat atkSpd = { EncodedString::from_unicode(StatNames::ATTACK_SPEED_NAME.c_str()), 255, 0, false, false, false, true, &srAtkSpdFormula };
    addElement(StatNames::ATTACK_SPEED, atkSpd);
    SrStat acc = { EncodedString::from_unicode(StatNames::ACCURACY_NAME.c_str()), 255, 0, false, false, false, true, &srAccFormula };
    addElement(StatNames::ACCURACY, acc);
    SrStat critRate = { EncodedString::from_unicode(StatNames::CRIT_RATE_NAME.c_str()), 255, 0, false, false, false, true, &srCritRateFormula };
    addElement(StatNames::CRIT_RATE, critRate);
    SrStat critPwr = { EncodedString::from_unicode(StatNames::CRIT_POWER_NAME.c_str()), 255, 0, false, false, false, true, &srCritPwrFormula };
    addElement(StatNames::CRIT_POWER, critPwr);
    SrStat stagPwr = { EncodedString::from_unicode(StatNames::STAGGER_POWER_NAME.c_str()), 255, 0, false, false, false, true, &srStgPwrFormula };
    addElement(StatNames::STAGGER_POWER, stagPwr);
    SrStat stagDmg = { EncodedString::from_unicode(StatNames::STAGGER_DAMAGE_NAME.c_str()), 100, 0, false, false, false, true, &srStgDmgFormula };
    addElement(StatNames::STAGGER_DAMAGE, stagDmg);
    SrStat armPen = { EncodedString::from_unicode(StatNames::ARMOR_PEN_NAME.c_str()), 100, 0, false };
    addElement(StatNames::ARMOR_PEN, armPen);
    SrStat defPen = { EncodedString::from_unicode(StatNames::DEFENSE_PEN_NAME.c_str()), 100, 0, false };
    addElement(StatNames::DEFENSE_PEN, defPen);
    SrStat evaPen = { EncodedString::from_unicode(StatNames::EVASION_PEN_NAME.c_str()), 100, 0, false };
    addElement(StatNames::EVASION_PEN, evaPen);
    SrStat parryPen = { EncodedString::from_unicode(StatNames::PARRY_PEN_NAME.c_str()), 100, 0, false };
    addElement(StatNames::PARRY_PEN, parryPen);
    SrStat blockPen = { EncodedString::from_unicode(StatNames::BLOCK_PEN_NAME.c_str()), 100, 0, false };
    addElement(StatNames::BLOCK_PEN, blockPen);

    SrStat def = { EncodedString::from_unicode(StatNames::DEFENSE_NAME.c_str()), 255, 0, false, false, false, true, &srDefenseFormula };
    addElement(StatNames::DEFENSE, def);
    SrStat armor = { EncodedString::from_unicode(StatNames::ARMOR_NAME.c_str()), 5000, 0, false, false, false, true, &srArmorFormula };
    addElement(StatNames::ARMOR, armor);
    SrStat pev = { EncodedString::from_unicode(StatNames::EVADE_NAME.c_str()), 255, 0, false, false, false, true, &srEvadeFormula };
    addElement(StatNames::EVADE, pev);
    SrStat block = { EncodedString::from_unicode(StatNames::BLOCK_NAME.c_str()), 5000, 0, false, false, false, true, &srBlockAmtFormula };
    addElement(StatNames::BLOCK, block);
    SrStat blockrate = { EncodedString::from_unicode(StatNames::BLOCK_RATE_NAME.c_str()), 255, 0, false, false, false, true, &srBlockFormula };
    addElement(StatNames::BLOCK_RATE, blockrate);
    SrStat parry = { EncodedString::from_unicode(StatNames::PARRY_RATE_NAME.c_str()), 255, 0, false, false, false, true, &srParryFormula };
    addElement(StatNames::PARRY_RATE, parry);
    SrStat stagRes = { EncodedString::from_unicode(StatNames::STAGGER_RES_NAME.c_str()), 100, -100, true };
    addElement(StatNames::STAGGER_RES, stagRes);
    SrStat critRes = { EncodedString::from_unicode(StatNames::CRIT_RES_NAME.c_str()), 100, -100, true };
    addElement(StatNames::CRIT_RES, critRes);
    SrStat woundRes = { EncodedString::from_unicode(StatNames::WOUND_RES_NAME.c_str()), 100, -100, true };
    addElement(StatNames::WOUND_RES, woundRes);
    SrStat physDbfRes = { EncodedString::from_unicode(StatNames::PHYS_DEBUFF_RES_NAME.c_str()), 100, -100, true };
    addElement(StatNames::PHYS_DEBUFF_RES, physDbfRes);
    SrStat physBufAff = { EncodedString::from_unicode(StatNames::PHYS_BUFF_AFF_NAME.c_str()), 100, -100, true };
    addElement(StatNames::PHYS_BUFF_AFF, physBufAff);



    // Register Stats Associated with Particular Elements And Statuses
    SrStat pen = { EncodedString::from_unicode(ElementNames::FIRE_NAME.c_str()), 100, 0, false };
    SrStat fres = { EncodedString::from_unicode(ElementNames::FIRE_NAME.c_str()), 200, -100, true };
    addElement(StatNames::FIRE_RES, fres);
    addElement(StatNames::FIRE_AFF, fres);
    pen.displayName = EncodedString::from_unicode(ElementNames::FIRE_NAME.c_str());
    addElement(StatNames::FIRE_PEN, pen);
    SrStat ice = { EncodedString::from_unicode(ElementNames::ICE_NAME.c_str()), 200, -100, true };
    addElement(StatNames::ICE_RES, ice);
    addElement(StatNames::ICE_AFF, ice);
    pen.displayName = EncodedString::from_unicode(ElementNames::ICE_NAME.c_str());
    addElement(StatNames::ICE_PEN, pen);
    SrStat light = { EncodedString::from_unicode(ElementNames::LIGHT_NAME.c_str()), 200, -100, true };
    addElement(StatNames::LIGHT_RES, light);
    addElement(StatNames::LIGHT_AFF, light);
    pen.displayName = EncodedString::from_unicode(ElementNames::LIGHT_NAME.c_str());
    addElement(StatNames::LIGHT_PEN, pen);
    SrStat earth = { EncodedString::from_unicode(ElementNames::EARTH_NAME.c_str()), 200, -100, true };
    addElement(StatNames::EARTH_RES, earth);
    addElement(StatNames::EARTH_AFF, earth);
    pen.displayName = EncodedString::from_unicode(ElementNames::EARTH_NAME.c_str());
    addElement(StatNames::EARTH_PEN, pen);
    SrStat wind = { EncodedString::from_unicode(ElementNames::WIND_NAME.c_str()), 200, -100, true };
    addElement(StatNames::WIND_RES, wind);
    addElement(StatNames::WIND_AFF, wind);
    pen.displayName = EncodedString::from_unicode(ElementNames::WIND_NAME.c_str());
    addElement(StatNames::WIND_PEN, pen);
    SrStat water = { EncodedString::from_unicode(ElementNames::WATER_NAME.c_str()), 200, -100, true };
    addElement(StatNames::WATER_RES, water);
    addElement(StatNames::WATER_AFF, water);
    pen.displayName = EncodedString::from_unicode(ElementNames::WATER_NAME.c_str());
    addElement(StatNames::WATER_PEN, pen);
    SrStat grav = { EncodedString::from_unicode(ElementNames::GRAV_NAME.c_str()), 200, -100, true };
    addElement(StatNames::GRAVITY_RES, grav);
    addElement(StatNames::GRAVITY_AFF, grav);
    pen.displayName = EncodedString::from_unicode(ElementNames::GRAV_NAME.c_str());
    addElement(StatNames::GRAVITY_PEN, pen);
    SrStat poison = { EncodedString::from_unicode(ElementNames::POISON_NAME.c_str()), 200, -100, true };
    addElement(StatNames::POISON_RES, poison);
    addElement(StatNames::POISON_AFF, poison);
    pen.displayName = EncodedString::from_unicode(ElementNames::POISON_NAME.c_str());
    addElement(StatNames::POISON_PEN, pen);
    SrStat holy = { EncodedString::from_unicode(ElementNames::HOLY_NAME.c_str()), 200, -100, true };
    addElement(StatNames::HOLY_RES, holy);
    addElement(StatNames::HOLY_AFF, holy);
    pen.displayName = EncodedString::from_unicode(ElementNames::HOLY_NAME.c_str());
    addElement(StatNames::HOLY_PEN, pen);
    SrStat shadow = { EncodedString::from_unicode(ElementNames::SHADOW_NAME.c_str()), 200, -100, true };
    addElement(StatNames::SHADOW_RES, shadow);
    addElement(StatNames::SHADOW_AFF, shadow);
    pen.displayName = EncodedString::from_unicode(ElementNames::SHADOW_NAME.c_str());
    addElement(StatNames::SHADOW_PEN, pen);
    SrStat restore = { EncodedString::from_unicode(ElementNames::RESTORE_NAME.c_str()), 200, -100, true };
    addElement(StatNames::RESTORE_RES, restore);
    addElement(StatNames::RESTORE_AFF, restore);
    pen.displayName = EncodedString::from_unicode(ElementNames::RESTORE_NAME.c_str());
    addElement(StatNames::RESTORE_PEN, pen);


    SrStat slash = { EncodedString::from_unicode(ElementNames::SLASH_NAME.c_str()), 200, -100, true };
    addElement(StatNames::SLASH_ARM, slash);
    addElement(StatNames::SLASH_SKL, slash);
    SrStat blunt = { EncodedString::from_unicode(ElementNames::BLUNT_NAME.c_str()), 200, -100, true };
    addElement(StatNames::BLUNT_ARM, blunt);
    addElement(StatNames::BLUNT_SKL, holy);
    SrStat pierce = { EncodedString::from_unicode(ElementNames::PIERCE_NAME.c_str()), 200, -100, true };
    addElement(StatNames::PIERCE_ARM, shadow);
    addElement(StatNames::PIERCE_SKL, shadow);

    //Status Res/Pen

    SrStat res = { EncodedString::from_unicode(StatusNames::DEATH_NAME.c_str()), 200, -100, true };

    res.displayName = EncodedString::from_unicode(StatusNames::DEATH_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::DEATH_NAME.c_str());
    addElement(StatNames::DEATH_RES, res);
    addElement(StatNames::DEATH_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::CRITICAL_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::CRITICAL_NAME.c_str());
    addElement(StatNames::CRITICAL_RES, res);
    addElement(StatNames::CRITICAL_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::SLEEP_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::SLEEP_NAME.c_str());
    addElement(StatNames::SLEEP_RES, res);
    addElement(StatNames::SLEEP_PEN, pen);


    res.displayName = EncodedString::from_unicode(StatusNames::PSN_STS_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::PSN_STS_NAME.c_str());
    addElement(StatNames::POISON_RES_STS, res);
    addElement(StatNames::POISON_PEN_STS, pen);


    res.displayName = EncodedString::from_unicode(StatusNames::SADNESS_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::SADNESS_NAME.c_str());
    addElement(StatNames::SADNESS_RES, res);
    addElement(StatNames::SADNESS_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::FURY_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::FURY_NAME.c_str());
    addElement(StatNames::FURY_RES, res);
    addElement(StatNames::FURY_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::CONFUSION_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::CONFUSION_NAME.c_str());
    addElement(StatNames::CONFUSION_RES, res);
    addElement(StatNames::CONFUSION_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::SILENCE_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::SILENCE_NAME.c_str());
    addElement(StatNames::SILENCE_RES, res);
    addElement(StatNames::SILENCE_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::HASTE_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::HASTE_NAME.c_str());
    addElement(StatNames::HASTE_RES, res);
    addElement(StatNames::HASTE_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::SLOW_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::SLOW_NAME.c_str());
    addElement(StatNames::SLOW_RES, res);
    addElement(StatNames::SLOW_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::STOP_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::STOP_NAME.c_str());
    addElement(StatNames::STOP_RES, res);
    addElement(StatNames::STOP_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::FROG_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::FROG_NAME.c_str());
    addElement(StatNames::FROG_RES, res);
    addElement(StatNames::FROG_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::MINI_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::MINI_NAME.c_str());
    addElement(StatNames::MINI_RES, res);
    addElement(StatNames::MINI_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::SLOWNUMB_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::SLOWNUMB_NAME.c_str());
    addElement(StatNames::SLOWNUMB_RES, res);
    addElement(StatNames::SLOWNUMB_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::PETRIFY_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::PETRIFY_NAME.c_str());
    addElement(StatNames::PETRIFY_RES, res);
    addElement(StatNames::PETRIFY_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::REGEN_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::REGEN_NAME.c_str());
    addElement(StatNames::REGEN_RES, res);
    addElement(StatNames::REGEN_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::BARRIER_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::BARRIER_NAME.c_str());
    addElement(StatNames::BARRIER_RES, res);
    addElement(StatNames::BARRIER_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::MBARRIER_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::MBARRIER_NAME.c_str());
    addElement(StatNames::MBARRIER_RES, res);
    addElement(StatNames::MBARRIER_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::REFLECT_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::REFLECT_NAME.c_str());
    addElement(StatNames::REFLECT_RES, res);
    addElement(StatNames::REFLECT_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::DUAL_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::DUAL_NAME.c_str());
    addElement(StatNames::DUAL_RES, res);
    addElement(StatNames::DUAL_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::SHIELD_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::SHIELD_NAME.c_str());
    addElement(StatNames::SHIELD_RES, res);
    addElement(StatNames::SHIELD_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::DOOM_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::DOOM_NAME.c_str());
    addElement(StatNames::DOOM_RES, res);
    addElement(StatNames::DOOM_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::MANIPULATE_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::MANIPULATE_NAME.c_str());
    addElement(StatNames::MANIPULATE_RES, res);
    addElement(StatNames::REFLECT_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::BERSERK_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::BERSERK_NAME.c_str());
    addElement(StatNames::BERSERK_RES, res);
    addElement(StatNames::BERSERK_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::PEERLESS_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::PEERLESS_NAME.c_str());
    addElement(StatNames::PEERLESS_RES, res);
    addElement(StatNames::PEERLESS_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::PARALYSIS_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::PARALYSIS_NAME.c_str());
    addElement(StatNames::PARALYSIS_RES, res);
    addElement(StatNames::PARALYSIS_PEN, pen);

    res.displayName = EncodedString::from_unicode(StatusNames::BLIND_NAME.c_str());
    pen.displayName = EncodedString::from_unicode(StatusNames::BLIND_NAME.c_str());
    addElement(StatNames::BLIND_RES, res);
    addElement(StatNames::BLIND_PEN, pen);
}

void initStats(bool initResistances) {
    gContext.stats = SrStatRegistry(initResistances);
    srLogWrite("initialized %lu stats", (unsigned long)gContext.stats.resourceCount());
}


void finalizeStats() {
    finalizeRegistry<SrStat, InitStatEvent, SrStatRegistry>(gContext.stats, INIT_STATS);
}

SISTERRAY_API void registerStat(const char* statName, const char* displayName, i32 max, i32 min) {
    bool canGoNegative = min < 0;
    SrStat newStat{ EncodedString::from_unicode(displayName), max, min, canGoNegative };
    gContext.stats.addElement(std::string(statName), newStat);
}


SISTERRAY_API void setStat(const char* statName, i32* target, i32 value) {
    auto stat = gContext.stats.getElement(std::string(statName));
    if (value > stat.maxValue) {
        *target = stat.maxValue;
        return;
    }
    if (value < stat.minValue) {
        *target = stat.minValue;
        return;
    }
    *target = value;
}

SISTERRAY_API void incrementStat(const char* statName, i32* target, i32 increment) {
    setStat(statName, target, *target + increment);
}


SISTERRAY_API void decrementStat(const char* statName, i32* target, i32 decrement) {
    setStat(statName, target, *target - decrement);
}
