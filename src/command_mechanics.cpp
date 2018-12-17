#include "command_mechanics.h"
#include "impl.h"

SISTERRAY_API void CommandMainRewrite(u8* cmd) {
    u8 character_id;
    u32 attacker_id;
    u32 command_index;

    attacker_id = gDamageContextPtr->attackerID;
    character_id = gAiActorVariables[attacker_id].characterID;
    command_index = gDamageContextPtr->commandIndex;

    if (attacker_id < 3) {
        cmd[3] = CMD_DOUBLE_CUT;
        if ((command_index == CMD_ATTACK) && (character_id == ACT_CLOUD)) {
            /*When Cloud uses a physical attack, turn it into a double cut*/
            if (TriggerCloudSpecial()) {
                gDamageContextPtr->commandIndex = CMD_DOUBLE_CUT;
            }
        }
    }

    /*Call the games existinct Command Main Function*/
    oldCommandMain(cmd);

    if (0 && attacker_id < 3) {
        if ((command_index == CMD_ATTACK) && (character_id == ACT_CLOUD)) {
            /*Enqueue a potential extra action with priority 0 for execution*/
            if (TriggerCloudSpecial()) {
                /*Populate with the correct arguments*/
                u8 targetMask = gDamageContextPtr->targetMaskCopy;
                enqueueAction(attacker_id, (u16)0, CMD_DEATHBLOW, (u8)0, (u8)targetMask);
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


    attacker_id = gDamageContextPtr->attackerID;
    current_hp = gAiActorVariables[attacker_id].currentHP;
    max_hp = gAiActorVariables[attacker_id].maxHP;
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
