#ifndef WEAPONS_H
#define WEAPONS_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"
#include "base_item.h"

#define KERNEL_WEAPON_COUNT 128

typedef struct {
    WeaponData gameWeapon;
    AuxWeaponData auxData;
    EncodedString weaponName;
    EncodedString weaponDescription;
    EquipmentStatBoosts equipEffects;
    std::vector<std::string> attackElements;
} SrWeapon;
 
class SrWeaponRegistry : public SrNamedResourceRegistry<SrWeapon, std::string> {
public:
    SrWeaponRegistry(SrKernelStream* stream);
    SrWeaponRegistry() : SrNamedResourceRegistry<SrWeapon, std::string>() {}
};

u8 getWeaponIcon(u8 characterID);

#endif
