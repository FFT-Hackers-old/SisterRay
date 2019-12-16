#include <stdlib.h>
#include <string.h>
#include "../impl.h"
#include "armor.h"

SISTERRAY_API SrArmorData getSrArmor(u16 modItemID, const char* modName) {
    SrArmorData srArmor = SrArmorData();
    auto name = std::string(modName) + std::to_string(modItemID);
    srArmor.baseData = gContext.armors.get_element(name);
    srArmor.auxData = gContext.auxArmors.get_element(name);

    ItemTypeData typeData = gContext.itemTypeData.get_element(name);
    auto relativeIndex = typeData.type_relative_id;
    srArmor.armorName = gContext.gameStrings.armor_names.get_string(relativeIndex);
    srArmor.armorDesc = gContext.gameStrings.armor_descriptions.get_string(relativeIndex);

    return srArmor;
}

SISTERRAY_API void setSrArmorData(SrArmorData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    gContext.armors.update_element(name, data.baseData);
    gContext.auxArmors.update_element(name, data.auxData);

    ItemTypeData typeData = gContext.itemTypeData.get_element(name);
    auto relativeIndex = typeData.type_relative_id;
    gContext.gameStrings.armor_names.update_resource(relativeIndex, EncodedString::from_unicode(data.armorName));
    gContext.gameStrings.armor_descriptions.update_resource(relativeIndex, EncodedString::from_unicode(data.armorDesc));
}

SISTERRAY_API void addSrArmor(SrArmorData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    gContext.armors.add_element(name, data.baseData);
    gContext.auxArmors.add_element(name, data.auxData);
    gContext.itemTypeData.append_item(name, ITYPE_ARMOR, ICONTYPE_ARMOR);

    gContext.gameStrings.armor_names.add_resource(EncodedString::from_unicode(data.armorName));
    gContext.gameStrings.armor_descriptions.add_resource(EncodedString::from_unicode(data.armorDesc));
}


void initializeAuxArmorRegistry() {
    for (auto i = 0; i < KERNEL_ARMOR_COUNT; ++i) {
        auto name = std::string(BASE_PREFIX) + std::to_string(i);
        auto& kernelArmor = gContext.armors.get_element(name);

        ActorStatBoosts boosts = ActorStatBoosts();
        auto& stats = kernelArmor.stats_to_boost;
        auto& amts = kernelArmor.stat_boost_amounts;
        populatekernelStatBoosts(stats, amts, boosts, 4);
        AuxArmorData auxArmor = { boosts };
        gContext.auxArmors.add_element(name, auxArmor);
    }
}


SISTERRAY_API void init_armor(SrKernelStream* stream) {
    gContext.armors = SrArmorRegistry(stream);
    gContext.auxArmors = SrAuxArmorRegistry();
    initializeAuxArmorRegistry();
    gContext.itemTypeData.initialize_augmented_data(ITYPE_ARMOR, gContext.armors.resource_count());
    srLogWrite("kernel.bin: Loaded %lu Armors", (unsigned long)gContext.armors.resource_count());
}
