#include "base_item.h"
#include "../impl.h"

/*Initialize the item_type mapping*/
SISTERRAY_API void initItemTypeData() {
    gContext.itemTypeData = SrItemTypeRegistry();
}

/*Called to initialize from kernel data the base types of items*/
void SrItemTypeRegistry::initialize_augmented_data(u8 itemType, u16 numberToInitialize) {
    u16 absoluteBase = resource_count() - 1;
    u16 absoluteIndex;
    for (u16 relativeIndex = 0; relativeIndex < numberToInitialize; relativeIndex++) {
        auto iconType = getKernelIconType(itemType, relativeIndex);
        ItemTypeData baseItemData{ itemType, (u16)relativeIndex, iconType };
        absoluteIndex = absoluteBase + relativeIndex;

        auto name = std::string(BASE_PREFIX) + std::to_string(absoluteIndex);
        gContext.itemTypeData.add_element(name, baseItemData);

        switch (itemType) {
            case 0:
                reverseItemRegistry.push_back(absoluteIndex);
            case 1:
                reverseWeaponRegistry.push_back(absoluteIndex);
            case 2:
                reverseArmorRegistry.push_back(absoluteIndex);
            case 3:
                reverseAccessoryRegistry.push_back(absoluteIndex);
        }
    }
}

void SrItemTypeRegistry::add_element(const std::string& name, u8 itemType, u8 iconType) {
    if (contains(name))
        return;

    std::vector<i16> reverseRegistry;
    switch (itemType) {
        case 0:
            reverseRegistry = reverseItemRegistry.at(relativeIndex);
        case 1:
            reverseRegistry = reverseWeaponRegistry.at(relativeIndex);
        case 2:
            reverseRegistry = reverseArmorRegistry.at(relativeIndex);
        case 3:
            reverseRegistry = reverseAccessoryRegistry.at(relativeIndex);
        default: {
            return;
        }
    }
    auto relative_item_id = reverseRegistry.size() - 1;
    ItemTypeData baseData = { itemType, relative_item_id, iconType };
    add_resource(baseData);
    u32 index = resource_count() - 1;
    named_registry[name] = index;
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

void populatekernelStatBoosts(u8* stats, u8* amts, ActorStatBoosts& boosts, u8 count) {
    for (auto idx = 0; idx < count; idx++) {
        auto statType = stats[idx];
        auto& boost = getStatBoostFromID(boosts, statType);
        boost.amount = amts[idx];
    }
}
