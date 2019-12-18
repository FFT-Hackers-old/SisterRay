#include <stdlib.h>
#include <string.h>
#include "../impl.h"
#include "armor.h"

SISTERRAY_API SrArmorData getSrArmor(u16 modItemID, const char* modName) {
    SrArmorData srArmor = SrArmorData();
    auto name = std::string(modName) + std::to_string(modItemID);
    srArmor.baseData = gContext.armors.getElement(name);
    srArmor.auxData = gContext.auxArmors.getElement(name);

    ItemTypeData typeData = gContext.itemTypeData.getElement(name);
    auto relativeIndex = typeData.typeRelativeID;
    srArmor.armorName = gContext.gameStrings.armor_names.get_string(relativeIndex);
    srArmor.armorDesc = gContext.gameStrings.armor_descriptions.get_string(relativeIndex);

    return srArmor;
}

SISTERRAY_API void setSrArmorData(SrArmorData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    gContext.armors.updateElement(name, data.baseData);
    gContext.auxArmors.updateElement(name, data.auxData);

    ItemTypeData typeData = gContext.itemTypeData.getElement(name);
    auto relativeIndex = typeData.typeRelativeID;
    gContext.gameStrings.armor_names.updateResource(relativeIndex, EncodedString::from_unicode(data.armorName));
    gContext.gameStrings.armor_descriptions.updateResource(relativeIndex, EncodedString::from_unicode(data.armorDesc));
}

SISTERRAY_API void addSrArmor(SrArmorData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    gContext.armors.addElement(name, data.baseData);
    gContext.auxArmors.addElement(name, data.auxData);
    gContext.itemTypeData.appendItem(name, ITYPE_ARMOR, ICONTYPE_ARMOR);

    gContext.gameStrings.armor_names.addResource(EncodedString::from_unicode(data.armorName));
    gContext.gameStrings.armor_descriptions.addResource(EncodedString::from_unicode(data.armorDesc));
}


void initializeAuxArmorRegistry() {
    for (auto i = 0; i < KERNEL_ARMOR_COUNT; ++i) {
        auto name = std::string(BASE_PREFIX) + std::to_string(i);
        auto& kernelArmor = gContext.armors.getElement(name);

        ActorStatBoosts boosts = ActorStatBoosts();
        auto& stats = kernelArmor.stats_to_boost;
        auto& amts = kernelArmor.stat_boost_amounts;
        populatekernelStatBoosts(stats, amts, boosts, 4);
        AuxArmorData auxArmor = { boosts };
        gContext.auxArmors.addElement(name, auxArmor);
    }
}


SISTERRAY_API void init_armor(SrKernelStream* stream) {
    gContext.armors = SrArmorRegistry(stream);
    gContext.auxArmors = SrAuxArmorRegistry();
    initializeAuxArmorRegistry();
    gContext.itemTypeData.initializeAugmentedData(ITYPE_ARMOR, gContext.armors.resourceCount());
    srLogWrite("kernel.bin: Loaded %lu Armors", (unsigned long)gContext.armors.resourceCount());
}
