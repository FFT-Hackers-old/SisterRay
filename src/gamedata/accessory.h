#ifndef ACCESSORY_H
#define ACCESSORY_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"
#include "base_item.h"

typedef struct {
    Equippable sharedBase;
    GameAccessory gameAccessory;
} SrAccessory;

#define KERNEL_ACCESSORY_COUNT 32

class SrAccessoryRegistry : public SrNamedResourceRegistry<SrAccessory, std::string> {
public:
    SrAccessoryRegistry(SrKernelStream* stream);
    SrAccessoryRegistry() : SrNamedResourceRegistry<SrAccessory, std::string>() {}
};

void initializeAccessoryElements(SrAccessory& accessory, u16 relativeID);

#endif
