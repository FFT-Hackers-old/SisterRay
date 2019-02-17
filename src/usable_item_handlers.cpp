#include "usable_item_handlers.h"
#include "impl.h"

/*On use callback for performing HP healing on the menu */
bool heal_party_member(u16 party_member_index, u16 item_id, u16 inventory_index) {
    if (!check_member_HP_full(party_member_index) && activePartyStructArray[party_member_index].currentHP) {
        play_menu_sound(263);
        u16 heal_amount = gContext.item_on_use_data[item_id].hp_heal_amount;
        heal_character_at_index(party_member_index, heal_amount);
        return true;
    }
    else {
        play_menu_sound(3);
        return false;
    }
};

/*On use callback for performing MP healing on the menu*/
bool heal_party_member_mp(u16 party_member_index, u16 item_id, u16 inventory_index) {
    if (!check_member_MP_full(party_member_index) && activePartyStructArray[party_member_index].currentHP) {
        play_menu_sound(263);
        u16 heal_amount = gContext.item_on_use_data[item_id].mp_heal_amount;
        restore_party_member_mp(party_member_index, heal amount);
        return true;
    }
    else
    {
        play_menu_sound(3);
        return false;
    }
}

/*might the above methods a more generic "heal" method and just data drive it*/
bool heal_party_member_hp_and_mp(u16 party_member_index, u16 item_id, u16 inventory_index) {
    if ((check_member_MP_full(party_member_index) && check_member_HP_full(party_member_index)) || activePartyStructArray[party_member_index].currentHP) {
        play_menu_sound(263);
        u16 heal_amount = gContext.item_on_use_data[item_id].hp_heal_amount;
        restore_party_member_mp(party_member_index, heal amount);
        heal_amount = gContext.item_on_use_data[item_id].mp_heal_amount;
        restore_party_member_mp(party_member_index, heal amount);
        return true;
    }
    else
    {
        play_menu_sound(3);
        return false;
    }
};

/*Can probably be merged with the above handlers and have the 'target-all' flag set */
bool megalixir_handler(u16 party_member_index, u16 item_id, u16 inventory_index) {
    u16 current_party_member = 0;
    bool healable_exists = false;
    while (current_party_member < 3) {
        if (CURRENT_PARTY_MEMBER_ARRAY[current_party_member] != 255 && (!check_member_HP_full(current_party_member)
            || !check_member_MP_full(current_party_member))) {
            healable_exists = true;
        }
        ++current_party_member;
    }
    if (healable_exists) {
        for (u16 member_to_heal = 0; member_to_heal < 3; ++member_to_heal) {
            if (activePartyStructArray[party_member_index].currentHP && CURRENT_PARTY_MEMBER_ARRAY[member_to_heal] != 0xFF) {
                heal_character_at_index(member_to_heal, 10000);
                restore_party_member_mp(member_to_heal, 10000);
            }
        }
        play_menu_sound(263);
        return true;
    }
    else {
        play_menu_sound(3);
        return false;
    }
};
