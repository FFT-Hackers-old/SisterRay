#ifndef ITEMS_H
#define ITEMS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "sr_registry_template.h"

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

class SrItemRegistry : public SrResourceRegistry<ItemData> {
public:
 SrItemRegistry(SrKernelStream* stream) : SrResourceRegistry<ItemData>(stream) {};
 SrItemRegistry(): SrResourceRegistry<ItemData>() {}
};

/*new structure holding additional item_id indexed data*/
typedef struct {
    u16 hp_heal_amount;
    u16 mp_heal_amount;
    u16 stat_to_boost;
    u16 character_restriction_mask; //Used to make certain items usable only by certain characters
    u8 hp_heal_percent;
    u8 mp_heal_percent;
    bool can_revive;
    bool target_all;
} onUseItemData;

class SrOnUseItemDataRegistry : public SrResourceRegistry<onUseItemData> {
public:
    SrOnUseItemDataRegistry(SrKernelStream* stream) : SrResourceRegistry<onUseItemData>(stream) {};
    SrOnUseItemDataRegistry() : SrResourceRegistry<onUseItemData>() {}
};

bool character_can_use_item(u8 character_ID, u16 item_id);
void initOnUseDataRegistry();
void createOnUseItemData(u16 hp_heal_amount, u16 mp_heal_amount, u16 stat_to_boost,
    u16 character_restriction_mask, u8 hp_heal_percent,
    u8 mp_heal_percent, bool can_revive, bool target_all);

#endif
