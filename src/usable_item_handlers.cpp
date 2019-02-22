#include "usable_item_handlers.h"
#include "items.h"
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
}

/*On use callback for performing MP healing on the menu*/
bool heal_party_member_mp(u16 party_member_index, u16 item_id, u16 inventory_index) {
    if (!check_member_MP_full(party_member_index) && activePartyStructArray[party_member_index].currentHP) {
        play_menu_sound(263);
        u16 heal_amount = gContext.item_on_use_data[item_id].mp_heal_amount;
        restore_party_member_mp(party_member_index, heal_amount);
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
        restore_party_member_mp(party_member_index, heal_amount);
        heal_amount = gContext.item_on_use_data[item_id].mp_heal_amount;
        restore_party_member_mp(party_member_index, heal_amount);
        return true;
    }
    else
    {
        play_menu_sound(3);
        return false;
    }
}

/*Can probably be merged with the above handlers and have the 'target-all' flag set */
bool megalixir_handler(u16 party_member_index, u16 item_id, u16 inventory_index) {
    u16 current_party_member = 0;
    bool healable_exists = false;
    while (current_party_member < 3) {
        if ((CURRENT_PARTY_MEMBER_ARRAY)[current_party_member] != 255 && (!check_member_HP_full(current_party_member)
            || !check_member_MP_full(current_party_member))) {
            healable_exists = true;
        }
        ++current_party_member;
    }
    if (healable_exists) {
        for (u16 member_to_heal = 0; member_to_heal < 3; ++member_to_heal) {
            if (activePartyStructArray[party_member_index].currentHP && (CURRENT_PARTY_MEMBER_ARRAY)[member_to_heal] != 0xFF) {
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
}

/*Merge into one handler with the healing handler and introduce "can revive" and "target_all" data flags*/
bool revive_handler(u16 party_member_index, u16 item_id, u16 inventory_index) {
    if (activePartyStructArray[party_member_index].currentHP) {
        play_menu_sound(3);
        return false;
    }
    else {
        u16 heal_ratio = gContext.item_on_use_data[item_id].hp_heal_ratio;
        heal_character_at_index(party_member_index, (activePartyStructArray[party_member_index].maxHP/heal_ratio));
        play_menu_sound(263);
        return true;
    }
}

/*Perhaps redesign to make it possible to boost multiple stats, lower stats, or boost by variable amounts*/
bool permanently_boost_stat(u16 party_member_index, u16 item_id, u16 inventory_index) {
    u8 character_ID = (CURRENT_PARTY_MEMBER_ARRAY)[party_member_index];
    u8 stat_to_boost = gContext.item_on_use_data[item_id].stat_to_boost;
    bool stat_boosted = false;
    switch (stat_to_boost) {
        case 0: {
            if (characterRecordArray[character_ID].strength < 255) {
                ++characterRecordArray[character_ID].bonus_strength;
                stat_boosted = true;
            }
            break;
        }
        case 1: {
            if (characterRecordArray[character_ID].vitality < 255) {
                ++characterRecordArray[character_ID].bonus_vitality;
                stat_boosted = true;
            }
            break;
        }
        case 2: {
            if (characterRecordArray[character_ID].magic < 255) {
                ++characterRecordArray[character_ID].bonus_magic;
                stat_boosted = true;
            }
            break;
        }
        case 3: {
            if (characterRecordArray[character_ID].spirit < 255) {
                ++characterRecordArray[character_ID].bonus_spirit;
                stat_boosted = true;
            }
            break;
        }
        case 4: {
            if (characterRecordArray[character_ID].dexterity < 255) {
                ++characterRecordArray[character_ID].bonus_dexterity;
                stat_boosted = true;
            }
            break;
        }
        case 5: {
            if (characterRecordArray[character_ID].luck < 255) {
                ++characterRecordArray[character_ID].bonus_luck;
                stat_boosted = true;
            }
            break;
        }
    }
    if (stat_boosted) {
        play_menu_sound(263);
        addEquipmentStatBoosts(party_member_index);
        sub_5CB2CC(party_member_index);
    }
    else {
        play_menu_sound(3);
    }

    return stat_boosted;
}

/*Introduce item restriction masks making items usable by some characters
  Limit Breaks will be the first test, their usable flags will be checked.
  Performing the same checks during battle will allow some "consumables" to have
  per character restrictions*/
bool teach_limit_breaks(u16 party_member_index, u16 item_id, u16 inventory_index) {
    u8 character_ID = (u8)(CURRENT_PARTY_MEMBER_ARRAY)[party_member_index];
    bool item_usable = character_can_use_item(character_ID, item_id); //If the character can't use the item, give the old "nothing to do with me message"
    bool limit_taught = false;
    auto registry = gContext.game_strings.character_specific_strings[character_ID];
    if (item_usable) {
        if (sub_715026(character_ID)) { //Check if requisite limits are learned
            play_menu_sound(384);
            characterRecordArray[character_ID].learned_limits = (characterRecordArray[character_ID].learned_limits | 0x0200);
            auto limit_learned_string = gContext.game_strings.character_specific_strings[character_ID].get_string(0); //String 0 is a characters "limit learned" string
            display_menu_string(limit_learned_string); //Get limit learned text, need to build out custom string storage
            sub_6C497C((int)byte_DD18C8, 7); //The game does this casting of a pointer as an arg... is ugly
            limit_taught = true;
        }
        else
        {
            auto not_ready_string = gContext.game_strings.character_specific_strings[character_ID].get_string(1);
            display_menu_string(not_ready_string); //get can't learn limit yet text. Can fetch from our own managed text array
            sub_6C497C((int)byte_DD18C8, 7);
            play_menu_sound(3);
        }
    }
    else {
        auto cannot_learn_string = gContext.game_strings.character_specific_strings[character_ID].get_string(2);
        display_menu_string(cannot_learn_string); //get can't learn limit text
        sub_6C497C((int)byte_DD18C8, 7);
        play_menu_sound(3);
    }
    return limit_taught;
}

    /* switch (item_ID) {
case 13:
    if (characterStatusFlags[132 * partyMemberID] & 0x20)
    {
        play_menu_sound(3);
    }
    else
    {
        if (characterStatusFlags[132 * partyMemberID] & 0x10)
            v20 = characterStatusFlags[132 * partyMemberID] & 0xEF;
        else
            v20 = characterStatusFlags[132 * partyMemberID] | 0x20;
        characterStatusFlags[132 * partyMemberID] = v20;
        play_menu_sound(263);
    }
    break;
case 14:
    if (characterStatusFlags[132 * partyMemberID] & 0x20)
    {
        characterStatusFlags[132 * partyMemberID] &= 0xDFu;
    }
    else
    {
        if (characterStatusFlags[132 * partyMemberID] & 0x10)
        {
            play_menu_sound(3);
            return;
        }
        characterStatusFlags[132 * partyMemberID] |= 0x10u;
    }
    play_menu_sound(263);
    break;
case 70:
    v55 = 0;
    v49 = 0;
    while (v55 < 3)
    {
        if (partyMember1[v55] != 255 && (!check_member_HP_full(v55) || !check_member_MP_full(v55)))
            v49 = 1;
        ++v55;
    }
    if (v49)
    {
        for (i4 = 0; i4 < 3; ++i4)
        {
            if (persistent_character_HP[544 * i4] && partyMember1[i4] != 255)
            {
                heal_character_at_index(i4, 10000);
                restore_party_member_mp(i4, 10000);
            }
        }
        play_menu_sound(263);
    }
    else
    {
        play_menu_sound(3);
    }
    break;
default:
    return;*/
