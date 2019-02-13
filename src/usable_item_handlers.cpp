#include "usable_item_handlers.h"

/*On use callback for performing HP healing on the menu */
bool heal_party_member(u16 party_member_index, u16 item_id, u16 inventory_index) {
    if (!checkHPFull(party_member_index) && persistent_character_HP[544 * party_member_index]) {
        play_menu_sound(263);
        u16 heal_amount = gContext.item_on_use_data[item_id].heal_amount;
        heal_character_at_index(party_member_index, heal_amount);
        return true;
    }
    else {
        play_menu_sound(3);
        return false;
    }
};

/*On use callback for performing MP healing on the menu*/
bool restore_party_member_mp(u16 party_member_index, u16 item_id, u16 inventory_index) {
    if (!checkMPFull(party_member_index) && persistent_character_HP[544 * party_member_index]) {
        play_menu_sound(263);
        u16 heal_amount = gContext.item_on_use_data[item_id].heal_amount;
        restore_party_member_mp(party_member_index, heal amount);
        return true;
    }
    else
    {
        play_menu_sound(3);
        return false;
    }
}
