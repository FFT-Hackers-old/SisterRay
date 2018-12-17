#include "elemental_modifiers.h"

/*reimplementaiton of elemental modifier mask*/

SISTERRAY_API void ApplyElementalModifers() {
    u32 elm_def_mask = (*DamageContextPtr).elementalDefenseMask;

    /*handle absorb*/
    if (elm_def_mask & 0x40) {
        DamageContextPtr->abilityFlags2 = (DamageContextPtr->abilityFlags2 ^ 0x0001);
    }
    else {
        /*handle weakness*/
        if (elm_def_mask & 0x04) {
            DamageContextPtr->currentDamage = (DamageContextPtr->currentDamage) * 2;
        }
        /*handle resistance*/
        if (elm_def_mask & 0x10) {
            DamageContextPtr->currentDamage = (DamageContextPtr->currentDamage + 1) / 2;
        }
    }

    /*handle_death*/
    if (elm_def_mask & 0x01) {
        if (DamageContextPtr->targetStatusMask & 0x01){
            DamageContextPtr->abilityFlags1= ((DamageContextPtr->abilityFlags1) | 3);
            nullMasks();
        }
        else {
            /*handle the flags*/
            DamageContextPtr -> abilityFlags1 = ((DamageContextPtr -> abilityFlags1) & 0xfffffffd);
            DamageContextPtr -> abilityFlags2 = ((DamageContextPtr -> abilityFlags2) & 0xfffffffe);
            DamageContextPtr -> addStatusMaskCopy = ((DamageContextPtr -> addStatusMaskCopy) | 0x01);
            DamageContextPtr -> rmStatusMaskCopy = ((DamageContextPtr->rmStatusMaskCopy) & 0xfffffffe);
            DamageContextPtr -> killRecoverDamage = 0xfffffffe;
        }
    }
    /*handle_recovery*/
    else if (elm_def_mask & 80) {
        u32 target_id = DamageContextPtr->targetID;
        AIActorVariables[DamageContextPtr->targetID].currentHP = AIActorVariables[DamageContextPtr->targetID].maxHP;

        AIActorVariables[DamageContextPtr->targetID].currentMP = AIActorVariables[DamageContextPtr->targetID].maxMP;

        /*handle le flags*/
        DamageContextPtr->abilityFlags1 = ((DamageContextPtr->abilityFlags1) & 0xfffffffd);
        DamageContextPtr->abilityFlags2 = 0x01;
        DamageContextPtr->addStatusMaskCopy = ((DamageContextPtr->addStatusMaskCopy) & 0xfffffffe);
        DamageContextPtr->killRecoverDamage = 0xfffffffd;
    }
    else {
        /*handle nullification*/
        if (elm_def_mask & 0x20){
            if (((DamageContextPtr->combinedStatusMask) != 0x00) || (((DamageContextPtr -> attackElementsMask) & 0x01) != 0)) {
                DamageContextPtr->abilityFlags1 = ((DamageContextPtr->abilityFlags1) | 00000001);
            }
            nullMasks();
        }
    }
}

/*Routine handles the infliction of new elemental status effects*/
SISTERRAY_API void InflictElementalStatus() {
    u32 attacker_id;
    u32 target_id;
    u32 attack_elements_mask;

    attacker_id = DamageContextPtr->attackerID;
    target_id = DamageContextPtr->targetID;
    attack_elements_mask = DamageContextPtr->attackElementsMask;

    /*Inflict burn if the attack is fire elemental*/
    if (attack_elements_mask & fireBit) {
        /*If not soaked, chilled, or frozen inflict burn */
        if (!((AIActorVariables[target_id].unused10 & 0x0200) || (AIActorVariables[target_id].unused10 & 0x0400)
            || (AIActorVariables[target_id].unused10 & 0x0800))) {
            /*If not already burned, initialize stuff*/
            if (!(AIActorVariables[target_id].unused10 & 0x2000)) {
                AIActorVariables[target_id].unused10 = (AIActorVariables[target_id].unused10 | 0x2000);
                statusConstantArray[target_id].burnDuration = defaultBurnDuration;
                statusConstantArray[target_id].burnTickRate = defaultBurnTick;
                statusConstantArray[target_id].burnIntensity = defaultBurnIntensity;
            }
            else {
                statusConstantArray[target_id].burnDuration = (
                    (statusConstantArray[target_id].burnDuration <= 255) ? (statusConstantArray[target_id].burnDuration + (defaultBurnDuration / 2)):statusConstantArray[target_id].burnDuration);
                statusConstantArray[target_id].burnIntensity = (
                    (statusConstantArray[target_id].burnIntensity <= 5) ? (statusConstantArray[target_id].burnIntensity + 1):statusConstantArray[target_id].burnIntensity);
            }
        }
    }

    /*inflict chilled if the attack is ice elemental*/
    if (attack_elements_mask & iceBit) {
        if (!((AIActorVariables[target_id].unused10 & 0x2000) || (AIActorVariables[target_id].unused10 & 0x1000))) {
            /*If the target is soaked, inflict frozen*/
            if (AIActorVariables[target_id].unused10 & 0x0200) {
                AIActorVariables[target_id].unused10 = (AIActorVariables[target_id].unused10 | 0x0400);
            }
            /*else inflict the chilled status*/
            else {
                AIActorVariables[target_id].unused10 = (AIActorVariables[target_id].unused10 | 0x0800);
            }
        }
    }
    /*inflict soaked if the attack is water elemental*/
    if (attack_elements_mask & waterBit) {
        if (!((AIActorVariables[target_id].unused10 & 0x2000) || (AIActorVariables[target_id].unused10 & 0x1000))) {
            AIActorVariables[target_id].unused10 = (AIActorVariables[target_id].unused10 | 0x0200);
        }
    }
}


/*Function for handling damage modifiers due to elemental interactions*/
SISTERRAY_API void HandleElementalInteractions() {
    u32 attacker_id;
    u32 target_id;
    u32 attack_elements_mask;

    attacker_id= DamageContextPtr->attackerID;
    target_id = DamageContextPtr->targetID;
    attack_elements_mask = DamageContextPtr->attackElementsMask;

    /*Fetch 16 bits of unused AI actor Data to use as new elements*/
    u16 new_actor_status_mask = AIActorVariables[target_id].unused10;

    /*If the target is inflicted with Burn*/
    if (new_actor_status_mask & 0x2000) {
        applyBurnedInteractions(new_actor_status_mask, attack_elements_mask, target_id);

    }
    /*handle elemental interactions with the soaked status*/
    if (new_actor_status_mask & 0x0200) {
        applySoakedInteractions(new_actor_status_mask, attack_elements_mask, target_id);
    }
}

/*Handle burned elemental interactions*/
void applyBurnedInteractions(u16 new_actor_status_mask, u32 attack_elements_mask, u32 target_id) {
    /*Ice and Water damage reduced*/
    if (attack_elements_mask & iceBit) {
        DamageContextPtr->currentDamage = (DamageContextPtr->currentDamage) / 2;
        /*Flag soaked on the target*/
        AIActorVariables[target_id].unused10 = (AIActorVariables[target_id].unused10 | 0x0200);
        /*Unset burn on target*/
        AIActorVariables[target_id].unused10 = (AIActorVariables[target_id].unused10 & ~0x2000);
    }
    if (attack_elements_mask & waterBit) {
        DamageContextPtr->currentDamage = (DamageContextPtr->currentDamage) / 2;
        /*Unset burn on target*/
        AIActorVariables[target_id].unused10 = (AIActorVariables[target_id].unused10 & ~0x2000);

    }
}

/*Handle soaked elemental interactions*/
void applySoakedInteractions(u16 new_actor_status_mask, u32 attack_elements_mask, u32 target_id) {
    if (attack_elements_mask & fireBit) {
        DamageContextPtr->currentDamage = (DamageContextPtr->currentDamage) / 2;
        /*Unset soaked on target*/
        AIActorVariables[target_id].unused10 = (AIActorVariables[target_id].unused10 & ~0x2000);
    }
    if (attack_elements_mask & boltBit) {
        /*Increase damage by 1.5*/
        DamageContextPtr->currentDamage = (DamageContextPtr->currentDamage) * 1.75;
    }
    /*If wind attack hits soaked target*/
    if (attack_elements_mask & windBit) {
        DamageContextPtr->currentDamage = (DamageContextPtr->currentDamage) * 1.5;
        /*Set the chilled status on the target*/
        AIActorVariables[target_id].unused10 = (AIActorVariables[target_id].unused10 | 0x0800);
        AIActorVariables[target_id].unused10 = (AIActorVariables[target_id].unused10 & ~0x2000);
    }
}
