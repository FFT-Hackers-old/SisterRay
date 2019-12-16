#include "usable_item_handlers.h"
#include "gamedata/items.h"
#include "impl.h"

srOnUseCallbackRegistry::srOnUseCallbackRegistry(): SrIndexedCallbackRegistry<onUseCallback>() {
    add_function("heal_party_member", handleHeal);
    add_function("increment_stat", handleBoostStat);
    add_function("teach_limit", handleTeachLimits);
    add_function("no_function", defaultUseItem);
}

srNoTargetCallbackRegistry::srNoTargetCallbackRegistry() : SrIndexedCallbackRegistry<noTargetCallback>() {
    add_function("save_crystal_handler", useSaveCrystal);
    add_function("no_function", defaultUseNoTarget);
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
            gContext.onUseHandlers.add_resource("heal_party_member");
            break;
        }
        default:
            gContext.onUseHandlers.add_resource("no_function");
        }
    }
}

void initNoTargetCallbackRegistry() {
    for (u16 item_id = 0; item_id < 320; item_id++) {
        switch (item_id) {
        case 98:
            gContext.onUseHandlers.add_resource("save_crystal_handler");
        default:
            gContext.onUseHandlers.add_resource("no_function");
        }
    }
}


// un-targetd handlers
bool defaultUseNoTarget() {
    return false;
}

bool useSaveCrystal() {
    playMenuSound(263);
    *byte_DC0C3C |= 1u;  //Save Crystal used?
    set_some_transition_data(5, 0);
    set_more_transition_data(0);
    save_crystal_handler();
    return true;
}


//targeted handlers
bool defaultUseItem(u8 partyIdx, u16 itemID, u16 inventoryIdx) {
    return false;
}

/*On use callback for performing HP healing on the menu */
bool handleHeal(u8 partyIdx, u16 itemID, u16 inventoryIdx) {
    bool use_successful = false;
    bool temp_bool;
    auto target_all = gContext.itemOnUseData.get_resource(itemID).target_all;

    if (target_all) {
        for (u8 member_to_heal = 0; member_to_heal < 3; ++member_to_heal) {
            temp_bool = healPartyMember(member_to_heal, itemID);
            if (!use_successful) {
                use_successful = temp_bool;
            }
        }
    }
    else {
        use_successful = healPartyMember(partyIdx, itemID);
    }
    playUseSound(use_successful, 263, 3);

    return use_successful;
}

bool healPartyMember(u8 partyIdx, u16 itemID) {
    bool heal_was_performed = false;
    auto is_hp_healable = checkIsHPHealable(partyIdx, itemID);
    auto is_mp_healable = checkIsMPHealable(partyIdx, itemID);
    auto item_heals_hp = (gContext.itemOnUseData.get_resource(itemID).hp_heal_amount || gContext.itemOnUseData.get_resource(itemID).hp_heal_percent);
    auto item_heals_mp = (gContext.itemOnUseData.get_resource(itemID).mp_heal_amount || gContext.itemOnUseData.get_resource(itemID).mp_heal_percent);

    if (item_heals_hp) {
        if (is_hp_healable) {
            u16 heal_amount = calculateHPHealAmount(partyIdx, itemID);
            heal_character_at_index(partyIdx, heal_amount);
            heal_was_performed = true;
        }
    }
    if (item_heals_mp) {
        if (is_mp_healable) {
            u16 heal_amount = calculateMPHealAmount(partyIdx, itemID);
            restore_party_member_mp(partyIdx, heal_amount);
            heal_was_performed = true;
        }
    }
    return heal_was_performed;
}

u16 calculateHPHealAmount(u8 partyIdx, u16 item_id) {
    if (gContext.itemOnUseData.get_resource(item_id).hp_heal_amount) {
        return gContext.itemOnUseData.get_resource(item_id).hp_heal_amount;
    }
    else if (gContext.itemOnUseData.get_resource(item_id).hp_heal_percent){
        u8 heal_divisor = gContext.itemOnUseData.get_resource(item_id).hp_heal_percent;
        return (((PARTY_STRUCT_ARRAY)[partyIdx].maxHP / heal_divisor) * 100);
    }
    return 0;
}

u16 calculateMPHealAmount(u8 partyIdx, u16 item_id) {
    if (gContext.itemOnUseData.get_resource(item_id).mp_heal_amount) {
        return gContext.itemOnUseData.get_resource(item_id).mp_heal_amount;
    }
    else if (gContext.itemOnUseData.get_resource(item_id).mp_heal_percent) {
        u8 heal_divisor = gContext.itemOnUseData.get_resource(item_id).mp_heal_percent;
        return (((PARTY_STRUCT_ARRAY)[partyIdx].maxMP / heal_divisor) * 100);
    }
    return 0;
}

bool checkIsHPHealable(u8 target, u16 item_id) {
    bool is_healable = checkActorHPFull(target);

    if (gContext.itemOnUseData.get_resource(item_id).can_revive) {
        return is_healable;
    }

    bool is_alive = ((bool)((PARTY_STRUCT_ARRAY)[target].currentHP));
    return (is_healable && is_alive);
}

bool checkIsMPHealable(u8 target, u16 item_id) {
    bool is_healable = checkActorMPFull(target);

    if (gContext.itemOnUseData.get_resource(item_id).can_revive) {
        return is_healable;
    }

    bool is_alive = ((bool)((PARTY_STRUCT_ARRAY)[target].currentHP));
    return (is_healable && is_alive);
}

bool checkActorHPFull(u8 party_member_index) {
     return (!check_member_HP_full(party_member_index));
}

bool checkActorMPFull(u8 party_member_index) {
    return (!check_member_MP_full(party_member_index));
}

bool check_character_dead(u8 party_member_index) {
    return (bool)PARTY_STRUCT_ARRAY[party_member_index].currentHP;
}

void playUseSound(bool did_succeed, i32 success_sound_id, i32 failure_sound_id) {
    if (did_succeed) {
        playMenuSound(success_sound_id);
    }
    else {
        playMenuSound(failure_sound_id);
    }
}

/*Perhaps redesign to make it possible to boost multiple stats, lower stats, or boost by variable amounts*/
bool handleBoostStat(u8 partyIdx, u16 itemID, u16 inventoryIdx) {
    u8 character_ID = (CURRENT_PARTY_MEMBER_ARRAY)[partyIdx];
    u16 stat_to_boost = gContext.itemOnUseData.get_resource(itemID).stat_to_boost;
    bool stat_boosted = false;
    switch (stat_to_boost) {
        case 0: {
            if (CHARACTER_RECORD_ARRAY[character_ID].strength < 255) {
                ++CHARACTER_RECORD_ARRAY[character_ID].bonus_strength;
                stat_boosted = true;
            }
            break;
        }
        case 1: {
            if (CHARACTER_RECORD_ARRAY[character_ID].vitality < 255) {
                ++CHARACTER_RECORD_ARRAY[character_ID].bonus_vitality;
                stat_boosted = true;
            }
            break;
        }
        case 2: {
            if (CHARACTER_RECORD_ARRAY[character_ID].magic < 255) {
                ++CHARACTER_RECORD_ARRAY[character_ID].bonus_magic;
                stat_boosted = true;
            }
            break;
        }
        case 3: {
            if (CHARACTER_RECORD_ARRAY[character_ID].spirit < 255) {
                ++CHARACTER_RECORD_ARRAY[character_ID].bonus_spirit;
                stat_boosted = true;
            }
            break;
        }
        case 4: {
            if (CHARACTER_RECORD_ARRAY[character_ID].dexterity < 255) {
                ++CHARACTER_RECORD_ARRAY[character_ID].bonus_dexterity;
                stat_boosted = true;
            }
            break;
        }
        case 5: {
            if (CHARACTER_RECORD_ARRAY[character_ID].luck < 255) {
                ++CHARACTER_RECORD_ARRAY[character_ID].bonus_luck;
                stat_boosted = true;
            }
            break;
        }
    }
    if (stat_boosted) {
        playMenuSound(263);
        recalculateBaseStats(partyIdx); //This should be rewritten to allow stat penalties
        recalculateDerivedStats(partyIdx);
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
bool handleTeachLimits(u8 partyIdx, u16 item_id, u16 inventory_index) {
    u8 characterID = getActivePartyMember(partyIdx)->characterID;
    bool isItemUsable = canCharacterUseItem(characterID, item_id); //If the character can't use the item, give the old "nothing to do with me message"
    bool limitTaught = false;
    auto registry = gContext.gameStrings.character_specific_strings[characterID];
    if (isItemUsable) {
        if (knows_all_prereq_limits(characterID)) { //Check if requisite limits are learned
            playMenuSound(384);
            CHARACTER_RECORD_ARRAY[characterID].learned_limits = (CHARACTER_RECORD_ARRAY[characterID].learned_limits | 0x0200);
            auto limit_learned_string = gContext.gameStrings.character_specific_strings[characterID].get_string(0); //String 0 is a characters "limit learned" string
            displayNewBoxString(limit_learned_string); //Get limit learned text, need to build out custom string storage
            sub_6C497C((int)byte_DD18C8, 7); //The game does this casting of a pointer as an arg... is ugly
            limitTaught = true;
        }
        else
        {
            auto not_ready_string = gContext.gameStrings.character_specific_strings[characterID].get_string(1);
            displayNewBoxString(not_ready_string); //get can't learn limit yet text. Can fetch from our own managed text array
            sub_6C497C((int)byte_DD18C8, 7);
            playMenuSound(3);
        }
    }
    else {
        auto cannot_learn_string = gContext.gameStrings.character_specific_strings[characterID].get_string(2);
        displayNewBoxString(cannot_learn_string); //get can't learn limit text
        sub_6C497C((int)byte_DD18C8, 7);
        playMenuSound(3);
    }
    return limitTaught;
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
