#ifndef ITEMS_H
#define ITEMS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>

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
    size_t          count;
    size_t          capacity;
    ItemData*       data;
} SrItemRegistry;

/*new structure holding additional item_id indexed data*/
typedef struct {
    u16 hp_heal_amount;
    u16 mp_heal_amount;
    u16 stat_to_boost;
    u16 character_restriction_mask; //Used to make certain items usable only by certain characters
} onUseItemData;
#endif
