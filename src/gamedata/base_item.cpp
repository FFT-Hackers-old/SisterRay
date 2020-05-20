#include "base_item.h"
#include "base_type_names.h"
#include "../impl.h"

std::string getItemTypeFromGearType(SrGameGearType gearType) {
    switch (gearType) {
    case SR_GEAR_WEAPON: {
        return ItemTypeNames::WEAPON_TYPE;
    }
    case SR_GEAR_ARMOR: {
        return ItemTypeNames::ARMOR_TYPE;
    }
    case SR_GEAR_ACCESSORY: {
        return ItemTypeNames::ACCESSORY_TYPE;
    }
    }
    return ItemTypeNames::ACCESSORY_TYPE;
}

void initItemTypes() {
    gContext.itemTypes = ItemTypeRegistry();
    ItemType consumable = {false, 0};
    gContext.itemTypes.addElement(ItemTypeNames::CONSUMABLE_TYPE, consumable);

    ItemType weapon = { true, 1 };
    gContext.itemTypes.addElement(ItemTypeNames::WEAPON_TYPE, weapon);

    ItemType armor = { true, 9 };
    gContext.itemTypes.addElement(ItemTypeNames::WEAPON_TYPE, armor);

    ItemType acc = { true, 10 };
    gContext.itemTypes.addElement(ItemTypeNames::WEAPON_TYPE, acc);
}

/*Initialize the item_type mapping*/
SISTERRAY_API void initBaseItems() {
    gContext.baseItems = BaseItemRegistry();
}

const char* getItemNameFromAbsoluteIdx(u16 absoluteIdx) {
    auto typeData = gContext.baseItems.getResource(absoluteIdx);
    auto type = typeData.itemType;
    auto relativeIdx = typeData.typeRelativeID;
    if (type == ItemTypeNames::CONSUMABLE_TYPE) {
        return gContext.items.getResource(relativeIdx).itemName.str();
    }
    if (type == ItemTypeNames::WEAPON_TYPE) {
        return gContext.weapons.getResource(relativeIdx).sharedBase.gearName.str();
    }
    if (type == ItemTypeNames::ARMOR_TYPE) {
        return gContext.armors.getResource(relativeIdx).sharedBase.gearName.str();
    }
    if (type == ItemTypeNames::ACCESSORY_TYPE) {
        return gContext.accessories.getResource(relativeIdx).sharedBase.gearName.str();
    }
    return nullptr;
}

const char* getItemDescFromAbsoluteIdx(u16 absoluteIdx) {
    auto typeData = gContext.baseItems.getResource(absoluteIdx);
    auto type = typeData.itemType;
    auto relativeIdx = typeData.typeRelativeID;
    if (type == ItemTypeNames::CONSUMABLE_TYPE) {
        return gContext.items.getResource(relativeIdx).itemDescription.str();
    }
    if (type == ItemTypeNames::WEAPON_TYPE) {
        return gContext.weapons.getResource(relativeIdx).sharedBase.gearDescription.str();
    }
    if (type == ItemTypeNames::ARMOR_TYPE) {
        return gContext.armors.getResource(relativeIdx).sharedBase.gearDescription.str();
    }
    if (type == ItemTypeNames::ACCESSORY_TYPE) {
        return gContext.accessories.getResource(relativeIdx).sharedBase.gearDescription.str();
    }
    return nullptr;
}

/*Called to initialize from kernel data the base types of items*/
void BaseItemRegistry::initializeAugmentedData(std::string itemType, u32 numberToInitialize) {
    u16 absoluteBase = resourceCount();
    u16 absoluteIndex;
    for (u16 relativeIndex = 0; relativeIndex < numberToInitialize; relativeIndex++) {
        auto iconType = getKernelIconType(itemType, relativeIndex);
        SrBaseItem baseItemData{ itemType, (u16)relativeIndex, iconType };
        absoluteIndex = absoluteBase + relativeIndex;

        auto name = std::string(BASE_PREFIX) + std::to_string(absoluteIndex);
        gContext.baseItems.appendItem(name, itemType, iconType);
        reverseIndexMaps[itemType].push_back(absoluteIndex);
    }
}

void BaseItemRegistry::appendItem(const std::string& name, std::string itemType, u8 iconType) {
    if (contains(name))
        return;

    auto& reverseRegistry = reverseIndexMaps[itemType];
    auto relativeItemID = reverseRegistry.size();
    SrBaseItem baseData = { itemType, relativeItemID, iconType };
    addElement(name, baseData);
}

/*Only used when loading from kernel to initialize the augmented data from kernel*/
u8 getKernelIconType(std::string itemType, u16 typeRelativeIndex) {
    if (itemType == ItemTypeNames::CONSUMABLE_TYPE) {
        return 0;
    }
    if (itemType == ItemTypeNames::WEAPON_TYPE) {
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
    }
    if (itemType == ItemTypeNames::ARMOR_TYPE) {
        return 10;
    }
    if (itemType == ItemTypeNames::ACCESSORY_TYPE) {
        return 11;
    }
    return 0;
}

u16 BaseItemRegistry::getAbsoluteID(std::string itemType, u8 relativeIndex) {
    return reverseIndexMaps[itemType].at(relativeIndex);
}

StatBoost createGearBoost(SrGameGearType gearType, u16 relativeGearIdx, bool isPercent, u16 amount, bool isNegative) {
    StatBoost statBoost = { isNegative, amount, isPercent };
    statBoost.tags.insert("GEAR");
    statBoost.tags.insert(assembleGDataKey(relativeGearIdx));
    switch (gearType) {
    case SR_GEAR_WEAPON: {
        statBoost.tags.insert("WEAPON");
        break;
    }
    case SR_GEAR_ARMOR: {
        statBoost.tags.insert("ARMOR");
        break;
    }
    case SR_GEAR_ACCESSORY: {
        statBoost.tags.insert("ACCESSORY");
        break;
    }
    }
    return statBoost;
}

void populatekernelStatBoosts(EquipmentStatBoosts& statBoosts, const u8* const stats, const u8* const amts, u8 count, u16 relativeGearIdx, SrGameGearType gearType) {
    for (u16 idx = 0; idx < count; idx++) {
        auto statType = stats[idx];
        if (statType == 0xFF)
            continue;

        auto statBoost = StatBoost();
        statBoost.amount = amts[idx];
        statBoost.tags.insert("GEAR");
        statBoost.tags.insert(assembleGDataKey(relativeGearIdx));
        switch (gearType) {
        case SR_GEAR_WEAPON: {
            statBoost.tags.insert("WEAPON");
            break;
        }
        case SR_GEAR_ARMOR: {
            statBoost.tags.insert("ARMOR");
            break;
        }
        case SR_GEAR_ACCESSORY: {
            statBoost.tags.insert("ACCESSORY");
            break;
        }
        }

        switch (statType) {
        case 0: {
            statBoosts["STR"].push_back(statBoost);
            break;
        }
        case 1: {
            statBoosts["VIT"].push_back(statBoost);
            break;
        }
        case 2: {
            statBoosts["MAG"].push_back(statBoost);
            break;
        }
        case 3: {
            statBoosts["SPR"].push_back(statBoost);
            break;
        }
        case 4: {
            statBoosts["DEX"].push_back(statBoost);
            break;
        }
        case 5: {
            statBoosts["LCK"].push_back(statBoost);
            break;
        }
        }
    }
}
