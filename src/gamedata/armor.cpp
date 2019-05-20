#include <stdlib.h>
#include <string.h>
#include "../impl.h"
#include "armor.h"


SISTERRAY_API ArmorData getArmor(u16 itemID) {
    return gContext.armors.get_resource(itemID);
}

SISTERRAY_API void setArmorData(ArmorData attackData, u16 itemID) {
    gContext.armors.update_resource(itemID, attackData);
}

SISTERRAY_API void addArmor(ArmorData attackData, char* name) {
    gContext.armors.add_element(std::string(name), attackData);
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

static void patch_armor(void)
{
    srPatchAddresses((void**)kPatchStructBase, ARRAY_SIZE(kPatchStructBase), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, unknown));
    srPatchAddresses((void**)kPatchDefense, ARRAY_SIZE(kPatchDefense), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, defense));
    srPatchAddresses((void**)kPatchMagicDefense, ARRAY_SIZE(kPatchMagicDefense), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, magic_defense));
    srPatchAddresses((void**)kPatchEvade, ARRAY_SIZE(kPatchEvade), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, evade));
    srPatchAddresses((void**)kPatchMagicEvade, ARRAY_SIZE(kPatchMagicEvade), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, magic_evade));
    srPatchAddresses((void**)kPatchMateriaSlot1, ARRAY_SIZE(kPatchMateriaSlot1), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, materia_slots));
    srPatchAddresses((void**)kPatchRestrictMask, ARRAY_SIZE(kPatchRestrictMask), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, restriction_mask));
    srPatchAddresses((void**)kPatchEquipMask, ARRAY_SIZE(kPatchEquipMask), ARMOR_DATA_PTR, gContext.armors.get_data(), offsetof(ArmorData, equip_mask));
}

SISTERRAY_API void init_armor(SrKernelStream* stream) {
    gContext.armors = SrArmorRegistry(stream);
    gContext.itemTypeData.initialize_augmented_data((u8)2, gContext.armors.resource_count());
    patch_armor();
    srLogWrite("kernel.bin: Loaded %lu Armors", (unsigned long)gContext.armors.resource_count());
}
