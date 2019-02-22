#ifndef ACCESSORY_H
#define ACCESSORY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "sr_registry_template.h"

#pragma pack(push, 1)
typedef struct {
    u8      stats_to_boost[2];
    u8      stat_boost_amounts[2];
    u8      elemental_interaction;
    u8      special_effect; /*We will hook in custom effects here*/
    u16     elements_mask;
    u32     status_defense_mask;
    u16     equip_mask;
    u16     restriction_mask;
} AccessoryData;
#pragma pack(pop)

class SrAccessoryRegistry : public SrResourceRegistry<AccessoryData> {
public:
    SrAccessoryRegistry(SrKernelStream* stream) : SrResourceRegistry<AccessoryData>(stream) {};
};

#endif
