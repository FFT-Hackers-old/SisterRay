#include "elemental_modifiers.h"

/*reimplementaiton of elemental modifier mask*/

newStatusActorBlock statusConstantArray[10];
int arc_enabled = 1;
int disable_burn = 0;

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
    if (new_actor_status_mask & STATUS_BURN) {
        applyBurnedInteractions(new_actor_status_mask, attack_elements_mask, target_id);

    }
    /*handle elemental interactions with the soaked status*/
    if (new_actor_status_mask & STATUS_SOAKED) {
        applySoakedInteractions(new_actor_status_mask, attack_elements_mask, target_id);
    }

	/*apply float interactions*/
	if (new_actor_status_mask & STATUS_FLOAT) {
		applyFloatInteractions(new_actor_status_mask, attack_elements_mask, target_id);
	}
}

/*Handle burned elemental interactions*/
void applyBurnedInteractions(u16 new_actor_status_mask, u32 attack_elements_mask, u32 target_id) {
    /*Ice and Water damage reduced*/
    if (attack_elements_mask & ELM_ICE_BIT) {
        gDamageContextPtr->currentDamage = (gDamageContextPtr->currentDamage) / 2;
        /*Flag soaked on the target*/
        gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | STATUS_SOAKED);
        /*Unset burn on target*/
        gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 & ~STATUS_BURN);
    }
    if (attack_elements_mask & ELM_WATER_BIT) {
        gDamageContextPtr->currentDamage = (gDamageContextPtr->currentDamage) / 2;
        /*Unset burn on target*/
        gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 & ~STATUS_BURN);
    }
}

/*Handle soaked elemental interactions*/
void applySoakedInteractions(u16 new_actor_status_mask, u32 attack_elements_mask, u32 target_id) {
    if (attack_elements_mask & ELM_FIRE_BIT) {
        gDamageContextPtr->currentDamage = (gDamageContextPtr->currentDamage) / 2;
        /*Unset soaked on target*/
        gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 & ~STATUS_SOAKED);
    }
    if (attack_elements_mask & ELM_THUNDER_BIT) {
        /*Increase damage by 1.5*/
        gDamageContextPtr->currentDamage = ((gDamageContextPtr->currentDamage) * 3) / 2;
    }
    /*If wind attack hits soaked target*/
    if (attack_elements_mask & ELM_WIND_BIT) {
        gDamageContextPtr->currentDamage = ((gDamageContextPtr->currentDamage) * 4) / 3;
        /*Set the chilled status on the target*/
        gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | STATUS_CHILLED);
        gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 & ~STATUS_SOAKED);
    }
	if (attack_elements_mask & ELM_ICE_BIT) {
		gDamageContextPtr->currentDamage = ((gDamageContextPtr->currentDamage) * 4) / 3;
	}
}

/*Handle soaked elemental interactions*/
void applyFloatInteractions(u16 new_actor_status_mask, u32 attack_elements_mask, u32 target_id) {
	if (attack_elements_mask & ELM_EARTH_BIT) {
		gDamageContextPtr->currentDamage = 0;
	}
	if (attack_elements_mask & ELM_THUNDER_BIT) {
		/*Reduce damage by 80%, as target not grounded*/
		gDamageContextPtr->currentDamage = (gDamageContextPtr->currentDamage) / 5;
		gAiActorVariables[target_id].unused10 = gAiActorVariables[target_id].unused10 | STATUS_CHARGED;
		statusConstantArray[target_id].chargeIntensity++;
	}
	/*If wind attack hits floating target, do more damage/blow them away
	  For now, stick to a damage multiplier*/
	if (attack_elements_mask & ELM_WIND_BIT) {
		gDamageContextPtr->currentDamage = ((gDamageContextPtr->currentDamage) * 3) / 2;
	}
}
