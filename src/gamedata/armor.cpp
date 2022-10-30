#include <stdlib.h>
#include <string.h>
#include "../impl.h"
#include "armor.h"
#include "stat_names.h"
#include "base_type_names.h"
#include "battle_stats.h"

SrArmorRegistry::SrArmorRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<SrArmor, std::string>() {
    size_t read_size;
    GameArmor baseArmor;

    auto idx = 0;
    while (1) {
        auto cmdIdx = 0;
        read_size = srKernelStreamRead(stream, &baseArmor, sizeof(baseArmor));
        if (read_size != sizeof(baseArmor))
            break;
        SrArmor armor;
        armor.sharedBase.gearName = gContext.gameStrings.armor_names.get_string(idx);
        armor.sharedBase.gearDescription = gContext.gameStrings.armor_descriptions.get_string(idx);
        armor.gameArmor = baseArmor;
        armor.sharedBase.equipEffects[StatNames::EVADE].push_back(createGearBoost(SR_GEAR_ARMOR, idx, false, baseArmor.evade, false));
        armor.sharedBase.equipEffects[StatNames::MEVADE].push_back(createGearBoost(SR_GEAR_ARMOR, idx, false, baseArmor.magicEvade, false));
        armor.sharedBase.equipEffects[StatNames::ARMOR_DEFENSE].push_back(createGearBoost(SR_GEAR_ARMOR, idx, false, baseArmor.defense, false));
        armor.sharedBase.equipEffects[StatNames::ARMOR_MDEFENSE].push_back(createGearBoost(SR_GEAR_ARMOR, idx, false, baseArmor.magicDefense, false));
        populatekernelStatBoosts(armor.sharedBase.equipEffects, armor.gameArmor.stats_to_boost, armor.gameArmor.stat_boost_amounts, 4, idx, SR_GEAR_ARMOR);
        initializeArmorStats(armor);
        initializeArmorElements(armor, idx);
        addElement(assembleGDataKey(idx), armor);
        ++idx;
    }
}

void initializeArmorStats(SrArmor& armor) {
    auto& gameArmor = armor.gameArmor;
    armor.sharedBase.stats[StatNames::ARMOR_DEFENSE].statValue = gameArmor.defense;
    armor.sharedBase.stats[StatNames::ARMOR_MDEFENSE].statValue = gameArmor.magicDefense;
    armor.sharedBase.stats[StatNames::EVADE].statValue = gameArmor.evade;
    armor.sharedBase.stats[StatNames::MEVADE].statValue = gameArmor.magicEvade;
}

void initializeArmorElements(SrArmor& armor, u16 relativeID) {
    auto& equipEffects = armor.sharedBase.equipEffects;
    for (auto elementIdx = 0; elementIdx < 16; elementIdx++) {
        if (!(armor.gameArmor.elemental_defense_mask & (1 << elementIdx))) {
            continue;
        }
        auto elementDamageType = armor.gameArmor.elementDamageType;
        auto elementName = getElementIDFromIndex(elementIdx);
        auto element = gContext.elements.getElement(elementName);
        if (elementDamageType = 0) {
            StatBoost statBoost{0, 125, false};
            statBoost.tags.insert("GEAR");
            statBoost.tags.insert("ARMOR");
            statBoost.tags.insert(assembleGDataKey(relativeID));
            equipEffects[element.resName].push_back(statBoost);
        }
        if (elementDamageType = 1) {
            StatBoost statBoost{ 0, 100, false };
            statBoost.tags.insert("GEAR");
            statBoost.tags.insert("ARMOR");
            statBoost.tags.insert(assembleGDataKey(relativeID));
            equipEffects[element.resName].push_back(statBoost);
        }
        if (elementDamageType = 2) {
            StatBoost statBoost{ 0, 50, false };
            statBoost.tags.insert("GEAR");
            statBoost.tags.insert("ARMOR");
            statBoost.tags.insert(assembleGDataKey(relativeID));
            equipEffects[element.resName].push_back(statBoost);
        }
    }
}

SISTERRAY_API SrArmorData getSrArmor(u16 modItemID, const char* modName) {
    SrArmorData apiArmor = SrArmorData();
    auto name = std::string(modName) + std::to_string(modItemID);
    auto& armor = gContext.armors.getElement(name);
    apiArmor.baseData = armor.gameArmor;
    apiArmor.armorName = armor.sharedBase.gearName.str();
    apiArmor.armorDesc = armor.sharedBase.gearDescription.str();
    return apiArmor;
}

SISTERRAY_API void setSrArmorData(SrArmorData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    auto srArmor = SrArmor();
    srArmor.gameArmor = data.baseData;
    srArmor.sharedBase.gearName = EncodedString::from_unicode(data.armorName);
    srArmor.sharedBase.gearDescription = EncodedString::from_unicode(data.armorDesc);
    gContext.armors.updateElement(name, srArmor);
}

SISTERRAY_API void addSrArmor(SrArmorData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    auto srArmor = SrArmor();
    srArmor.gameArmor = data.baseData;
    srArmor.sharedBase.gearName = EncodedString::from_unicode(data.armorName);
    srArmor.sharedBase.gearDescription = EncodedString::from_unicode(data.armorDesc);
    gContext.armors.addElement(name, srArmor);
    gContext.baseItems.appendItem(name, ItemTypeNames::ARMOR_TYPE, ICONTYPE_ARMOR);
}


SISTERRAY_API void initArmor(SrKernelStream* stream) {
    gContext.armors = SrArmorRegistry(stream);
    gContext.baseItems.initializeAugmentedData(ItemTypeNames::ARMOR_TYPE, gContext.armors.resourceCount());
    srLogWrite("kernel.bin: Loaded %lu Armors", (unsigned long)gContext.armors.resourceCount());
}

void finalizeArmors() {
    finalizeRegistry<SrArmor, InitArmorEvent, SrArmorRegistry>(gContext.armors, INIT_ARMOR);
}
