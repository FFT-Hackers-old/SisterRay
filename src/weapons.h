#ifndef WEAPONS_H
#define WEAPONS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "sr_registry_template.h"

#pragma pack(push, 1)
typedef struct {
    u8      target_flags;
    u8      attack_effect_id;
    u8      damage_calculation;
    u8      unused_byte;
    u8      weapon_strength;
    u8      status_attack;
    u8      materia_growth;
    u8      critical_rate;
    u8      weapon_hit_rate;
    u8      weapon_model;
    u8      padding;
    u8      sound_access_mask;
    u16     camera_movement;
    u16     equip_mask;
    u16     attack_element;
    u16     unknown;
    u8      stats_to_boost[4];
    u8      stat_boost_amounts[4];
    u8      materia_slots[8];
    u8      normal_sound_id;
    u8      crit_sound_id;
    u8      miss_sound_id;
    u8      impact_effect_id;
    u16     special_attack_flags;
    u16     restriction_mask;
} WeaponData;
#pragma pack(pop)

class SrWeaponRegistry : public SrResourceRegistry<WeaponData> {
public:
    SrWeaponRegistry(SrKernelStream* stream) : SrResourceRegistry<WeaponData>(stream) {}
    SrWeaponRegistry() : SrResourceRegistry<WeaponData>() {}
};

#endif
