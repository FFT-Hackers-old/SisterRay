#ifndef USABLE_ITEM_HANDLERS_H
#define USABLE_ITEM_HANDLERS_H

#include <SisterRay/types.h>
#include "function_registry_template.h"
#include <unordered_map>
#include <string>

typedef bool(*onUseCallback)(u16, u16, u16);
typedef bool(*noTargetCallback)();

/*Hold the names of functions along with function ptrs*/
void initOnUseCallbackRegistry();
void initNoTargetCallbackRegistry();

/*Should refactor this into a template subclass function registry pattern*/
class srOnUseCallbackRegistry : public SrFunctionResourceRegistry<onUseCallback> {
public:
    srOnUseCallbackRegistry();
};

/*Holds functions to modularize items used that are not "targeted"*/
class srNoTargetCallbackRegistry : public SrFunctionResourceRegistry<noTargetCallback> {
public:
    srNoTargetCallbackRegistry();
};

// these are actual callbacks
bool default_no_target_use();
bool save_crystal_on_use();
bool default_item_use(u16 party_member_index, u16 item_id, u16 inventory_index);
bool heal_handler(u16 party_member_index, u16 item_id, u16 inventory_index);
bool permanently_boost_stat(u16 party_member_index, u16 item_id, u16 inventory_index);
bool teach_limit_breaks(u16 party_member_index, u16 item_id, u16 inventory_index);


// These are functions which should be moved to different files later on
bool heal_single_party_member(u16 party_member_index, u16 item_id);
u16 calculate_hp_heal_amount(u16 party_member_index, u16 item_id);
u16 calculate_mp_heal_amount(u16 party_member_index, u16 item_id);
void play_success_or_failure_sound(bool did_succeed, i32 success_sound_id, i32 failure_sound_id);
bool check_target_hp_healable(u16 target, u16 item_id);
bool check_target_mp_healable(u16 target, u16 item_id);
bool check_character_hp_full(u16 party_member_index);
bool check_character_mp_full(u16 party_member_index);

#endif
