#include "items.h"
#include "../impl.h"

SISTERRAY_API SrConsumableData getSrItem(u16 modItemID, const char* modName) {
    SrConsumableData srItem = SrConsumableData();
    auto name = std::string(modName) + std::to_string(modItemID);
    srItem.baseData = gContext.items.get_element(name);
    srItem.useData = gContext.itemOnUseData.get_element(name);

    ItemTypeData typeData = gContext.itemTypeData.get_element(name);
    auto relativeIndex = typeData.type_relative_id;
    srItem.itemName = gContext.gameStrings.item_names.get_string(relativeIndex);
    srItem.itemDesc = gContext.gameStrings.item_descriptions.get_string(relativeIndex);

    return srItem;
}

SISTERRAY_API void setSrItemData(SrConsumableData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    gContext.items.update_element(name, data.baseData);
    gContext.itemOnUseData.update_element(name, data.useData);

    ItemTypeData typeData = gContext.itemTypeData.get_element(name);
    auto relativeIndex = typeData.type_relative_id;
    gContext.gameStrings.item_names.update_resource(relativeIndex, EncodedString::from_unicode(data.itemName));
    gContext.gameStrings.item_descriptions.update_resource(relativeIndex, EncodedString::from_unicode(data.itemDesc));
}

SISTERRAY_API void addSrItem(SrConsumableData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    gContext.items.add_element(name, data.baseData);
    gContext.itemOnUseData.add_element(name, data.useData);
    gContext.itemTypeData.append_item(name, ITYPE_CONSUMABLE, ICONTYPE_CONSUMABLE);

    gContext.gameStrings.item_names.add_resource(EncodedString::from_unicode(data.itemName));
    gContext.gameStrings.item_descriptions.add_resource(EncodedString::from_unicode(data.itemDesc));
}

/*initialize on use data to match the vanilla game*/
void initOnUseItemDataRegistry() {
    for (u16 item_id = 0; item_id < 320; item_id++) {
        switch (item_id) {
        case 0:
            createOnUseItemData(100, 0, 0, 0, 0, 0, false, false, true);
            break;
        case 1:
            createOnUseItemData(500, 0, 0, 0, 0, 0, false, false, true);
            break;
        case 2:
            createOnUseItemData(10000, 0, 0, 0, 0, 0, false, false, true);
            break;
        case 3:
            createOnUseItemData(0, 100, 0, 0, 0, 0, false, false, true);
            break;
        case 4:
            createOnUseItemData(0, 10000, 0, 0, 0, 0, false, false, true);
            break;
        case 5:
            createOnUseItemData(10000, 10000, 0, 0, 0, 0, false, false, true);
            break;
        case 6:
            createOnUseItemData(10000, 10000, 0, 0, 0, 0, false, true, true);
            break;
        case 7:
            createOnUseItemData(0, 0, 0, 0, 25, 0, true, false, true);
            break;
        default:
            createOnUseItemData(0, 0, 0, 0, 0, 0, false, false, true);
        }
    }
}

void createOnUseItemData(u16 hp_heal_amount, u16 mp_heal_amount,
    u16 stat_to_boost, u16 character_restriction_mask, u8 hp_heal_percent,
    u8 mp_heal_percent, bool can_revive, bool target_all, bool requires_target) {

    ConsumableUseData itemData = {
        hp_heal_amount,
        mp_heal_amount,
        stat_to_boost,
        character_restriction_mask,
        hp_heal_percent,
        mp_heal_percent,
        can_revive,
        target_all,
        requires_target
    };

    gContext.itemOnUseData.add_resource(itemData);

}

SISTERRAY_API void initItems(SrKernelStream* stream) {
    gContext.items = SrItemRegistry(stream);
    gContext.itemTypeData.initialize_augmented_data((u8)0, gContext.items.resource_count());
    srLogWrite("kernel.bin: Loaded %lu items", (unsigned long)gContext.items.resource_count());
}

/*Utility check if an item is usable globally*/
bool canCharacterUseItem(u8 character_ID, u16 item_id) {
    u8 item_type = gContext.itemTypeData.get_resource(item_id).item_type;
    u16 relative_id = gContext.itemTypeData.get_resource(item_id).item_type;
    u16 restriction_mask;
    switch(item_type) {
    case 0: {
        restriction_mask = gContext.itemOnUseData.get_resource(item_id).characterRestrictionMask;
        if (restriction_mask & (1 << (character_ID))) {
            return true;
        }
        break;
    }
    case 1: {
        restriction_mask = gContext.weapons.get_resource(relative_id).equip_mask;
        if (restriction_mask & (1 << (character_ID))) {
            return true;
        }
        break;
    }
    case 2: {
        restriction_mask = gContext.armors.get_resource(relative_id).equip_mask;
        if (restriction_mask & (1 << (character_ID))) {
            return true;
        }
        break;
    }
    case 3: {
        restriction_mask = gContext.accessories.get_resource(relative_id).equip_mask;
        if (restriction_mask & (1 << (character_ID))) {
            return true;
        }
        break;
    }
    default:
        return false;
    }

    return false;
}
