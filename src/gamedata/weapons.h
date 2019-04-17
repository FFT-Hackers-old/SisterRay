#ifndef WEAPONS_H
#define WEAPONS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"
 
class SrWeaponRegistry : public SrNamedResourceRegistry<WeaponData, std::string> {
public:
    SrWeaponRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<WeaponData, std::string>(stream) {}
    SrWeaponRegistry() : SrNamedResourceRegistry<WeaponData, std::string>() {}
};

#endif
