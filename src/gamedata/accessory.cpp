#include <stdlib.h>
#include <string.h>
#include "../impl.h"
#include "accessory.h"

/*This is how the API for adding new resources will look on the code side
  These are very similar for the different types, and probably can be refactored in the future once
  More of the game is under our control into templates, but this is not practical atm
  Because we need to keep data in different places to continue using the patching strategy*/
SISTERRAY_API SrAccessoryData getSrAccessory(u16 modItemID, const char* modName) {
    SrAccessoryData srAccessory = SrAccessoryData();
    auto name = std::string(modName) + std::to_string(modItemID);
    srAccessory.baseData = gContext.accessories.get_element(name);
    srAccessory.auxData = gContext.auxAccessories.get_element(name);

    ItemTypeData typeData = gContext.itemTypeData.get_element(name);
    auto relativeIndex = typeData.type_relative_id;
    srAccessory.accessoryName = gContext.gameStrings.accessory_names.get_string(relativeIndex);
    srAccessory.accessoryDesc = gContext.gameStrings.accessory_descriptions.get_string(relativeIndex);

    return srAccessory;
}

SISTERRAY_API void setSrAccessoryData(SrAccessoryData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    gContext.accessories.update_element(name, data.baseData);
    gContext.auxAccessories.update_element(name, data.auxData);

    ItemTypeData typeData = gContext.itemTypeData.get_element(name);
    auto relativeIndex = typeData.type_relative_id;
    gContext.gameStrings.accessory_names.update_resource(relativeIndex, EncodedString::from_unicode(data.accessoryName));
    gContext.gameStrings.accessory_descriptions.update_resource(relativeIndex, EncodedString::from_unicode(data.accessoryDesc));
}

SISTERRAY_API void addSrAccessory(SrAccessoryData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    gContext.accessories.add_element(name, data.baseData);
    gContext.auxAccessories.add_element(name, data.auxData);
    gContext.itemTypeData.append_item(name, ITYPE_ACC, ICONTYPE_ACC);

    gContext.gameStrings.accessory_names.add_resource(EncodedString::from_unicode(data.accessoryName));
    gContext.gameStrings.accessory_descriptions.add_resource(EncodedString::from_unicode(data.accessoryDesc));
}

void initializeAuxAccessoryRegistry() {
    for (auto i = 0; i < KERNEL_ACCESSORY_COUNT; ++i) {
        auto name = std::string(BASE_PREFIX) + std::to_string(i);
        auto& kernelAccessory = gContext.accessories.get_element(name);

        ActorStatBoosts boosts = ActorStatBoosts();
        auto& stats = kernelAccessory.stats_to_boost;
        auto& amts = kernelAccessory.stat_boost_amounts;
        populatekernelStatBoosts(stats, amts, boosts, 2);
        AuxAccessoryData auxAccessory = { boosts };
        gContext.auxAccessories.add_element(name, auxAccessory);
    }
}

SISTERRAY_API void init_accessory(SrKernelStream* stream) {
    gContext.accessories = SrAccessoryRegistry(stream);
    gContext.auxAccessories = SrAuxAccessoryRegistry();
    initializeAuxAccessoryRegistry();
    gContext.itemTypeData.initialize_augmented_data((u8)3, gContext.accessories.resource_count());
    srLogWrite("kernel.bin: Loaded %lu accessories", (unsigned long)gContext.accessories.resource_count());
}
