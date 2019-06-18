#ifndef WEAPONS_H
#define WEAPONS_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"
#include "base_item.h"

#define KERNEL_WEAPON_COUNT 128
 
class SrWeaponRegistry : public SrNamedResourceRegistry<WeaponData, std::string> {
public:
    SrWeaponRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<WeaponData, std::string>(stream) {}
    SrWeaponRegistry() : SrNamedResourceRegistry<WeaponData, std::string>() {}
};

class SrAuxWeaponRegistry : public SrNamedResourceRegistry<AuxWeaponData, std::string> {
public:
    SrAuxWeaponRegistry() : SrNamedResourceRegistry<AuxWeaponData, std::string>() {}
};

void initializeAuxWeaponRegistry();
u8 getWeaponIcon(u8 characterID);

#endif
