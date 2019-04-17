#ifndef ARMOR_H
#define ARMOR_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"

#pragma pack(push, 1)
typedef struct {
    u8      unknown;
    u8      damage_type;
    u8      defense;
    u8      magic_defense;
    u8      evade;
    u8      magic_evade;
    u8      status_defense;
    u16     unknown2;
    u8      materia_slots[8];
    u8      materia_growth;
    u16     equip_mask;
    u16     elemental_defense_mask;
    u16     unknown3;
    u8      stats_to_boost[4];
    u8      stat_boost_amounts[4];
    u16     restriction_mask;
    u16     unknown4;
} ArmorData;
#pragma pack(pop)

class SrArmorRegistry : public SrNamedResourceRegistry<ArmorData, std::string> {
public:
    SrArmorRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<ArmorData, std::string>(stream) {}
    SrArmorRegistry() : SrNamedResourceRegistry<ArmorData, std::string>(){}
};

#endif
