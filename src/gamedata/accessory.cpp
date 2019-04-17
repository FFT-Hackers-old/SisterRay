#include <stdlib.h>
#include <string.h>
#include "../impl.h"
#include "accessory.h"

SISTERRAY_API AccessoryData getAccessory(u16 itemID) {
    return gContext.accessories.get_resource(itemID);
}

SISTERRAY_API void setAccessoryData(AccessoryData data, u16 itemID) {
    gContext.accessories.update_resource(itemID, data);
}

SISTERRAY_API void addAccessory(AccessoryData data, char* name) {
    gContext.accessories.add_element(std::string(name), data);
}

static const u32 kPatchStructBase[] = {
    0x005d0317, 0x006cb987, 0x007ba08c
};

static const u32 kSpecialEffect[] = {
    0x005D0317
};

static const u32 kPatchRestrictMask[] = {
    0x0043336A, 0x005D1659, 0x006C514B
};

static const u32 kPatchEquipMask[] = {
    0x007085FF
};

static void patch_accessories(void)
{
    srPatchAddresses((void**)kPatchStructBase, ARRAY_SIZE(kPatchStructBase), ACCESSORY_DATA_PTR, gContext.accessories.get_data(), offsetof(AccessoryData, stats_to_boost));
    srPatchAddresses((void**)kSpecialEffect, ARRAY_SIZE(kSpecialEffect), ACCESSORY_DATA_PTR, gContext.accessories.get_data(), offsetof(AccessoryData, special_effect));
    srPatchAddresses((void**)kPatchRestrictMask, ARRAY_SIZE(kPatchRestrictMask), ACCESSORY_DATA_PTR, gContext.accessories.get_data(), offsetof(AccessoryData, restriction_mask));
    srPatchAddresses((void**)kPatchEquipMask, ARRAY_SIZE(kPatchEquipMask), ACCESSORY_DATA_PTR, gContext.accessories.get_data(), offsetof(AccessoryData, equip_mask));
}

SISTERRAY_API void init_accessory(SrKernelStream* stream)
{
    gContext.accessories = SrAccessoryRegistry(stream);
    /*calling this method adds entries into the relative-entry look up for all accessories*/
    gContext.itemTypeData.initialize_augmented_data((u8)3, gContext.accessories.resource_count());
    patch_accessories();
    srLogWrite("kernel.bin: Loaded %lu accessories", (unsigned long)gContext.accessories.resource_count());
}
