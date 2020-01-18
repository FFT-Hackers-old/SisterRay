#include "weapons.h"
#include "../impl.h"
#include "../party/stat_names.h"


SrWeaponRegistry::SrWeaponRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<SrWeapon, std::string>() {
    size_t read_size;
    WeaponData baseWeapon;

    auto idx = 0;
    while (1) {
        auto cmdIdx = 0;
        read_size = srKernelStreamRead(stream, &baseWeapon, sizeof(baseWeapon));
        if (read_size != sizeof(baseWeapon))
            break;
        SrWeapon weapon;
        weapon.equipEffects[StatNames::WEAPON_ATTACK].push_back(createGearBoost(SR_GEAR_WEAPON, idx, false, baseWeapon.weapon_strength, false));
        weapon.equipEffects[StatNames::WEAPON_ACCURACY].push_back(createGearBoost(SR_GEAR_WEAPON, idx, false, baseWeapon.weaponHitRate, false));
        weapon.weaponName = gContext.gameStrings.weapon_names.get_string(idx);
        weapon.weaponDescription = gContext.gameStrings.weapon_descriptions.get_string(idx);
        weapon.gameWeapon = baseWeapon;
        populatekernelStatBoosts(weapon.equipEffects, weapon.gameWeapon.stats_to_boost, weapon.gameWeapon.stat_boost_amounts, 4, idx, SR_GEAR_WEAPON);
        addElement(assembleGDataKey(idx), weapon);
        ++idx;
    }
}

SISTERRAY_API SrWeaponData getSrWeapon(u16 modItemID, const char* modName) {
    SrWeaponData apiWeapon = SrWeaponData();
    auto name = std::string(modName) + std::to_string(modItemID);
    auto& weapon = gContext.weapons.getElement(name);
    apiWeapon.baseData = weapon.gameWeapon;
    apiWeapon.auxData = weapon.auxData;
    apiWeapon.weaponName = weapon.weaponName.str();
    apiWeapon.weaponDesc = weapon.weaponDescription.str();
    return apiWeapon;
}

SISTERRAY_API void setSrWeaponData(SrWeaponData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    auto srWeapon = SrWeapon();
    srWeapon.gameWeapon = data.baseData;
    srWeapon.auxData = data.auxData;
    srWeapon.weaponName = EncodedString::from_unicode(data.weaponName);
    srWeapon.weaponDescription = EncodedString::from_unicode(data.weaponDesc);
    gContext.weapons.updateElement(name, srWeapon);
}

SISTERRAY_API void addSrWeapon(SrWeaponData data, u16 modItemID, const char* modName, u8 characterID) {
    auto name = std::string(modName) + std::to_string(modItemID);
    auto srWeapon = SrWeapon();
    srWeapon.gameWeapon = data.baseData;
    srWeapon.auxData = data.auxData;
    srWeapon.weaponName = EncodedString::from_unicode(data.weaponName);
    srWeapon.weaponDescription = EncodedString::from_unicode(data.weaponDesc);
    gContext.weapons.addElement(name, srWeapon);

    u8 iconType = getWeaponIcon(characterID);
    gContext.itemTypeData.appendItem(name, ITYPE_WEAPON, iconType);
}

SISTERRAY_API void initWeapons(SrKernelStream* stream) {
    gContext.weapons = SrWeaponRegistry(stream);
    gContext.itemTypeData.initializeAugmentedData(ITYPE_WEAPON, gContext.weapons.resourceCount());
    srLogWrite("kernel.bin: Loaded %lu weapons", (unsigned long)gContext.weapons.resourceCount());
}

u8 getWeaponIcon(u8 characterID) {
    switch (characterID) {
        case 0:
            return ICONTYPE_SWORD;
        case 1:
            return ICONTYPE_GATGUN;
        case 2:
            return ICONTYPE_GLOVE;
        case 3:
            return ICONTYPE_STAFF;
        case 4:
            return ICONTYPE_CLIP;
        case 5:
            return ICONTYPE_PHONE;
        case 6:
            return ICONTYPE_SHUR;
        case 7:
            return ICONTYPE_GUN;
        case 8:
            return ICONTYPE_SPEAR;
        default: {
            return ICONTYPE_SWORD;
        }
    }
}
