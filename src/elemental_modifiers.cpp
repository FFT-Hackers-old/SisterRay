#include "elemental_modifiers.h"
#include <time.h>
#include <stdlib.h>

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

/*Routine handles the infliction of new elemental status effects*/
SISTERRAY_API void InflictElementalStatus() {
	u32 attacker_id;
	u32 target_id;
	u32 attack_elements_mask;

	attacker_id = gDamageContextPtr->attackerID;
	target_id = gDamageContextPtr->targetID;
	attack_elements_mask = gDamageContextPtr->attackElementsMask;

	/*Inflict burn if the attack is fire elemental*/
	if ((attack_elements_mask & ELM_FIRE_BIT) && (!disable_burn)) {
		/*If not soaked, chilled, or frozen inflict burn */
		if (!((gAiActorVariables[target_id].unused10 & STATUS_SOAKED) || (gAiActorVariables[target_id].unused10 & STATUS_FROZEN)
			|| (gAiActorVariables[target_id].unused10 & STATUS_CHILLED))) {
			/*If not already burned, initialize stuff*/
			if (!(gAiActorVariables[target_id].unused10 & STATUS_BURN)) {
				gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | STATUS_BURN);
				statusConstantArray[target_id].burnTickRate = (u16)defaultBurnTick;
				statusConstantArray[target_id].burnIntensity = (u16)defaultBurnTick;
				enqueueAction(target_id, 0, 0x23, 0x01, 0);
			}
			else {
				statusConstantArray[target_id].burnIntensity = (
					(statusConstantArray[target_id].burnIntensity > 1) ? (statusConstantArray[target_id].burnIntensity - 1) : statusConstantArray[target_id].burnIntensity);
			}
		}
	}
	disable_burn = 0;

	/*inflict chilled if the attack is ice elemental*/
	if (attack_elements_mask & ELM_ICE_BIT) {
		if (!((gAiActorVariables[target_id].unused10 & STATUS_BURN) || (gAiActorVariables[target_id].unused10 & STATUS_OVERHEAT))) {
			/*If the target is soaked, inflict frozen*/
			if (gAiActorVariables[target_id].unused10 & STATUS_SOAKED) {
				gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | STATUS_FROZEN);
			}
			/*else inflict the chilled status*/
			else {
				gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | STATUS_CHILLED);
			}
		}
	}
	/*inflict soaked if the attack is water elemental*/
	if (attack_elements_mask & ELM_WATER_BIT) {
		if (!((gAiActorVariables[target_id].unused10 & STATUS_BURN) || (gAiActorVariables[target_id].unused10 & STATUS_OVERHEAT))) {
			gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | STATUS_SOAKED);
		}
	}

	/*Allow Cut Attacks to inflict Bleed*/
	if ((attack_elements_mask & ELM_CUT_BIT)) {
		if (!(gAiActorVariables[target_id].unused10 & STATUS_BLEED)) {
			gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | STATUS_BLEED);
			statusConstantArray[target_id].bleedTickRate = (u16)defaultBleedTick;
			statusConstantArray[target_id].bleedIntensity = (u16)defaultBleedTick;
			enqueueAction(target_id, 0, 0x23, 0x02, 0);
		}
	}


	/*Arcing effect for lightning type attacks*/
	if ((attack_elements_mask & ELM_THUNDER_BIT) && (arc_enabled == 1)) {
		u16 target_mask;
		u16 allowed_targets = getAllowableArcTargets(target_id);
		u32 mask_of_source = gDamageContextPtr->targetMaskCopy;
		srand(time(NULL));
		int rand_int = (rand() % 2);
		if (allowed_targets) {
			target_mask = getRandomBit(allowed_targets);
		}
		if (target_mask) {
			enqueueAction(target_id, 0, CMD_POISONTICK, 0x05, target_mask);
		}
	}
	arc_enabled = 1;
}

u16 getRandomBit(u16 bit_vector) {
	srand(time(NULL));
	while (1) {
		u8 rand_bit = (rand() % 0xB);
		int bit_set = isBitSet(bit_vector, rand_bit);
		if (bit_set) {
			return (u16)(1 << rand_bit);
		}
	}
}

int isBitSet(u16 vector, int bit) {
	return 1 == ((vector >> bit) & 1);
}

/*This helper function returns a mask containing valid targets for lighting to arc to*/
u16 getAllowableArcTargets(u32 source_target) {
	u16 allowable_targets = 0;
	u32 source_state = gAiActorVariables[source_target].stateFlags;
	switch (source_target) {
	case 0:
		u8 enabled = gAiActorVariables[0x02].stateFlags & 0x0008;
		if (enabled) {
			allowable_targets = 0x02; 
		}
		break;
	case 1:
		u8 enabled_left = gAiActorVariables[0x04].stateFlags & 0x0008;
		u8 enabled_right = gAiActorVariables[0x01].stateFlags & 0x0008;
		if (enabled_left && enabled_right) {
			allowable_targets = (0x01 | 0x04);
		}
		else if (enabled_left) {
			allowable_targets = 0x04;
		}
		else if (enabled_right) {
			allowable_targets = 0x01;
		}
		break;
	case 2:
		u8 enabled = gAiActorVariables[0x02].stateFlags & 0x0008;
		if (enabled) {
			allowable_targets = 0x02;
		}
		break;
	default:
		for (int actor = 4; actor <= 9; actor++) {
			u32 actor_state = gAiActorVariables[actor].stateFlags;
			if ((actor_state & 0x0008) && (!(actor_state & 0x2000)) && (source_target != actor)) {
				allowable_targets = allowable_targets | (1 << (actor));
			}
		}
	}
	return allowable_targets;
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
        gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 & ~STATUS_BURN);
    }
    if (attack_elements_mask & ELM_THUNDER_BIT) {
        /*Increase damage by 1.5*/
        gDamageContextPtr->currentDamage = ((gDamageContextPtr->currentDamage) * 3)/2;
    }
    /*If wind attack hits soaked target*/
    if (attack_elements_mask & ELM_WIND_BIT) {
        gDamageContextPtr->currentDamage = ((gDamageContextPtr->currentDamage) * 4)/3;
        /*Set the chilled status on the target*/
        gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | STATUS_CHILLED);
        gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 & ~STATUS_BURN);
    }
}
