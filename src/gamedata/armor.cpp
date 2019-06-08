#include <stdlib.h>
#include <string.h>
#include "../impl.h"
#include "armor.h"

SISTERRAY_API SrArmorData getArmor(u16 modItemID, const char* modName) {
    SrArmorData srArmor = SrArmorData();
    auto name = std::string(modName) + std::to_string(modItemID);
    srArmor.baseData = gContext.armors.get_element(name);
    srArmor.auxData = gContext.auxArmors.get_element(name);

    ItemTypeData typeData = gContext.itemTypeData.get_element(name);
    auto relativeIndex = typeData.type_relative_id;
    srArmor.armorName = gContext.gameStrings.armor_names.get_string(relativeIndex).str();
    srArmor.armorDesc = gContext.gameStrings.armor_descriptions.get_string(relativeIndex).str();

    return srArmor
}

SISTERRAY_API void setArmorData(SrArmorData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    gContext.armors.update_element(name, data.baseData);
    gContext.auxArmors.update_element(name, data.auxData);

    ItemTypeData typeData = gContext.itemTypeData.get_element(name);
    auto relativeIndex = typeData.type_relative_id;
    gContext.gameStrings.armor_names.update_resource(relativeIndex, EncodedString::from_unicode(data.armorName));
    gContext.gameStrings.armor_descriptions.update_resource(relativeIndex, EncodedString::from_unicode(data.armorDesc));
}

SISTERRAY_API void addArmor(SrArmorData data, u16 modItemID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modItemID);
    gContext.armors.add_element(name, data.baseData);
    gContext.auxArmors.add_element(name, data.auxData);
    gContext.itemTypeData.add_element(name, ITYPE_ARMOR, ICONTYPE_ARMOR);

    gContext.gameStrings.armor_names.add_resource(EncodedString::from_unicode(data.armorName));
    gContext.gameStrings.armor_descriptions.add_resource(EncodedString::from_unicode(data.armorDesc));
}

static const u32 kPatchStructBase[] = {
    0x005cf92b, 0x006cb977, 0x007ba088
};

static const u32 kPatchDefense[] = {
    0x006c51ca, 0x007079aa, 0x00707bde, 0x00707f6e, 0x0070818e,0x0071a016, 0x0071a11c, 0x0071a441, 0x0071a547
};

static const u32 kPatchMagicDefense[] = {
    0x00707a02, 0x00707c00, 0x00707fc6, 0x007081b0
};

static const u32 kPatchEvade[] = {
    0x00704fba, 0x00705395, 0x007079d6, 0x00707bef, 0x00707f9a, 0x0070819f
};

static const u32 kPatchMagicEvade[] = {
    0x00705012, 0x007053ed, 0x00707a2e, 0x00707c11, 0x00707ff2, 0x007081c1
};

static const u32 kPatchMateriaSlot1[] = {
    0x006cb921, 0x00700249, 0x007005c8, 0x007047da, 0x00704b68, 0x0070601a,
    0x007061e4, 0x00706999, 0x00706b58, 0x0070d154, 0x0070d3d2, 0x0070f196,
    0x0070fa7a, 0x007100cd, 0x0071020f, 0x00710331, 0x0071057b, 0x007105c5,
    0x0071069d, 0x00710741, 0x00717e81, 0x0071b4b2, 0x0071cbaa
};

static const u32 kPatchRestrictMask[] = {
    0x00433344, 0x005D162C, 0x006C5132
};

static const u32 kPatchEquipMask[] = {
    0x00708548
};

static void patch_armor(void) {
    srPatchAddresses((void**)kPatchStructBase, ARRAY_SIZE(kPatchStructBase), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, unknown));
    srPatchAddresses((void**)kPatchDefense, ARRAY_SIZE(kPatchDefense), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, defense));
    srPatchAddresses((void**)kPatchMagicDefense, ARRAY_SIZE(kPatchMagicDefense), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, magic_defense));
    srPatchAddresses((void**)kPatchEvade, ARRAY_SIZE(kPatchEvade), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, evade));
    srPatchAddresses((void**)kPatchMagicEvade, ARRAY_SIZE(kPatchMagicEvade), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, magic_evade));
    srPatchAddresses((void**)kPatchMateriaSlot1, ARRAY_SIZE(kPatchMateriaSlot1), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, materia_slots));
    srPatchAddresses((void**)kPatchRestrictMask, ARRAY_SIZE(kPatchRestrictMask), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, restriction_mask));
    srPatchAddresses((void**)kPatchEquipMask, ARRAY_SIZE(kPatchEquipMask), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, equip_mask));
}

void initializeAuxArmorRegistry() {
    for (auto i = 0; i < KERNEL_ARMOR_COUNT, ++i) {
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
    gContext.itemTypeData.initialize_augmented_data((ITYPE_ARMOR, gContext.armors.resource_count());
    patch_armor();
    srLogWrite("kernel.bin: Loaded %lu Armors", (unsigned long)gContext.armors.resource_count());
}
