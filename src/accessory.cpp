#include <stdlib.h>
#include <string.h>
#include "impl.h"
#include "kernel_utils.h"
#include "accessory.h"

static void InitializeAugmentedAccessoryData() {
    for (int i = 0; i < gContext.accessories.count; i++) {
        ItemTypeData* object = allocKernelObject<ItemTypeData, SrItemTypeData>(&gContext.item_type_data);
        object->item_type = 3;
        object->type_relative_id = i; //relative ideas 0-31 for the accessories
    }
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

static void PatchAccessories(void)
{
    srPatchAddresses((void**)kPatchStructBase, ARRAY_SIZE(kPatchStructBase), ACCESSORY_DATA_PTR, gContext.accessories.data, offsetof(AccessoryData, statsToBoost));
    srPatchAddresses((void**)kSpecialEffect, ARRAY_SIZE(kSpecialEffect), ACCESSORY_DATA_PTR, gContext.accessories.data, offsetof(AccessoryData, specialEffect));
    srPatchAddresses((void**)kPatchRestrictMask, ARRAY_SIZE(kPatchRestrictMask), ACCESSORY_DATA_PTR, gContext.accessories.data, offsetof(AccessoryData, restrictionMask));
    srPatchAddresses((void**)kPatchEquipMask, ARRAY_SIZE(kPatchEquipMask), ACCESSORY_DATA_PTR, gContext.accessories.data, offsetof(AccessoryData, equipMask));
}

SISTERRAY_API void InitAccessory(SrKernelStream* stream)
{
    SrAccessoryRegistry* registry = &gContext.accessories;
    initRegistry<AccessoryData>(
        stream,
        registry,
        allocKernelObject<AccessoryData, SrAccessoryRegistry>,
        initObjectRegistry<AccessoryData, SrAccessoryRegistry>);
    InitializeAugmentedAccessoryData();
    PatchAccessories();
    srLogWrite("kernel.bin: Loaded %lu accessories", (unsigned long)gContext.accessories.count);
}
