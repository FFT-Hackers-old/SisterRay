#include "weapons.h"
#include "../impl.h"
#include "stat_names.h"
#include "base_type_names.h"


SrWeaponRegistry::SrWeaponRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<SrWeapon, std::string>() {
    size_t read_size;
    GameWeapon baseWeapon;

    auto idx = 0;
    while (1) {
        auto cmdIdx = 0;
        read_size = srKernelStreamRead(stream, &baseWeapon, sizeof(baseWeapon));
        if (read_size != sizeof(baseWeapon))
            break;
        SrWeapon weapon;
        weapon.sharedBase.equipEffects[StatNames::WEAPON_ATTACK].push_back(createGearBoost(SR_GEAR_WEAPON, idx, false, baseWeapon.weaponStrength, false));
        weapon.sharedBase.equipEffects[StatNames::WEAPON_ACCURACY].push_back(createGearBoost(SR_GEAR_WEAPON, idx, false, baseWeapon.weaponHitRate, false));
        weapon.sharedBase.gearName = gContext.gameStrings.weapon_names.get_string(idx);
        weapon.sharedBase.gearDescription = gContext.gameStrings.weapon_descriptions.get_string(idx);
        weapon.gameWeapon = baseWeapon;
        weapon.weaponModelID = baseWeapon.weapon_model & 0xF;
        initializeWeaponStats(weapon);
        initializeWeaponElements(weapon);
        initializeWeaponAfflictions(weapon);
        populatekernelStatBoosts(weapon.sharedBase.equipEffects, weapon.gameWeapon.stats_to_boost, weapon.gameWeapon.stat_boost_amounts, 4, idx, SR_GEAR_WEAPON);
        setSrDamageInfo<SrWeapon>(weapon, baseWeapon.damageCalculation);
        addElement(assembleGDataKey(idx), weapon);
        ++idx;
    }
}

void initializeWeaponStats(SrWeapon& weapon) {
    auto& gameWeapon = weapon.gameWeapon;
    weapon.sharedBase.stats[StatNames::WEAPON_ATTACK].statValue = gameWeapon.weaponStrength;
    weapon.sharedBase.stats[StatNames::WEAPON_ACCURACY].statValue = gameWeapon.weaponHitRate;
    weapon.sharedBase.stats[StatNames::WEAPON_CRITRATE].statValue = gameWeapon.criticalRate;
}

void initializeWeaponElements(SrWeapon& weapon) {
    auto& attackElements = weapon.attackElements;
    for (auto elementIdx = 0; elementIdx < 16; elementIdx++) {
        if (!(weapon.gameWeapon.attackElementMask & (1 << elementIdx))) {
            continue;
        }
        attackElements.push_back(getElementIDFromIndex(elementIdx));
    }
}

void initializeWeaponAfflictions(SrWeapon& weapon) {
    auto& statusAttack = weapon.statusAttack;
    auto statusIdx = weapon.gameWeapon.status_attack;
    if (statusIdx == 0xFF) {
        return;
    }
    StatusInfliction infliction{ getStatusIDFromIndex(statusIdx), 63, false, false };
    statusAttack.push_back(infliction);
}

SISTERRAY_API SrWeaponData getSrWeapon(u16 modItemID, const char* modName) {
    SrWeaponData apiWeapon = SrWeaponData();
    auto name = std::string(modName) + std::to_string(modItemID);
    auto& weapon = gContext.weapons.getElement(name);
    apiWeapon.baseData = weapon.gameWeapon;
    apiWeapon.weaponName = weapon.sharedBase.gearName.str();
    apiWeapon.weaponDesc = weapon.sharedBase.gearDescription.str();
    return apiWeapon;
}

SISTERRAY_API void setSrWeaponData(SrWeaponData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    auto srWeapon = SrWeapon();
    srWeapon.gameWeapon = data.baseData;
    srWeapon.sharedBase.gearName = EncodedString::from_unicode(data.weaponName);
    srWeapon.sharedBase.gearDescription = EncodedString::from_unicode(data.weaponDesc);
    gContext.weapons.updateElement(name, srWeapon);
}

SISTERRAY_API void addSrWeapon(SrWeaponData data, u16 modItemID, const char* modName, u8 characterID) {
    auto name = std::string(modName) + std::to_string(modItemID);
    auto srWeapon = SrWeapon();
    srWeapon.gameWeapon = data.baseData;
    srWeapon.sharedBase.gearName = EncodedString::from_unicode(data.weaponName);
    srWeapon.sharedBase.gearDescription = EncodedString::from_unicode(data.weaponDesc);
    gContext.weapons.addElement(name, srWeapon);

    u8 iconType = getWeaponIcon(characterID);
    gContext.baseItems.appendItem(name, ItemTypeNames::WEAPON_TYPE, iconType);
}

SISTERRAY_API void initWeapons(SrKernelStream* stream) {
    gContext.weapons = SrWeaponRegistry(stream);
    gContext.baseItems.initializeAugmentedData(ItemTypeNames::WEAPON_TYPE, gContext.weapons.resourceCount());
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

void finalizeWeapons() {
    finalizeRegistry<SrWeapon, InitWeaponEvent, SrWeaponRegistry>(gContext.weapons, INIT_WEAPON);
}
