#include "elemental_modifiers.h"

/*reimplementaiton of elemental modifier mask*/

SISTERRAY_API void ApplyElementalModifiers() {
    u32 elm_def_mask = gDamageContextPtr->elementalDefenseMask;

    /*handle absorb*/
    if (elm_def_mask & 0x40) {
        gDamageContextPtr->abilityFlags2 = (gDamageContextPtr->abilityFlags2 ^ 0x0001);
    }
    else {
        /*handle weakness*/
        if (elm_def_mask & 0x04) {
            gDamageContextPtr->currentDamage = (gDamageContextPtr->currentDamage) * 2;
        }
        /*handle resistance*/
        if (elm_def_mask & 0x10) {
            gDamageContextPtr->currentDamage = (gDamageContextPtr->currentDamage + 1) / 2;
        }
    }

    /*handle_death*/
    if (elm_def_mask & 0x01) {
        if (gDamageContextPtr->targetStatusMask & 0x01){
            gDamageContextPtr->abilityFlags1= ((gDamageContextPtr->abilityFlags1) | 3);
            nullMasks();
        }
        else {
            /*handle the flags*/
            gDamageContextPtr->abilityFlags1 = ((gDamageContextPtr->abilityFlags1) & 0xfffffffd);
            gDamageContextPtr->abilityFlags2 = ((gDamageContextPtr->abilityFlags2) & 0xfffffffe);
            gDamageContextPtr->addStatusMaskCopy = ((gDamageContextPtr->addStatusMaskCopy) | 0x01);
            gDamageContextPtr->rmStatusMaskCopy = ((gDamageContextPtr->rmStatusMaskCopy) & 0xfffffffe);
            gDamageContextPtr->killRecoverDamage = 0xfffffffe;
        }
    }
    /*handle_recovery*/
    else if (elm_def_mask & 80) {
        u32 target_id = gDamageContextPtr->targetID;
        gAiActorVariables[gDamageContextPtr->targetID].currentHP = gAiActorVariables[gDamageContextPtr->targetID].maxHP;
        gAiActorVariables[gDamageContextPtr->targetID].currentMP = gAiActorVariables[gDamageContextPtr->targetID].maxMP;

        /*handle le flags*/
        gDamageContextPtr->abilityFlags1 = ((gDamageContextPtr->abilityFlags1) & 0xfffffffd);
        gDamageContextPtr->abilityFlags2 = 0x01;
        gDamageContextPtr->addStatusMaskCopy = ((gDamageContextPtr->addStatusMaskCopy) & 0xfffffffe);
        gDamageContextPtr->killRecoverDamage = 0xfffffffd;
    }
    else {
        /*handle nullification*/
        if (elm_def_mask & 0x20){
            if (((gDamageContextPtr->combinedStatusMask) != 0x00) || (((gDamageContextPtr-> attackElementsMask) & 0x01) != 0)) {
                gDamageContextPtr->abilityFlags1 = ((gDamageContextPtr->abilityFlags1) | 00000001);
            }
            nullMasks();
        }
    }

	/*Inflict elemental status effects assuming the target doesn't resist the given element*/
	if (!((elm_def_mask & 0x40)||(elm_def_mask & 0x020))) {
		InflictElementalStatus();
	}

	/*If any elemental status bits are flagged, handle their interactions*/
	if (gAiActorVariables[gDamageContextPtr->targetID].unused10) {
		HandleElementalInteractions();
	}
}

/*Routine handles the infliction of new elemental status effects*/
SISTERRAY_API void InflictElementalStatus() {
    u32 attacker_id;
    u32 target_id;
    u32 attack_elements_mask;

    attacker_id = gDamageContextPtr->attackerID;
    target_id = gDamageContextPtr->targetID;
    attack_elements_mask = gDamageContextPtr->attackElementsMask;

    /*Inflict burn if the attack is fire elemental*/
    if (attack_elements_mask & ELM_FIRE_BIT) {
        /*If not soaked, chilled, or frozen inflict burn */
        if (!((gAiActorVariables[target_id].unused10 & 0x0200) || (gAiActorVariables[target_id].unused10 & 0x0400)
            || (gAiActorVariables[target_id].unused10 & 0x0800))) {
            /*If not already burned, initialize stuff*/
            if (!(gAiActorVariables[target_id].unused10 & 0x2000)) {
                gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | 0x2000);
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
    if (attack_elements_mask & ELM_ICE_BIT) {
        if (!((gAiActorVariables[target_id].unused10 & 0x2000) || (gAiActorVariables[target_id].unused10 & 0x1000))) {
            /*If the target is soaked, inflict frozen*/
            if (gAiActorVariables[target_id].unused10 & 0x0200) {
                gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | 0x0400);
            }
            /*else inflict the chilled status*/
            else {
                gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | 0x0800);
            }
        }
    }
    /*inflict soaked if the attack is water elemental*/
    if (attack_elements_mask & ELM_WATER_BIT) {
        if (!((gAiActorVariables[target_id].unused10 & 0x2000) || (gAiActorVariables[target_id].unused10 & 0x1000))) {
            gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | 0x0200);
        }
    }
}


/*Function for handling damage modifiers due to elemental interactions*/
SISTERRAY_API void HandleElementalInteractions() {
    u32 attacker_id;
    u32 target_id;
    u32 attack_elements_mask;

    attacker_id= gDamageContextPtr->attackerID;
    target_id = gDamageContextPtr->targetID;
    attack_elements_mask = gDamageContextPtr->attackElementsMask;

    /*Fetch 16 bits of unused AI actor Data to use as new elements*/
    u16 new_actor_status_mask = gAiActorVariables[target_id].unused10;

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
    if (attack_elements_mask & ELM_ICE_BIT) {
        gDamageContextPtr->currentDamage = (gDamageContextPtr->currentDamage) / 2;
        /*Flag soaked on the target*/
        gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | 0x0200);
        /*Unset burn on target*/
        gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 & ~0x2000);
    }
    if (attack_elements_mask & ELM_WATER_BIT) {
        gDamageContextPtr->currentDamage = (gDamageContextPtr->currentDamage) / 2;
        /*Unset burn on target*/
        gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 & ~0x2000);
    }
}

/*Handle soaked elemental interactions*/
void applySoakedInteractions(u16 new_actor_status_mask, u32 attack_elements_mask, u32 target_id) {
    if (attack_elements_mask & ELM_FIRE_BIT) {
        gDamageContextPtr->currentDamage = (gDamageContextPtr->currentDamage) / 2;
        /*Unset soaked on target*/
        gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 & ~0x2000);
    }
    if (attack_elements_mask & ELM_THUNDER_BIT) {
        /*Increase damage by 1.5*/
        gDamageContextPtr->currentDamage = (gDamageContextPtr->currentDamage) * 1.75;
    }
    /*If wind attack hits soaked target*/
    if (attack_elements_mask & ELM_WIND_BIT) {
        gDamageContextPtr->currentDamage = (gDamageContextPtr->currentDamage) * 1.5;
        /*Set the chilled status on the target*/
        gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | 0x0800);
        gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 & ~0x2000);
    }
}
