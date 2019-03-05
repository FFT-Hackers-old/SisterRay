#include "usable_item_handlers.h"
#include "items.h"
#include "impl.h"

srOnUseCallbackRegistry::srOnUseCallbackRegistry(): SrFunctionResourceRegistry<onUseCallback>() {
    add_function("heal_party_member", heal_handler);
    add_function("increment_stat", permanently_boost_stat);
    add_function("teach_limit", teach_limit_breaks);
    add_function("no_function", default_item_use);
}

srNoTargetCallbackRegistry::srNoTargetCallbackRegistry() : SrFunctionResourceRegistry<noTargetCallback>() {
    add_function("save_crystal_handler", save_crystal_on_use);
    add_function("no_function", default_no_target_use);
}

void initOnUseCallbackRegistry() {
    for (u16 item_id = 0; item_id < 320; item_id++) {
        switch (item_id) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7: {
            gContext.on_use_handlers.add_resource("heal_party_member");
            break;
        }
        default:
            gContext.on_use_handlers.add_resource("no_function");
        }
    }
}

void initNoTargetCallbackRegistry() {
    for (u16 item_id = 0; item_id < 320; item_id++) {
        switch (item_id) {
        case 98:
            gContext.on_use_handlers.add_resource("save_crystal_handler");
        default:
            gContext.on_use_handlers.add_resource("no_function");
        }
    }
}


// un-targetd handlers
bool default_no_target_use() {
    return false;
}

bool save_crystal_on_use() {
    playMenuSound(263);
    *byte_DC0C3C |= 1u;  //Save Crystal used?
    set_some_transition_data(5, 0);
    set_more_transition_data(0);
    save_crystal_handler();
    return true;
}


//targeted handlers
bool default_item_use(u16 party_member_index, u16 item_id, u16 inventory_index) {
    return false;
}

/*On use callback for performing HP healing on the menu */
bool heal_handler(u16 party_member_index, u16 item_id, u16 inventory_index) {
    bool use_successful = false;
    bool temp_bool;
    auto target_all = gContext.item_on_use_data.get_resource(item_id).target_all;

    if (target_all) {
        for (u16 member_to_heal = 0; member_to_heal < 3; ++member_to_heal) {
            temp_bool = heal_single_party_member(member_to_heal, item_id);
            if (!use_successful) {
                use_successful = temp_bool;
            }
        }
    }
    else {
        use_successful = heal_single_party_member(party_member_index, item_id);
    }
    play_success_or_failure_sound(use_successful, 263, 3);

    return use_successful;
}

bool heal_single_party_member(u16 party_member_index, u16 item_id) {
    bool heal_was_performed = false;
    auto is_hp_healable = check_target_hp_healable(party_member_index, item_id);
    auto is_mp_healable = check_target_mp_healable(party_member_index, item_id);
    auto item_heals_hp = (gContext.item_on_use_data.get_resource(item_id).hp_heal_amount || gContext.item_on_use_data.get_resource(item_id).hp_heal_percent);
    auto item_heals_mp = (gContext.item_on_use_data.get_resource(item_id).mp_heal_amount || gContext.item_on_use_data.get_resource(item_id).mp_heal_percent);

    if (item_heals_hp) {
        if (is_hp_healable) {
            u16 heal_amount = calculate_hp_heal_amount(party_member_index, item_id);
            heal_character_at_index(party_member_index, heal_amount);
            heal_was_performed = true;
        }
    }
    if (item_heals_mp) {
        if (is_mp_healable) {
            u16 heal_amount = calculate_mp_heal_amount(party_member_index, item_id);
            restore_party_member_mp(party_member_index, heal_amount);
            heal_was_performed = true;
        }
    }
    return heal_was_performed;
}

u16 calculate_hp_heal_amount(u16 party_member_index, u16 item_id) {
    if (gContext.item_on_use_data.get_resource(item_id).hp_heal_amount) {
        return gContext.item_on_use_data.get_resource(item_id).hp_heal_amount;
    }
    else if (gContext.item_on_use_data.get_resource(item_id).hp_heal_percent){
        u8 heal_divisor = gContext.item_on_use_data.get_resource(item_id).hp_heal_percent;
        return (((activePartyStructArray)[party_member_index].maxHP / heal_divisor) * 100);
    }
    return 0;
}

u16 calculate_mp_heal_amount(u16 party_member_index, u16 item_id) {
    if (gContext.item_on_use_data.get_resource(item_id).mp_heal_amount) {
        return gContext.item_on_use_data.get_resource(item_id).mp_heal_amount;
    }
    else if (gContext.item_on_use_data.get_resource(item_id).mp_heal_percent) {
        u8 heal_divisor = gContext.item_on_use_data.get_resource(item_id).mp_heal_percent;
        return (((activePartyStructArray)[party_member_index].maxMP / heal_divisor) * 100);
    }
    return 0;
}

bool check_target_hp_healable(u16 target, u16 item_id) {
    bool is_healable = check_character_hp_full(target);

    if (gContext.item_on_use_data.get_resource(item_id).can_revive) {
        return is_healable;
    }

    bool is_alive = ((bool)((activePartyStructArray)[target].currentHP));
    return (is_healable && is_alive);
}

bool check_target_mp_healable(u16 target, u16 item_id) {
    bool is_healable = check_character_mp_full(target);

    if (gContext.item_on_use_data.get_resource(item_id).can_revive) {
        return is_healable;
    }

    bool is_alive = ((bool)((activePartyStructArray)[target].currentHP));
    return (is_healable && is_alive);
}

bool check_character_hp_full(u16 party_member_index) {
     return (!check_member_HP_full(party_member_index));
}

bool check_character_mp_full(u16 party_member_index) {
    return (!check_member_MP_full(party_member_index));
}

bool check_character_dead(u16 party_member_index) {
    return (bool)activePartyStructArray[party_member_index].currentHP;
}

void play_success_or_failure_sound(bool did_succeed, i32 success_sound_id, i32 failure_sound_id) {
    if (did_succeed) {
        playMenuSound(success_sound_id);
    }
    else {
        playMenuSound(failure_sound_id);
    }
}

/*Perhaps redesign to make it possible to boost multiple stats, lower stats, or boost by variable amounts*/
bool permanently_boost_stat(u16 party_member_index, u16 item_id, u16 inventory_index) {
    u8 character_ID = (CURRENT_PARTY_MEMBER_ARRAY)[party_member_index];
    u8 stat_to_boost = gContext.item_on_use_data.get_resource(item_id).stat_to_boost;
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
        playMenuSound(263);
        recalculate_party_stats(party_member_index); //This should be rewritten to allow stat penalties
        sub_5CB2CC(party_member_index);
    }
    else {
        playMenuSound(3);
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
            playMenuSound(384);
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
            playMenuSound(3);
        }
    }
    else {
        auto cannot_learn_string = gContext.game_strings.character_specific_strings[character_ID].get_string(2);
        display_menu_string(cannot_learn_string); //get can't learn limit text
        sub_6C497C((int)byte_DD18C8, 7);
        playMenuSound(3);
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
