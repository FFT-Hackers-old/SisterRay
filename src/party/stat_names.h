#ifndef STAT_NAMES_H
#define STAT_NAMES_H

#include <string>

namespace StatNames {
    const std::string HP = "HP";
    const std::string MP = "MP";
    const std::string STRENGTH = "STR";
    const std::string VITALITY = "VIT";
    const std::string MAGIC = "MAG";
    const std::string SPIRIT = "SPR";
    const std::string DEXTERITY = "DEX";
    const std::string LUCK = "LCK";

    // Display Names for primary stats
    const std::string HP_NAME = "HP";
    const std::string MP_NAME = "MP";
    const std::string STR_NAME = "Strength";
    const std::string VIT_NAME= "Vitality";
    const std::string MAG_NAME = "Magic";
    const std::string SPR_NAME = "Spirit";
    const std::string DEX_NAME = "Dexterity";
    const std::string LUCK_NAME = "LCK";

    const std::string WEAPON_ATTACK = "WPNATK";
    const std::string WEAPON_ACCURACY = "WPNHIT";
    const std::string ARMOR_DEFENSE = "ARMDEF";
    const std::string ARMOR_MDEFENSE = "ARMMAGDEF";
    const std::string WEAPON_MAGIC = "WPNMATK";
    const std::string EVADE = "PEV";
    const std::string MEVADE = "PEV";

    const std::string WEAPON_ATTACK_NAME = "Weapon Atk";
    const std::string WEAPON_ACCURACY_NAME = "Weapon Hit";
    const std::string ARMOR_DEFENSE_NAME = "Defense";
    const std::string ARMOR_MDEFENSE_NAME = "M.Defense";
    const std::string WEAPON_MAGIC_NAME = "M.Attack";
    const std::string EVADE_NAME = "Evade";
    const std::string MEVADE_NAME = "M.Evade";

    const std::string SLASH_ARM = "SLASHRES";
    const std::string PIERCE_ARM = "PIERCERES";
    const std::string BLUNT_ARM = "BLUNTRES";

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
}
#endif
