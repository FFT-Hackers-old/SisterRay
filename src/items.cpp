#include "items.h"
#include "impl.h"

static const u32 kPatchStructBase[] = {
    0x005c9b7b, 0x007ba080
};

static const u32 kPatchRestrictMask[] = {
    0x004332db, 0x005d15b6, 0x006c50f1,
};

static const u32 kPatchTargetFlags[] = {
    0x004332c8, 0x005d15cb
};

static void patch_items(void)
{
    srPatchAddresses((void**)kPatchStructBase, ARRAY_SIZE(kPatchStructBase), ITEM_DATA_PTR, gContext.items.get_data(), offsetof(ItemData, unknown));;
    srPatchAddresses((void**)kPatchRestrictMask, ARRAY_SIZE(kPatchRestrictMask), ITEM_DATA_PTR, gContext.items.get_data(), offsetof(ItemData, restriction_mask));
    srPatchAddresses((void**)kPatchTargetFlags, ARRAY_SIZE(kPatchTargetFlags), ITEM_DATA_PTR, gContext.items.get_data(), offsetof(ItemData, target_flags));
}


SISTERRAY_API void init_item(SrKernelStream* stream)
{
    gContext.items = SrItemRegistry(stream);
    gContext.item_type_data.initialize_augmented_data((u8)0, gContext.items.resource_count());
    patch_items();
    srLogWrite("kernel.bin: Loaded %lu items", (unsigned long)gContext.items.resource_count());
}

/*Utility check if an item is usable globally*/
bool character_can_use_item(u8 character_ID, u16 item_id) {
    u8 item_type = gContext.item_type_data.get_resource(item_id).item_type;
    u16 relative_id = gContext.item_type_data.get_resource(item_id).item_type;
    u16 restriction_mask;
    switch(item_type) {
    case 0: {
        restriction_mask = gContext.item_on_use_data[relative_id].character_restriction_mask;
        if (restriction_mask & (1 << (character_ID))) {
            return true;
        }
        break;
    }
    case 1: {
        restriction_mask = gContext.weapons.get_resource(relative_id).equip_mask;
        if (restriction_mask & (1 << (character_ID))) {
            return true;
        }
        break;
    }
    case 2: {
        restriction_mask = gContext.armors.get_resource(relative_id).equip_mask;
        if (restriction_mask & (1 << (character_ID))) {
            return true;
        }
        break;
    }
    case 3: {
        restriction_mask = gContext.accessories.get_resource(relative_id).equip_mask;
        if (restriction_mask & (1 << (character_ID))) {
            return true;
        }
        break;
    }
    default:
        return false;
    }

    return false;
}
