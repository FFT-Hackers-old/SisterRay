#include "usable_item_handlers.h"
#include "items.h"
#include "impl.h"

srOnUseCallbackRegistry::srOnUseCallbackRegistry() {
    add_function("heal_party_member", heal_handler);
}

void srOnUseCallbackRegistry::add_function(std::string name, onUseCallback callback) {
    handler_names[name] = callback;
}

onUseCallback srOnUseCallbackRegistry::get_handler(u16 item_id) {
    auto& name = get_resource(item_id);
    return handler_names[name];
}
/*Registers callback to a specific ID*/


/*On use callback for performing HP healing on the menu */
bool heal_handler(u16 party_member_index, u16 item_id, u16 inventory_index) {
    bool did_something = false;
    bool temp_bool;
    auto target_all = gContext.item_on_use_data.get_resource(item_id).target_all;

    if (target_all) {
        for (u16 member_to_heal = 0; member_to_heal < 3; ++member_to_heal) {
            temp_bool = heal_single_party_member(member_to_heal);
            if (!did_something) {
                did_something = temp_bool;
            }
        }
    }
    else  {
        did_something = heal_single_party_member(party_member_index, item_id);
    }
    play_success_or_failure_sound(did_something, 263, 3);

    return did_something
}

bool heal_single_party_member(u16 party_member_index, u16 item_id) {
    auto is_hp_healable = check_target_hp_healable(party_member_index, item_id);
    auto is_mp_healable = check_target_mp_healable(party_member_index, item_id);

    if (gContext.item_on_use_data[item_id].hp_heal_amount) {
        if (is_hp_healable) {
            u16 heal_amount = gContext.item_on_use_data[item_id].hp_heal_amount;
            heal_character_at_index(party_member_index, heal_amount);
            did_something = true;
        }
    }
    if (gContext.item_on_use_data[item_id].mp_heal_amount) {
        if (is_mp_healable) {
            u16 heal_amount = gContext.item_on_use_data[item_id].mp_heal_amount;
            restore_party_member_mp(party_member_index, heal_amount);
            did_something = true;
        }
    }
    return did_something;
}

bool check_target_hp_healable(u16 target, u16 item_id) {
    bool can_target_dead = false;
    if (gContext.item_on_use_data.get_resource(item_id).can_revive) {
        can_target_dead = true;
    }

    bool is_healable = check_character_hp_full(target);

    return (is_healable && can_target_dead);
}

bool check_target_mp_healable(u16 target, u16 item_id) {
    bool can_target_dead = false;
    if (gContext.item_on_use_data.get_resource(item_id).can_revive) {
        can_target_dead = true;
    }

    bool is_healable = check_character_mp_full(target);

    return (is_healable && can_target_dead);
}

bool check_character_hp_full(u16 party_member_index) {
     return (!check_member_HP_full(party_member_index));
}

bool check_character_mp_full(u16 party_member_index) {
    return (!check_member_MP_full(party_member_index));
}

bool check_character_dead(u16 party_member_index) {
    return (bool)activePartyStructArray[party_member_index].currentHP
}

void play_success_or_failure_sound(bool did_succeed, i32 success_sound_id, i32 failure_sound_id) {
    if (did_succeed) {
        play_menu_sound(success_sound_id);
    }
    else {
        play_menu_sound(failure_sound_id);
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
        recalculate_party_stats(party_member_index); //This should be rewritten to allow stat penalties
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
        if (knows_all_prereq_limits(character_ID)) { //Check if requisite limits are learned
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
