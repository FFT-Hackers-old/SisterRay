#ifndef STAT_NAMES_H
#define STAT_NAMES_H

#include <string>

namespace StatNames {
    const std::string HP = "HP";
    const std::string MP = "MP";
    const std::string STRENGTH = "STR";
    const std::string DEXTERITY = "DEX";
    const std::string VITALITY = "VIT";
    const std::string MAGIC = "MAG";
    const std::string FOCUS = "FOC";
    const std::string SPIRIT = "SPR";
    const std::string AGILITY = "AGI";
    const std::string LUCK = "LCK";

    // Display Names for primary stats
    const std::string HP_NAME = "HP";
    const std::string MP_NAME = "MP";
    const std::string STR_NAME = "Strength";
    const std::string DEX_NAME = "Dexterity";
    const std::string VIT_NAME= "Vitality";
    const std::string MAG_NAME = "Magic";
    const std::string FOCUS_NAME = "Focus";
    const std::string SPR_NAME = "Spirit";
    const std::string AGILITY_NAME = "Agility";
    const std::string LUCK_NAME = "Luck";

    const std::string WEAPON_ATTACK = "WPNATK";
    const std::string WEAPON_ACCURACY = "WPNHIT";
    const std::string WEAPON_CRITRATE = "WPNCRIT";
    const std::string ARMOR_DEFENSE = "ARMDEF";
    const std::string ARMOR_MDEFENSE = "ARMMAGDEF";
    const std::string WEAPON_MAGIC = "WPNMATK";
    const std::string EVADE = "PEV";
    const std::string MEVADE = "MEV";
    const std::string MAGIC_ACCURACY = "MAGHIT";

    const std::string WEAPON_ATTACK_NAME = "Weapon Atk";
    const std::string WEAPON_ACCURACY_NAME = "Weapon Hit";
    const std::string ARMOR_DEFENSE_NAME = "Defense";
    const std::string ARMOR_MDEFENSE_NAME = "M.Defense";
    const std::string WEAPON_MAGIC_NAME = "M.Attack";
    const std::string EVADE_NAME = "Evade";
    const std::string MEVADE_NAME = "M.Evade";
    const std::string MAGIC_ACCURACY_NAME = "Magic Acc.";

    const std::string SLASH_ARM = "SLASHRES";
    const std::string PIERCE_ARM = "PIERCERES";
    const std::string BLUNT_ARM = "BLUNTRES";

    const std::string SLASH_SKL = "SLASHSKL";
    const std::string PIERCE_SKL = "PIERCESKL";
    const std::string BLUNT_SKL = "BLUNTSKL";

    // New Elemental Resistances
    const std::string FIRE_RES = "FIRERES";
    const std::string ICE_RES = "ICERES";
    const std::string LIGHT_RES = "LITRES";
    const std::string EARTH_RES = "ETHRES";
    const std::string WATER_RES = "WATRES";
    const std::string WIND_RES = "WINDRES";
    const std::string GRAVITY_RES = "GRAVRES";
    const std::string POISON_RES = "PSNRES";
    const std::string HOLY_RES = "HOLYRES";
    const std::string SHADOW_RES = "SHADRES";
    const std::string RESTORE_RES = "RESTORERES";

    //Element Affinities
    const std::string FIRE_AFF = "FIREAFF";
    const std::string ICE_AFF = "ICEAFF";
    const std::string LIGHT_AFF = "LITAFF";
    const std::string EARTH_AFF = "ETHAFF";
    const std::string WATER_AFF = "WATAFF";
    const std::string WIND_AFF = "WINDAFF";
    const std::string GRAVITY_AFF = "GRAVAFF";
    const std::string POISON_AFF = "PSNAFF";
    const std::string HOLY_AFF = "HOLYAFF";
    const std::string SHADOW_AFF = "SHADAFF";
    const std::string RESTORE_AFF = "RESTOREAFF";

    const std::string FIRE_PEN = "FIREPEN";
    const std::string ICE_PEN = "ICEPEN";
    const std::string LIGHT_PEN = "LITPEN";
    const std::string EARTH_PEN = "ETHPEN";
    const std::string WATER_PEN = "WATPEN";
    const std::string WIND_PEN = "WINDPEN";
    const std::string GRAVITY_PEN = "GRAVPEN";
    const std::string POISON_PEN = "PSNPEN";
    const std::string HOLY_PEN = "HOLYPEN";
    const std::string SHADOW_PEN = "SHADPEN";
    const std::string RESTORE_PEN = "RESTOREPEN";

    // New Status Resistances
    const std::string DEATH_RES = "DEATHRES";
    const std::string CRITICAL_RES = "CRITICALRES";
    const std::string SLEEP_RES = "SLEEPRES";
    const std::string POISON_RES_STS = "PSNSTSRES";
    const std::string SADNESS_RES = "SADNESSRES";
    const std::string FURY_RES = "FURYRES";
    const std::string CONFUSION_RES = "CONFURES";
    const std::string SILENCE_RES = "SILENCERES";
    const std::string HASTE_RES = "HASTERES";
    const std::string SLOW_RES = "SLOWRES";
    const std::string STOP_RES = "STOPRES";
    const std::string FROG_RES = "FROGRES";
    const std::string MINI_RES = "SMALLRES";
    const std::string SLOWNUMB_RES = "SLOWNUMBRES";
    const std::string PETRIFY_RES = "STONERES";
    const std::string REGEN_RES = "REGENRES";
    const std::string BARRIER_RES = "BARRIERRES";
    const std::string MBARRIER_RES = "MBARRIERRES";
    const std::string REFLECT_RES = "REFLECTRES";
    const std::string DUAL_RES = "DUALRES";
    const std::string SHIELD_RES = "SHIELDRES";
    const std::string DOOM_RES = "DOOMRES";
    const std::string MANIPULATE_RES = "MANIPRES";
    const std::string BERSERK_RES = "BERSERKRES";
    const std::string PEERLESS_RES = "PEER_RES";
    const std::string PARALYSIS_RES = "PARA_RES";
    const std::string BLIND_RES = "BLIND_RES";
    const std::string DRAIN_RES = "DRAIN_RES";
    const std::string ZOMBIE_RES = "ZOMBIE_RES";
    const std::string RESIST_RES = "RESIST_RES";
    const std::string LUCKYGIRL_RES = "LUCKY_RES";
    const std::string IMPRISON_RES = "IMPRISON_RES";

    // New Status Penetration
    const std::string DEATH_PEN = "DEATHPEN";
    const std::string CRITICAL_PEN = "CRITICALPEN";
    const std::string SLEEP_PEN = "SLEEPPEN";
    const std::string POISON_PEN_STS = "PSNSTSPEN";
    const std::string SADNESS_PEN = "SADNESSPEN";
    const std::string FURY_PEN = "FURYPEN";
    const std::string CONFUSION_PEN = "CONFUPEN";
    const std::string SILENCE_PEN = "SILENCEPEN";
    const std::string HASTE_PEN = "HASTEPEN";
    const std::string SLOW_PEN = "SLOWPEN";
    const std::string STOP_PEN = "STOPPEN";
    const std::string FROG_PEN = "FROGPEN";
    const std::string MINI_PEN = "SMALLPEN";
    const std::string SLOWNUMB_PEN = "SLOWNUMBPEN";
    const std::string PETRIFY_PEN = "STONEPEN";
    const std::string REGEN_PEN = "REGENPEN";
    const std::string BARRIER_PEN = "BARRIERPEN";
    const std::string MBARRIER_PEN = "MBARRIERPEN";
    const std::string REFLECT_PEN = "REFLECTPEN";
    const std::string DUAL_PEN = "DUALPEN";
    const std::string SHIELD_PEN = "SHIELDPEN";
    const std::string DOOM_PEN = "DOOMPEN";
    const std::string MANIPULATE_PEN = "MANIPPEN";
    const std::string BERSERK_PEN = "BERSERKPEN";
    const std::string PEERLESS_PEN = "PEER_PEN";
    const std::string PARALYSIS_PEN = "PARA_PEN";
    const std::string BLIND_PEN = "BLIND_PEN";
    const std::string DRAIN_PEN = "DRAIN_PEN";
    const std::string ZOMBIE_PEN = "ZOMBIE_PEN";
    const std::string RESIST_PEN = "RESIST_PEN";
    const std::string LUCKYGIRL_PEN = "LUCKY_PEN";
    const std::string IMPRISON_PEN = "IMPRISON_PEN";
}
#endif
