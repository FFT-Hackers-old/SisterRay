#ifndef ACCESSORY_H
#define ACCESSORY_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"
#include "base_item.h"

#define KERNEL_ACCESSORY_COUNT 32

class SrAccessoryRegistry : public SrNamedResourceRegistry<AccessoryData, std::string> {
public:
    SrAccessoryRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<AccessoryData, std::string>(stream) {}
    SrAccessoryRegistry() : SrNamedResourceRegistry<AccessoryData, std::string>() {}
};

class SrAuxAccessoryRegistry : public SrNamedResourceRegistry<AuxAccessoryData, std::string> {
public:
    SrAuxAccessoryRegistry() : SrNamedResourceRegistry<AuxAccessoryData, std::string>() {}
};

void initializeAuxAccessoryRegistry();

#endif
