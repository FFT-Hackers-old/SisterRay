#ifndef ACCESSORY_H
#define ACCESSORY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "sr_registry_template.h"

#pragma pack(push, 1)
typedef struct {
    u8      statsToBoost[2];
    u8      statBoostAmounts[2];
    u8      elementInteraction;
    u8      specialEffect; /*We will hook in custom effects here*/
    u16     elementsMask;
    u32     statusDefenseMask;
    u16     equipMask;
    u16     restrictionMask;
} AccessoryData;
#pragma pack(pop)

class SrAccessoryRegistry : SrResourceRegistry<AccessoryData> {
public:
    SrAccessoryRegistry(SrKernelStream* stream) : SrResourceRegistry<AccessoryData>::SrResourceRegistry(SrKernelStream* stream) {};
};

#endif
