#include "inventory_utils.h"
#include "../impl.h"

/*Utility check if an item is usable on the menu*/
bool usableInInventoryMenu(u16 item_id) {
    u16 restriction_mask = get_restriction_mask(item_id);
    if (restriction_mask & 4) {
        return true;
    }
    return false;
}

u16 get_restriction_mask(u16 item_id) {
    u8 item_type = gContext.itemTypeData.get_resource(item_id).item_type;
    u16 relative_id = gContext.itemTypeData.get_resource(item_id).type_relative_id;
    u16 restriction_mask;
    switch (item_type) {
    case 0: {
        restriction_mask = gContext.items.get_resource(relative_id).restriction_mask;
        break;
    }
    case 1: {
        restriction_mask = gContext.weapons.get_resource(relative_id).restriction_mask;
        break;
    }
    case 2: {
        restriction_mask = gContext.armors.get_resource(relative_id).restriction_mask;
        break;
    }
    case 3: {
        restriction_mask = gContext.accessories.get_resource(relative_id).restriction_mask;
        break;
    }
    default:
        restriction_mask = 0x00;
    }
    return restriction_mask;
}

u8 get_target_flags(u16 item_id) {
    u8 item_type = gContext.itemTypeData.get_resource(item_id).item_type;
    u16 relative_id = gContext.itemTypeData.get_resource(item_id).type_relative_id;
    u8 target_flags;
    switch (item_type) {
    case 0: {
        target_flags = gContext.items.get_resource(relative_id).target_flags;
        break;
    }
    case 1: {
        target_flags = gContext.weapons.get_resource(relative_id).target_flags;
        break;
    }
    case 2: {
        target_flags = 3;
        break;
    }
    case 3: {
        target_flags = 3;
        break;
    }
    default:
        target_flags = 0x00;
    }
    return target_flags;
}
