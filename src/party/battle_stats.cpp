#include "battle_stats.h"
#include "stat_names.h"
#include "../impl.h"


SrStatRegistry::SrStatRegistry(bool initResistances) : SrNamedResourceRegistry<SrStat, std::string>() {
    SrStat hp = { EncodedString::from_unicode(StatNames::HP_NAME.c_str()), 9999, false };
    addElement(StatNames::HP, hp);
    SrStat mp = { EncodedString::from_unicode(StatNames::MP_NAME.c_str()), 999, false };
    addElement(StatNames::MP, mp);
    SrStat str = { EncodedString::from_unicode(StatNames::STR_NAME.c_str()), 255, false };
    addElement(StatNames::STRENGTH, str);
    SrStat vit = { EncodedString::from_unicode(StatNames::VIT_NAME.c_str()), 255, false };
    addElement(StatNames::VITALITY, vit);
    SrStat mag = { EncodedString::from_unicode(StatNames::MAG_NAME.c_str()), 255, false };
    addElement(StatNames::MAGIC, mag);
    SrStat spr = { EncodedString::from_unicode(StatNames::SPR_NAME.c_str()), 255, false };
    addElement(StatNames::SPIRIT, spr);
    SrStat luck = { EncodedString::from_unicode(StatNames::LUCK_NAME.c_str()), 255, false };
    addElement(StatNames::LUCK, luck);
    SrStat dex = { EncodedString::from_unicode(StatNames::DEX_NAME.c_str()), 255, false };
    addElement(StatNames::DEXTERITY, dex);

    SrStat wpnAtk = { EncodedString::from_unicode(StatNames::WEAPON_ATTACK_NAME.c_str()), 255, false };
    addElement(StatNames::WEAPON_ATTACK, wpnAtk);
    SrStat wpnHit = { EncodedString::from_unicode(StatNames::WEAPON_ACCURACY_NAME.c_str()), 255, false };
    addElement(StatNames::WEAPON_ACCURACY, wpnHit);
    SrStat def = { EncodedString::from_unicode(StatNames::ARMOR_DEFENSE_NAME.c_str()), 255, false };
    addElement(StatNames::ARMOR_DEFENSE, def);
    SrStat mdef = { EncodedString::from_unicode(StatNames::ARMOR_MDEFENSE_NAME.c_str()), 255, false };
    addElement(StatNames::ARMOR_MDEFENSE, mdef);
    SrStat pev = { EncodedString::from_unicode(StatNames::EVADE_NAME.c_str()), 255, false };
    addElement(StatNames::EVADE, pev);
    SrStat mev = { EncodedString::from_unicode(StatNames::MEVADE_NAME.c_str()), 255, false };
    addElement(StatNames::MEVADE_NAME, mev);

    SrStat fres = { EncodedString::from_unicode(StatNames::FIRE_NAME.c_str()), 100, true };
    addElement(StatNames::FIRE_RES, fres);
    addElement(StatNames::FIRE_AFF, fres);
    SrStat ice = { EncodedString::from_unicode(StatNames::ICE_NAME.c_str()), 100, true };
    addElement(StatNames::ICE_RES, ice);
    addElement(StatNames::ICE_AFF, ice);
    SrStat light = { EncodedString::from_unicode(StatNames::LIGHT_NAME.c_str()), 100, true };
    addElement(StatNames::LIGHT_RES, light);
    addElement(StatNames::LIGHT_AFF, light);
    SrStat earth = { EncodedString::from_unicode(StatNames::EARTH_NAME.c_str()), 100, true };
    addElement(StatNames::EARTH_RES, earth);
    addElement(StatNames::EARTH_AFF, earth);
    SrStat wind = { EncodedString::from_unicode(StatNames::WIND_NAME.c_str()), 100, true };
    addElement(StatNames::WIND_RES, wind);
    addElement(StatNames::WIND_AFF, wind);
    SrStat water = { EncodedString::from_unicode(StatNames::WATER_NAME.c_str()), 100, true };
    addElement(StatNames::WATER_RES, water);
    addElement(StatNames::WATER_AFF, water);
    SrStat grav = { EncodedString::from_unicode(StatNames::GRAV_NAME.c_str()), 100, true };
    addElement(StatNames::GRAVITY_RES, grav);
    addElement(StatNames::GRAVITY_AFF, grav);
    SrStat poison = { EncodedString::from_unicode(StatNames::POISON_NAME.c_str()), 100, true };
    addElement(StatNames::POISON_RES, poison);
    addElement(StatNames::POISON_AFF, poison);
    SrStat holy = { EncodedString::from_unicode(StatNames::HOLY_NAME.c_str()), 100, true };
    addElement(StatNames::HOLY_RES, holy);
    addElement(StatNames::HOLY_AFF, holy);
    SrStat shadow = { EncodedString::from_unicode(StatNames::SHADOW_NAME.c_str()), 100, true };
    addElement(StatNames::SHADOW_RES, holy);
    addElement(StatNames::SHADOW_AFF, holy);
}

void initStats(bool initResistances) {
    gContext.stats = SrStatRegistry(initResistances);
    srLogWrite("initialized %lu stats", (unsigned long)gContext.stats.resourceCount());
}

