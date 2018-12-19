#include "command_mechanics.h"
#include "impl.h"

SISTERRAY_API void CommandMainRewrite(u8* cmd) {
    u8 character_id;
    u32 attacker_id;
    u8 command_index;

    attacker_id = cmd[2];
    character_id = gAiActorVariables[attacker_id].characterID;
    command_index = cmd[3];

    if (attacker_id < 3) {
        /*remove the command index check and see queue behavior working*/
        if ((command_index == CMD_ATTACK) && (character_id == ACT_CLOUD)){
             cmd[3] = CMD_DOUBLE_CUT;
             gDamageContextPtr->commandIndex = CMD_DOUBLE_CUT;

             u16 targetMask = cmd[6];
             enqueueAction(attacker_id, 0, CMD_DEATHBLOW, 0, targetMask);
           
        }
    }
    /*Call the games existing Command Main Function*/
    oldCommandMain(cmd);
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

/*This function should hijack the effect of poison
 It should now be fire elemental, and trigger a fire animation*/
SISTERRAY_API void ModifyPoisonTest() {
    gDamageContextPtr->attackElementsMask = (u32)ELM_FIRE_BIT;
    gDamageContextPtr->abilityPower = 2;
    gDamageContextPtr->targetStateMask = (u32)0;
    //gDamageContextPtr->commandIndex = 0x23;
    //gDamageContextPtr->actionIDCopy = 0x23;
    gDamageContextPtr->animationScriptID = (u32)0x14;
    gDamageContextPtr->animationBaseOffset = (u32)0;
    gDamageContextPtr->AttackEffectID = (u32)0x1B;
    gDamageContextPtr->impactEffectID = (u32)0x3F;
    gDamageContextPtr->impactSound = 0x090;
    gDamageContextPtr->critkSound = 0x090;
}
