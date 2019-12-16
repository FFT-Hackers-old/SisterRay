#include "base_item.h"
#include "../impl.h"

/*Initialize the item_type mapping*/
SISTERRAY_API void initItemTypeData() {
    gContext.itemTypeData = SrItemTypeRegistry();
}

const char* getItemNameFromAbsoluteIdx(u16 absoluteIdx) {
    auto typeData = gContext.itemTypeData.get_resource(absoluteIdx);
    auto type = typeData.item_type;
    auto relativeIdx = typeData.type_relative_id;
    switch (type) {
    case 0: {
        return gContext.gameStrings.item_names.get_string(relativeIdx);
        break;
    }
    case 1: {
        return gContext.gameStrings.weapon_names.get_string(relativeIdx);
        break;
    }
    case 2: {
        return gContext.gameStrings.armor_names.get_string(relativeIdx);
        break;
    }
    case 3: {
        return gContext.gameStrings.accessory_names.get_string(relativeIdx);
        break;
    }
    default: {
        return nullptr;
    }
    }
}

const char* getItemDescFromAbsoluteIdx(u16 absoluteIdx) {
    auto typeData = gContext.itemTypeData.get_resource(absoluteIdx);
    auto type = typeData.item_type;
    auto relativeIdx = typeData.type_relative_id;
    switch (type) {
    case 0: {
        return gContext.gameStrings.item_descriptions.get_string(relativeIdx);
        break;
    }
    case 1: {
        return gContext.gameStrings.weapon_descriptions.get_string(relativeIdx);
        break;
    }
    case 2: {
        return gContext.gameStrings.armor_descriptions.get_string(relativeIdx);
        break;
    }
    case 3: {
        return gContext.gameStrings.accessory_descriptions.get_string(relativeIdx);
        break;
    }
    default: {
        return nullptr;
    }
    }
}

/*Called to initialize from kernel data the base types of items*/
void SrItemTypeRegistry::initialize_augmented_data(u8 itemType, u32 numberToInitialize) {
    u16 absoluteBase = resource_count();
    u16 absoluteIndex;
    for (u16 relativeIndex = 0; relativeIndex < numberToInitialize; relativeIndex++) {
        auto iconType = getKernelIconType(itemType, relativeIndex);
        ItemTypeData baseItemData{ itemType, (u16)relativeIndex, iconType };
        absoluteIndex = absoluteBase + relativeIndex;

        auto name = std::string(BASE_PREFIX) + std::to_string(absoluteIndex);
        gContext.itemTypeData.append_item(name, itemType, iconType);

        switch (itemType) {
            case 0: {
                reverseItemRegistry.push_back(absoluteIndex);
                break;
            }
            case 1: {
                reverseWeaponRegistry.push_back(absoluteIndex);
                break;
            }
            case 2: {
                reverseArmorRegistry.push_back(absoluteIndex);
                break;
            }
            case 3: {
                reverseAccessoryRegistry.push_back(absoluteIndex);
                break;
            }
            default: {
                return;
            }
        }
    }
}

void SrItemTypeRegistry::append_item(const std::string& name, u8 itemType, u8 iconType) {
    if (contains(name))
        return;

    std::vector<i16>& reverseRegistry = reverseItemRegistry;
    switch (itemType) {
        case 0: {
            reverseRegistry = reverseItemRegistry;
            break;
        }
        case 1: {
            reverseRegistry = reverseWeaponRegistry;
            break;
        }
        case 2: {
            reverseRegistry = reverseArmorRegistry;
            break;
        }
        case 3: {
            reverseRegistry = reverseAccessoryRegistry;
            break;
        }
        default: {
            return;
        }
    }
    auto relative_item_id = reverseRegistry.size();
    ItemTypeData baseData = { itemType, relative_item_id, iconType };
    add_element(name, baseData);
}

/*Only used when loading from kernel to initialize the augmented data from kernel*/
u8 getKernelIconType(u8 itemType, u16 typeRelativeIndex) {
    switch (itemType) {
        case 0:
            return 0;
        case 1: {
            if (typeRelativeIndex < 16)
                return 1;
            if (typeRelativeIndex < 32)
                return 3;
            if (typeRelativeIndex < 48)
                return 2;
            if (typeRelativeIndex < 62)
                return 5;
            if (typeRelativeIndex < 73)
                return 4;
            if (typeRelativeIndex < 87)
                return 9;
            if (typeRelativeIndex < 101)
                return 6;
            if (typeRelativeIndex < 114)
                return 7;
            return 8;
            break;
        }
        case 2:
            return 10;
        case 3:
            return 11;
        default:
            return 0;
    }
}

u16 SrItemTypeRegistry::get_absolute_id(u8 itemType, u8 relativeIndex) {
    switch (itemType) {
        case 0:
            return reverseItemRegistry.at(relativeIndex);
        case 1:
            return reverseWeaponRegistry.at(relativeIndex);
        case 2:
            return reverseArmorRegistry.at(relativeIndex);
        case 3:
            return reverseAccessoryRegistry.at(relativeIndex);
    }
}

void populatekernelStatBoosts(const u8* const stats, const u8* const amts, ActorStatBoosts& boosts, u8 count) {
    for (u16 idx = 0; idx < count; idx++) {
        auto statType = stats[idx];
        if (statType == 0xFF)
            continue;
        auto& boost = getStatBoostFromID(boosts, statType);
        boost.amount = amts[idx];
    }
}
