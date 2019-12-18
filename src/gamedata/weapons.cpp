#include "weapons.h"
#include "../impl.h"

SISTERRAY_API SrWeaponData getSrWeapon(u16 modItemID, const char* modName) {
    SrWeaponData srWeapon = SrWeaponData();
    auto name = std::string(modName) + std::to_string(modItemID);
    srWeapon.baseData = gContext.weapons.getElement(name);
    srWeapon.auxData = gContext.auxWeapons.getElement(name);

    const ItemTypeData& typeData = gContext.itemTypeData.getElement(name);
    auto relativeIndex = typeData.typeRelativeID;
    srWeapon.weaponName = gContext.gameStrings.weapon_names.get_string(relativeIndex);
    srWeapon.weaponDesc = gContext.gameStrings.weapon_descriptions.get_string(relativeIndex);

    return srWeapon;
}

SISTERRAY_API void setSrWeaponData(SrWeaponData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    gContext.weapons.updateElement(name, data.baseData);
    gContext.auxWeapons.updateElement(name, data.auxData);

    const ItemTypeData& typeData = gContext.itemTypeData.getElement(name);
    auto relativeIndex = typeData.typeRelativeID;
    gContext.gameStrings.weapon_names.updateResource(relativeIndex, EncodedString::from_unicode(data.weaponName));
    gContext.gameStrings.weapon_descriptions.updateResource(relativeIndex, EncodedString::from_unicode(data.weaponDesc));
}

SISTERRAY_API void addSrWeapon(SrWeaponData data, u16 modweaponID, const char* modName, u8 characterID) {
    auto name = std::string(modName) + std::to_string(modweaponID);
    gContext.weapons.addElement(name, data.baseData);
    gContext.auxWeapons.addElement(name, data.auxData);

    u8 iconType = getWeaponIcon(characterID);
    gContext.itemTypeData.appendItem(name, ITYPE_WEAPON, iconType);

    const ItemTypeData& typeData = gContext.itemTypeData.getElement(name);
    auto relativeIndex = typeData.typeRelativeID;
    gContext.gameStrings.weapon_names.addResource(EncodedString::from_unicode(data.weaponName));
    gContext.gameStrings.weapon_descriptions.addResource(EncodedString::from_unicode(data.weaponDesc));
}

/*Initialize the registry with the correct stat info for kernel weapons*/
void initializeAuxWeaponRegistry() {
    for (auto i = 0; i < KERNEL_WEAPON_COUNT;  ++i) {
        auto name = std::string(BASE_PREFIX) + std::to_string(i);
        auto& kernelWeapon = gContext.weapons.getElement(name);

        ActorStatBoosts boosts = ActorStatBoosts();
        populatekernelStatBoosts(&(kernelWeapon.stats_to_boost[0]), &(kernelWeapon.stat_boost_amounts[0]), boosts, 4);
        AuxWeaponData auxWeapon = { boosts };
        gContext.auxWeapons.addElement(name, auxWeapon);
    }
}

SISTERRAY_API void init_weapon(SrKernelStream* stream) {
    gContext.weapons = SrWeaponRegistry(stream);
    gContext.itemTypeData.initializeAugmentedData(ITYPE_WEAPON, gContext.weapons.resourceCount());
    gContext.auxWeapons = SrAuxWeaponRegistry();
    initializeAuxWeaponRegistry();
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
