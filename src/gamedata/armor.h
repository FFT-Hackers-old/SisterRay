#ifndef ARMOR_H
#define ARMOR_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"

class SrArmorRegistry : public SrNamedResourceRegistry<ArmorData, std::string> {
public:
    SrArmorRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<ArmorData, std::string>(stream) {}
    SrArmorRegistry() : SrNamedResourceRegistry<ArmorData, std::string>(){}
};

#endif
