#include "usable_item_handlers.h"

void heal_party_member(u8 party_member_index, u32 heal_amount, u16 inventory_index) {
    if (!checkHPFull(party_member_index) && saveMapCharacterHP[544 * party_member_index])
        play_menu_sound(263);
    heal_character_at_index(party_member_index, heal_amount);
    handle_decrement_inventory(inventory_index, 1);
    if (gContext.inventory.data[inventory_index].item_id == 0xFFFF)// If the Inventory Entry is -1, i.e it has been used up
        *INVENTORY_MENU_STATE = 1;
}
    else {
    play_menu_sound(3);
    }
    break;
};
