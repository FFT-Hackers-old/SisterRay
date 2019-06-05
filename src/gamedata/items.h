#ifndef ITEMS_H
#define ITEMS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"


class SrItemRegistry : public SrNamedResourceRegistry<ItemData, std::string> {
public:
 SrItemRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<ItemData, std::string>(stream) {};
 SrItemRegistry(): SrNamedResourceRegistry<ItemData, std::string>() {}
};

class SrOnUseItemDataRegistry : public SrResourceRegistry<onUseItemData> {
public:
    SrOnUseItemDataRegistry(SrKernelStream* stream) : SrResourceRegistry<onUseItemData>(stream) {};
    SrOnUseItemDataRegistry() : SrResourceRegistry<onUseItemData>() {}
};

bool canCharacterUseItem(u8 character_ID, u16 item_id);
void initOnUseItemDataRegistry();
void createOnUseItemData(u16 hp_heal_amount, u16 mp_heal_amount, u16 stat_to_boost,
    u16 character_restriction_mask, u8 hp_heal_percent,
    u8 mp_heal_percent, bool can_revive, bool target_all, bool requires_target);

#endif
