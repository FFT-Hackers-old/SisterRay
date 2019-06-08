#ifndef ARMOR_H
#define ARMOR_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"
#include "base_item.h"

#define KERNEL_ARMOR_COUNT 32

class SrArmorRegistry : public SrNamedResourceRegistry<ArmorData, std::string> {
public:
    SrArmorRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<ArmorData, std::string>(stream) {}
    SrArmorRegistry() : SrNamedResourceRegistry<ArmorData, std::string>(){}
};

class SrAuxArmorRegistry : public SrNamedResourceRegistry<AuxArmorData, std::string> {
public:
    SrAuxArmorRegistry() : SrNamedResourceRegistry<ArmorData, std::string>() {}
};


void initializeAuxArmorRegistry();

#endif
