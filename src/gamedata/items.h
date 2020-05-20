#ifndef ITEMS_H
#define ITEMS_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"
#include "../EncodedString.h"
#include "base_item.h"
#include "attacks.h"

typedef struct {
    ItemData gameItem;
    ConsumableUseData auxData;
    EncodedString itemName;
    EncodedString itemDescription;
    u16 attackIdx; //The true index of the SrAttack associated with this item
} SrItem;

class SrItemRegistry : public SrNamedResourceRegistry<SrItem, std::string> {
public:
 SrItemRegistry(SrKernelStream* stream);
 SrItemRegistry(): SrNamedResourceRegistry<SrItem, std::string>() {}
};

SrAttack attackFromItem(const SrItem& item);
bool canCharacterUseItem(u8 character_ID, u16 materiaID);
ConsumableUseData getOnUseData(u16 itemID);
ConsumableUseData createOnUseItemData(u16 hp_heal_amount, u16 mp_heal_amount, u16 stat_to_boost,
    u16 character_restriction_mask, u8 hp_heal_percent,
    u8 mp_heal_percent, bool can_revive, bool target_all, bool requires_target);

#endif
