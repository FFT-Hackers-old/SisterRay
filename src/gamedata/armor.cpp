#include <stdlib.h>
#include <string.h>
#include "../impl.h"
#include "armor.h"
#include "../party/stat_names.h"

SrArmorRegistry::SrArmorRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<SrArmor, std::string>() {
    size_t read_size;
    ArmorData baseArmor;

    auto idx = 0;
    while (1) {
        auto cmdIdx = 0;
        read_size = srKernelStreamRead(stream, &baseArmor, sizeof(baseArmor));
        if (read_size != sizeof(baseArmor))
            break;
        SrArmor armor;
        armor.armorName = gContext.gameStrings.armor_names.get_string(idx);
        armor.armorDescription = gContext.gameStrings.armor_descriptions.get_string(idx);
        armor.gameArmor = baseArmor;
        armor.equipEffects[StatNames::EVADE].push_back(createGearBoost(SR_GEAR_ARMOR, idx, false, baseArmor.evade, false));
        armor.equipEffects[StatNames::MEVADE].push_back(createGearBoost(SR_GEAR_ARMOR, idx, false, baseArmor.magic_evade, false));
        armor.equipEffects[StatNames::ARMOR_DEFENSE].push_back(createGearBoost(SR_GEAR_ARMOR, idx, false, baseArmor.defense, false));
        armor.equipEffects[StatNames::ARMOR_MDEFENSE].push_back(createGearBoost(SR_GEAR_ARMOR, idx, false, baseArmor.magic_defense, false));
        populatekernelStatBoosts(armor.equipEffects, armor.gameArmor.stats_to_boost, armor.gameArmor.stat_boost_amounts, 4, idx, SR_GEAR_ARMOR);
        addElement(assembleGDataKey(idx), armor);
        ++idx;
    }
}

SISTERRAY_API SrArmorData getSrArmor(u16 modItemID, const char* modName) {
    SrArmorData apiArmor = SrArmorData();
    auto name = std::string(modName) + std::to_string(modItemID);
    auto& armor = gContext.armors.getElement(name);
    apiArmor.baseData = armor.gameArmor;
    apiArmor.auxData = armor.auxData;
    apiArmor.armorName = armor.armorName.str();
    apiArmor.armorDesc = armor.armorDescription.str();
    return apiArmor;
}

SISTERRAY_API void setSrArmorData(SrArmorData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    auto srArmor = SrArmor();
    srArmor.gameArmor = data.baseData;
    srArmor.auxData = data.auxData;
    srArmor.armorName = EncodedString::from_unicode(data.armorName);
    srArmor.armorDescription = EncodedString::from_unicode(data.armorDesc);
    gContext.armors.updateElement(name, srArmor);
}

SISTERRAY_API void addSrArmor(SrArmorData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    auto srArmor = SrArmor();
    srArmor.gameArmor = data.baseData;
    srArmor.auxData = data.auxData;
    srArmor.armorName = EncodedString::from_unicode(data.armorName);
    srArmor.armorDescription = EncodedString::from_unicode(data.armorDesc);
    gContext.armors.addElement(name, srArmor);
    gContext.itemTypeData.appendItem(name, ITYPE_ARMOR, ICONTYPE_ARMOR);
}


SISTERRAY_API void initArmor(SrKernelStream* stream) {
    gContext.armors = SrArmorRegistry(stream);
    gContext.itemTypeData.initializeAugmentedData(ITYPE_ARMOR, gContext.armors.resourceCount());
    srLogWrite("kernel.bin: Loaded %lu Armors", (unsigned long)gContext.armors.resourceCount());
}
