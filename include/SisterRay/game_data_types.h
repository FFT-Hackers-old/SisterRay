#ifndef GAME_DATA_TYPES_H
#define GAME_DATA_TYPES_H

typedef enum {
    SR_GEAR_WEAPON,
    SR_GEAR_ARMOR
} SrGearType;

#pragma pack(push, 1)
typedef struct {
    u8 commandMenuID;
    u8 targetingFlags;
    u8 align[2];
    u16 singleCameraID;
    u16 multipleCameraID;
} CommandData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u8      unknown[8];
    u16     camera_data;
    u16     restriction_mask;
    u8      target_flags;
    u8      attack_effect_id;
    u8      damage_forumla;
    u8      item_power;
    u8      resource_conditions;
    u8      status_change_mask;
    u8      additional_effect_id;
    u8      additional_effect_modifier;
    u32     status_mask;
    u16     attack_element_mask;
    u16     special_attack_flags;
} ItemData;
#pragma pack(pop)

typedef struct {
    u16 hp_heal_amount;
    u16 mp_heal_amount;
    u16 stat_to_boost;
    u16 characterRestrictionMask; //Used to make certain items usable only by certain characters
    u8 hp_heal_percent;
    u8 mp_heal_percent;
    bool can_revive;
    bool target_all;
    bool requires_target;
} onUseItemData;

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

#pragma pack(push, 1)
typedef struct {
    u16     apLevel[4];
    u8      equipEffect;
    u8      statusEffects[3];
    u8      elementType;
    u8      type;
    u8      data[6];
} MateriaData;
#pragma pack(pop)

typedef struct {
    u16 item_id;
    u32 materia_ap; //u24 originally 
} MateriaInventoryEntry;


#endif // !GAME_DATA_TYPES_H
