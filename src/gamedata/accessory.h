#ifndef ACCESSORY_H
#define ACCESSORY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"

class SrAccessoryRegistry : public SrNamedResourceRegistry<AccessoryData, std::string> {
public:
    SrAccessoryRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<AccessoryData, std::string>(stream) {};
    SrAccessoryRegistry() : SrNamedResourceRegistry<AccessoryData, std::string>() {}
};

#endif
