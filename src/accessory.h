#ifndef ACCESSORY_H
#define ACCESSORY_H

#include <SisterRay/types.h>

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

typedef struct
{
    size_t          count;
    size_t          capacity;
    AccessoryData*  data;
} SrAccessoryRegistry;

#endif
