#include "command_mechanics.h"

SISTERRAY_API void CommandMainRewrite(u32 arg_1) {
    u8 character_id;
    u32 attacker_id;
    u32 command_index;

    attacker_id = DamageContextPtr->attackerID;
    character_id = AIActorVariables[attacker_id].characterID;
    command_index = DamageContextPtr->commandIndex;

    if (attacker_id < 3) {
        if ((command_index == ATTACK) && (character_id = CLOUD)) {
            /*When Cloud uses a physical attack, turn it into a double cut*/
            if (TriggerCloudSpecial()) {
                DamageContextPtr->commandIndex = DOUBLE_CUT;
            }
        }
    }

    /*Call the games existinct Command Main Function*/
    commandMain(arg_1);

    if (attacker_id < 3) {
        if ((command_index == ATTACK) && (character_id = CLOUD)) {
            /*Enqueue a potential extra action with priority 0 for execution*/
            if (TriggerCloudSpecial()) {
                /*Populate with the correct arguments*/
                u8 targetMask = DamageContextPtr->targetMaskCopy;
                enqueueAction(attacker_id, (u16)0, DEATHBLOW, (u8)0, (u8)targetMask);
            }
        }
    }
}

/*Determine whethe or not Cloud's special actions trigger*/
SISTERRAY_API int TriggerCloudSpecial() {
    u32 attacker_id;
    u32 current_hp;
    u32 max_hp;
    bool is_fury;
    bool in_front_row;


    attacker_id = DamageContextPtr->attackerID;
    current_hp = AIActorVariables[attacker_id].currentHP;
    max_hp = AIActorVariables[attacker_id].maxHP;
    is_fury = true;
    in_front_row = true;

    if (!(in_front_row)) {
        return false;
    }

    u32 base_chance = (1 - ((current_hp / max_hp)/2));

    if (is_fury) {
        base_chance = base_chance * 3;
    }

    return true;
}
