#ifndef ARMOR_H
#define ARMOR_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "sr_registry_template.h"

#pragma pack(push, 1)
typedef struct {
    u8      unknown;
    u8      damageType;
    u8      defense;
    u8      magicDefense;
    u8      evade;
    u8      magicEvade;
    u8      statusDefense;
    u16     unknown2;
    u8      materiaSlots[8];
    u8      materiaGrowth;
    u16     equipMask;
    u16     elementalDefenseMask;
    u16     unknown3;
    u8      statsToBoost[4];
    u8      statBoostAmounts[4];
    u16     restrictionMask;
    u16     unknown4;
} ArmorData;
#pragma pack(pop)

class SrArmorRegistry : SrResourceRegistry<ArmorData> {
public:
    SrArmorRegistry(SrKernelStream* stream) : SrResourceRegistry<ArmorData>::SrResourceRegistry(SrKernelStream* stream) {};
};

#endif
