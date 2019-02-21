#include "items.h"
#include "impl.h"

/*Patch references to items to reference to reference the new gContext objects*/
static void patch_items() {
    u8 a = 1;
}

SISTERRAY_API void init_item(SrKernelStream* stream)
{
    gContext.items = SrItemRegistry(stream);
    initialize_augmented_data(0);
    patch_items();
    srLogWrite("kernel.bin: Loaded %lu items", (unsigned long)gContext.items.resource_count());
}

bool character_can_use_item(u8 character_ID, u16 item_id) {
    u16 item_restriction_mask = gContext.item_on_use_data[item_id].character_restriction_mask;
    if (item_restriction_mask & (1 << (character_ID))) {
        return true;
    }
    return false;
}
