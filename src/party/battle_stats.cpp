#include "battle_stats.h"
#include "stat_names.h"


SrStatRegistry::SrStatRegistry() : SrNamedResourceRegistry<SrStat, std::string>() {
    SrStat hp = { EncodedString(StatNames::HP_NAME.c_str()), 9999, false };
    addElement(StatNames::HP, hp);
    SrStat mp = { EncodedString(StatNames::MP_NAME.c_str()), 999, false };
    addElement(StatNames::MP, mp);
    SrStat str = { EncodedString(StatNames::STR_NAME.c_str()), 255, false };
    addElement(StatNames::STRENGTH, str);
    SrStat vit = { EncodedString(StatNames::VIT_NAME.c_str()), 255, false };
    addElement(StatNames::VITALITY, vit);
    SrStat mag = { EncodedString(StatNames::MAG_NAME.c_str()), 255, false };
    addElement(StatNames::MAGIC, mag);
    SrStat spr = { EncodedString(StatNames::SPR_NAME.c_str()), 255, false };
    addElement(StatNames::SPIRIT, spr);
    SrStat luck = { EncodedString(StatNames::LUCK_NAME.c_str()), 255, false };
    addElement(StatNames::LUCK, luck);
    SrStat dex = { EncodedString(StatNames::DEX_NAME.c_str()), 255, false };
    addElement(StatNames::DEXTERITY, dex);

    SrStat wpnAtk = { EncodedString(StatNames::WEAPON_ATTACK_NAME.c_str()), 255, false };
    addElement(StatNames::WEAPON_ATTACK, wpnAtk);
    SrStat wpnHit = { EncodedString(StatNames::WEAPON_ACCURACY_NAME.c_str()), 255, false };
    addElement(StatNames::WEAPON_ACCURACY, wpnHit);
    SrStat def = { EncodedString(StatNames::ARMOR_DEFENSE_NAME.c_str()), 255, false };
    addElement(StatNames::ARMOR_DEFENSE, def);
    SrStat mdef = { EncodedString(StatNames::ARMOR_MDEFENSE_NAME.c_str()), 255, false };
    addElement(StatNames::ARMOR_MDEFENSE, mdef);
    SrStat pev = { EncodedString(StatNames::EVADE_NAME.c_str()), 255, false };
    addElement(StatNames::EVADE, pev);
    SrStat mev = { EncodedString(StatNames::MEVADE_NAME.c_str()), 255, false };
    addElement(StatNames::MEVADE_NAME, mev);
}
