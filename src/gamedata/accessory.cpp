#include <stdlib.h>
#include <string.h>
#include "../impl.h"
#include "accessory.h"

SrAccessoryRegistry::SrAccessoryRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<SrAccessory, std::string>() {
    size_t read_size;
    AccessoryData baseAccessory;

    auto idx = 0;
    while (1) {
        auto cmdIdx = 0;
        read_size = srKernelStreamRead(stream, &baseAccessory, sizeof(baseAccessory));
        if (read_size != sizeof(baseAccessory))
            break;
        SrAccessory accessory;
        accessory.accessoryName = gContext.gameStrings.accessory_names.get_string(idx);
        accessory.accessoryDescription = gContext.gameStrings.accessory_descriptions.get_string(idx);
        accessory.gameAccessory = baseAccessory;
        initializeAccessoryElements(accessory, idx);
        populatekernelStatBoosts(accessory.equipEffects, accessory.gameAccessory.stats_to_boost, accessory.gameAccessory.stat_boost_amounts, 2, idx, SR_GEAR_ACCESSORY);
        addElement(assembleGDataKey(idx), accessory);
        ++idx;
    }
}

void initializeAccessoryElements(SrAccessory& accessory, u16 relativeID) {
    auto& equipEffects = accessory.equipEffects;
    for (auto elementIdx = 0; elementIdx < 16; elementIdx++) {
        if (!(accessory.gameAccessory.elements_mask & (1 << elementIdx))) {
            continue;
        }
        auto elementDamageType = accessory.gameAccessory.elemental_interaction;
        auto elementName = getElementIDFromIndex(elementIdx);
        auto element = gContext.elements.getElement(elementName);
        if (elementDamageType = 0) {
            StatBoost statBoost{ 0, 125, false };
            statBoost.tags.insert("GEAR");
            statBoost.tags.insert("ACCESSORY");
            statBoost.tags.insert(assembleGDataKey(relativeID));
            equipEffects[element.resName].push_back(statBoost);
        }
        if (elementDamageType = 1) {
            StatBoost statBoost{ 0, 100, false };
            statBoost.tags.insert("GEAR");
            statBoost.tags.insert("ACCESSORY");
            statBoost.tags.insert(assembleGDataKey(relativeID));
            equipEffects[element.resName].push_back(statBoost);
        }
        if (elementDamageType = 2) {
            StatBoost statBoost{ 0, 50, false };
            statBoost.tags.insert("GEAR");
            statBoost.tags.insert("ACCESSORY");
            statBoost.tags.insert(assembleGDataKey(relativeID));
            equipEffects[element.resName].push_back(statBoost);
        }
    }
}

SISTERRAY_API SrAccessoryData getSrAccessory(u16 modItemID, const char* modName) {
    SrAccessoryData apiAccessory = SrAccessoryData();
    auto name = std::string(modName) + std::to_string(modItemID);
    auto& accessory = gContext.accessories.getElement(name);
    apiAccessory.baseData = accessory.gameAccessory;
    apiAccessory.auxData = accessory.auxData;
    apiAccessory.accessoryName = accessory.accessoryName.str();
    apiAccessory.accessoryDesc= accessory.accessoryDescription.str();
    return apiAccessory;
}

SISTERRAY_API void setSrAccessoryData(SrAccessoryData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    auto srAccessory = SrAccessory();
    srAccessory.gameAccessory = data.baseData;
    srAccessory.auxData = data.auxData;
    srAccessory.accessoryName = EncodedString::from_unicode(data.accessoryName);
    srAccessory.accessoryDescription = EncodedString::from_unicode(data.accessoryDesc);
    gContext.accessories.updateElement(name, srAccessory);
}

SISTERRAY_API void addSrAccessory(SrAccessoryData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    auto srAccessory = SrAccessory();
    srAccessory.gameAccessory = data.baseData;
    srAccessory.auxData = data.auxData;
    srAccessory.accessoryName = EncodedString::from_unicode(data.accessoryName);
    srAccessory.accessoryDescription = EncodedString::from_unicode(data.accessoryDesc);
    gContext.accessories.addElement(name, srAccessory);
    gContext.itemTypeData.appendItem(name, ITYPE_ACC, ICONTYPE_ACC);
}

SISTERRAY_API void initAccessories(SrKernelStream* stream) {
    gContext.accessories = SrAccessoryRegistry(stream);
    gContext.itemTypeData.initializeAugmentedData((u8)3, gContext.accessories.resourceCount());
    srLogWrite("kernel.bin: Loaded %lu accessories", (unsigned long)gContext.accessories.resourceCount());
}
