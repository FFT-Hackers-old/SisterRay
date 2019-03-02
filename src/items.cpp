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

static void patchItems(void)
{
    srPatchAddresses((void**)kPatchStructBase, ARRAY_SIZE(kPatchStructBase), ITEM_DATA_PTR, gContext.items.get_data(), offsetof(ItemData, unknown));;
    srPatchAddresses((void**)kPatchRestrictMask, ARRAY_SIZE(kPatchRestrictMask), ITEM_DATA_PTR, gContext.items.get_data(), offsetof(ItemData, restriction_mask));
    srPatchAddresses((void**)kPatchTargetFlags, ARRAY_SIZE(kPatchTargetFlags), ITEM_DATA_PTR, gContext.items.get_data(), offsetof(ItemData, target_flags));
}

/*initialize on use data to match the vanilla game*/
void initOnUseDataRegistry() {
    for (u16 item_id = 0; item_id < 320; item_id++) {
        switch (item_id) {
        case 0:
            createOnUseItemData(100, 0, 0, 0, 0, 0, false, false);
            break;
        case 1:
            createOnUseItemData(500, 0, 0, 0, 0, 0, false, false);
            break;
        case 2:
            createOnUseItemData(10000, 0, 0, 0, 0, 0, false, false);
            break;
        case 3:
            createOnUseItemData(0, 100, 0, 0, 0, 0, false, false);
            break;
        case 4:
            createOnUseItemData(0, 10000, 0, 0, 0, 0, false, false);
            break;
        case 5:
            createOnUseItemData(10000, 10000, 0, 0, 0, 0, false, false);
            break;
        case 6:
            createOnUseItemData(10000, 10000, 0, 0, 0, 0, false, true);
            break;
        case 7:
            createOnUseItemData(0, 0, 0, 0, 25, 0, true, false);
            break;
        default:
            createOnUseItemData(0, 0, 0, 0, 0, 0, false, false);
        }
    }
}

void createOnUseItemData(u16 hp_heal_amount, u16 mp_heal_amount, u16 stat_to_boost,
    u16 character_restriction_mask, u8 hp_heal_percent,
    u8 mp_heal_percent, bool can_revive, bool target_all) {

    onUseItemData itemData = {
        hp_heal_amount,
        mp_heal_amount,
        stat_to_boost,
        character_restriction_mask,
        hp_heal_percent,
        mp_heal_percent,
        can_revive,
        target_all
    };

    gContext.item_on_use_data.add_resource(itemData);

}

SISTERRAY_API void initItems(SrKernelStream* stream)
{
    gContext.items = SrItemRegistry(stream);
    gContext.item_type_data.initialize_augmented_data((u8)0, gContext.items.resource_count());
    patchItems();
    srLogWrite("kernel.bin: Loaded %lu items", (unsigned long)gContext.items.resource_count());
}

/*Utility check if an item is usable globally*/
bool character_can_use_item(u8 character_ID, u16 item_id) {
    u8 item_type = gContext.item_type_data.get_resource(item_id).item_type;
    u16 relative_id = gContext.item_type_data.get_resource(item_id).item_type;
    u16 restriction_mask;
    switch(item_type) {
    case 0: {
        restriction_mask = gContext.item_on_use_data.get_resource(item_id).character_restriction_mask;
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
