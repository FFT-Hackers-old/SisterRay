#ifndef USABLE_ITEM_HANDLERS_H
#define USABLE_ITEM_HANDLERS_H

#include <SisterRay/types.h>
#include "sr_callback_registry.h"
#include <unordered_map>
#include <string>

typedef bool(*onUseCallback)(u8, u16, u16);
typedef bool(*noTargetCallback)();

/*Hold the names of functions along with function ptrs*/
void initOnUseCallbackRegistry();
void initNoTargetCallbackRegistry();

/*Should refactor this into a template subclass function registry pattern*/
class srOnUseCallbackRegistry : public SrIndexedCallbackRegistry<onUseCallback> {
public:
    srOnUseCallbackRegistry();
};

/*Holds functions to modularize items used that are not "targeted"*/
class srNoTargetCallbackRegistry : public SrIndexedCallbackRegistry<noTargetCallback> {
public:
    srNoTargetCallbackRegistry();
};

// these are actual callbacks
bool defaultUseNoTarget();
bool useSaveCrystal();
bool defaultUseItem(u8 partyIdx, u16 item_id, u16 inventory_index);
bool handleHeal(u8 partyIdx, u16 item_id, u16 inventory_index);
bool handleBoostStat(u8 partyIdx, u16 item_id, u16 inventory_index);
bool handleTeachLimits(u8 partyIdx, u16 item_id, u16 inventory_index);


// These are functions which should be moved to different files later on
bool healPartyMember(u8 partyIdx, u16 item_id);
u16 calculateHPHealAmount(u8 partyIdx, u16 item_id);
u16 calculateMPHealAmount(u8 partyIdx, u16 item_id);
void playUseSound(bool did_succeed, i32 success_sound_id, i32 failure_sound_id);
bool checkIsHPHealable(u8 target, u16 item_id);
bool checkIsMPHealable(u8 target, u16 item_id);
bool checkActorHPFull(u8 partyIdx);
bool checkActorMPFull(u8 partyIdx);

#endif
