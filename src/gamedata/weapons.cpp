#include "weapons.h"
#include "../impl.h"

SISTERRAY_API SrWeaponData getWeapon(u16 modItemID, const char* modName) {
    SrWeaponData srWeapon = SrWeaponData();
    auto name = std::string(modName) + std::to_string(modItemID);
    srWeapon.baseData = gContext.weapons.get_element(name);
    srWeapon.auxData = gContext.auxWeapons.get_element(name);

    const ItemTypeData& typeData = gContext.itemTypeData.get_element(name);
    auto relativeIndex = typeData.type_relative_id;
    srWeapon.weaponName = gContext.gameStrings.weapon_names.get_string(relativeIndex);
    srWeapon.weaponDesc = gContext.gameStrings.weapon_descriptions.get_string(relativeIndex);

    return srWeapon;
}

SISTERRAY_API void setWeaponData(SrWeaponData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    gContext.weapons.update_element(name, data.baseData);
    gContext.auxWeapons.update_element(name, data.auxData);

    const ItemTypeData& typeData = gContext.itemTypeData.get_element(name);
    auto relativeIndex = typeData.type_relative_id;
    gContext.gameStrings.weapon_names.update_resource(relativeIndex, EncodedString::from_unicode(data.weaponName));
    gContext.gameStrings.weapon_descriptions.update_resource(relativeIndex, EncodedString::from_unicode(data.weaponDesc));
}

SISTERRAY_API void addWeapon(SrWeaponData data, u16 modweaponID, const char* modName, u8 characterID) {
    auto name = std::string(modName) + std::to_string(modweaponID);
    gContext.weapons.add_element(name, data.baseData);
    gContext.auxWeapons.add_element(name, data.auxData);

    u8 iconType = getWeaponIcon(characterID);
    gContext.itemTypeData.append_item(name, ITYPE_WEAPON, iconType);

    const ItemTypeData& typeData = gContext.itemTypeData.get_element(name);
    auto relativeIndex = typeData.type_relative_id;
    gContext.gameStrings.weapon_names.add_resource(EncodedString::from_unicode(data.weaponName));
    gContext.gameStrings.weapon_descriptions.add_resource(EncodedString::from_unicode(data.weaponDesc));
}

/*Patch references to items to reference to reference the new gContext objects*/
static void patch_weapons() {
    return;
}

/*Initialize the registry with the correct stat info for kernel weapons*/
void initializeAuxWeaponRegistry() {
    for (auto i = 0; i < KERNEL_WEAPON_COUNT;  ++i) {
        auto name = std::string(BASE_PREFIX) + std::to_string(i);
        auto& kernelWeapon = gContext.weapons.get_element(name);

        ActorStatBoosts boosts = ActorStatBoosts();
        populatekernelStatBoosts(&(kernelWeapon.stats_to_boost[0]), &(kernelWeapon.stat_boost_amounts[0]), boosts, 4);
        AuxWeaponData auxWeapon = { boosts };
        gContext.auxWeapons.add_element(name, auxWeapon);
    }
}

SISTERRAY_API void init_weapon(SrKernelStream* stream) {
    gContext.weapons = SrWeaponRegistry(stream);
    patch_weapons();

    gContext.itemTypeData.initialize_augmented_data(ITYPE_WEAPON, gContext.weapons.resource_count());
    gContext.auxWeapons = SrAuxWeaponRegistry();
    initializeAuxWeaponRegistry();
    srLogWrite("kernel.bin: Loaded %lu weapons", (unsigned long)gContext.weapons.resource_count());
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
