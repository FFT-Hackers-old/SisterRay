#include "elemental_modifiers.h"
#include "utils.h"

SISTERRAY_API void InflictElementalStatus() {
	u32 attacker_id;
	u32 target_id;
	u32 attack_elements_mask;

	attacker_id = gDamageContextPtr->attackerID;
	target_id = gDamageContextPtr->targetID;
	attack_elements_mask = gDamageContextPtr->attackElementsMask;

	/*Inflict burn if the attack is fire elemental*/
	if ((attack_elements_mask & ELM_FIRE_BIT) && (!disable_burn)) {
		handleInflictBurn(target_id);
	}
	disable_burn = 0;

	/*inflict chilled if the attack is ice elemental*/
	if (attack_elements_mask & ELM_ICE_BIT) {
		handleInflictChilled(target_id);
	}

	/*inflict soaked if the attack is water elemental*/
	if (attack_elements_mask & ELM_WATER_BIT) {
		handleInflictSoaked(target_id);
	}

	/*Allow Cut Attacks to inflict Bleed*/
	if ((attack_elements_mask & ELM_CUT_BIT)) {
		handleInflictBleed(target_id);
	}

	/*Arcing effect for lightning type attacks*/
	if ((attack_elements_mask & ELM_THUNDER_BIT) && (arc_enabled == 1)) {
		handleLightingArc(target_id);
	}
	arc_enabled = 1;

	/*Allow Wind attacks to inflict float, no current animation*/
	if ((attack_elements_mask & ELM_WIND_BIT)) {
		handleInflictFloat(target_id);
	}
}

void handleInflictBurn(u32 target_id) {
	/*If not soaked, chilled, or frozen inflict burn */
	if (!((gAiActorVariables[target_id].unused10 & STATUS_SOAKED) || (gAiActorVariables[target_id].unused10 & STATUS_FROZEN)
		|| (gAiActorVariables[target_id].unused10 & STATUS_CHILLED))) {
		/*If not already burned, initialize stuff*/
		if (!(gAiActorVariables[target_id].unused10 & STATUS_BURN)) {
			gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | STATUS_BURN);
			statusConstantArray[target_id].burnTickRate = defaultBurnIntensity;
			statusConstantArray[target_id].burnIntensity = defaultBurnIntensity;
			enqueueAction(target_id, 0, 0x23, 0x01, 0);
		}
		else {
			statusConstantArray[target_id].burnIntensity = (
				(statusConstantArray[target_id].burnIntensity > 1) ? (statusConstantArray[target_id].burnIntensity - 1) : statusConstantArray[target_id].burnIntensity);
		}
	}
}

void handleInflictChilled(u32 target_id) {
	if (!((gAiActorVariables[target_id].unused10 & STATUS_BURN) || (gAiActorVariables[target_id].unused10 & STATUS_OVERHEAT))) {
		/*If the target is soaked, inflict frozen*/
		if (gAiActorVariables[target_id].unused10 & STATUS_SOAKED) {
			gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | STATUS_FROZEN);
		}
		/*else inflict the chilled status or make it stronger*/
		else {
			if (!(gAiActorVariables[target_id].unused10 & STATUS_CHILLED)) {
				gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | STATUS_CHILLED);
				statusConstantArray[target_id].chilledIntensity = defaultChilledIntensity;
			}
			statusConstantArray[target_id].chilledIntensity = (
				(statusConstantArray[target_id].chilledIntensity > 1) ? (statusConstantArray[target_id].chilledIntensity - 1) : statusConstantArray[target_id].chilledIntensity);
		}
	}
}

void handleInflictSoaked(u32 target_id) {
	if (!((gAiActorVariables[target_id].unused10 & STATUS_BURN) || (gAiActorVariables[target_id].unused10 & STATUS_OVERHEAT))) {
		gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | STATUS_SOAKED);
	}
}

void handleInflictBleed(u32 target_id) {
	if (!(gAiActorVariables[target_id].unused10 & STATUS_BLEED)) {
		gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | STATUS_BLEED);
		statusConstantArray[target_id].bleedTickRate = defaultBleedIntensity;
		statusConstantArray[target_id].bleedIntensity = defaultBleedIntensity;
		enqueueAction(target_id, 0, 0x23, 0x02, 0);
	}
}

void handleLightingArc(u32 target_id) {
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

void handleInflictFloat(u32 target_id) {
	if (!(gAiActorVariables[target_id].unused10 & STATUS_FLOAT)) {
		gAiActorVariables[target_id].unused10 = (gAiActorVariables[target_id].unused10 | STATUS_FLOAT);
		statusConstantArray[target_id].floatDuration = defaultFloatDuration;
	}
}

/*This helper function returns a mask containing valid targets for lighting to arc to*/
u16 getAllowableArcTargets(u32 source_target) {
	u16 allowable_targets = 0;
	u32 source_state = gAiActorVariables[source_target].stateFlags;
	u8 enabled;
	u8 enabled_left;
	u8 enabled_right;
	switch (source_target) {
	case 0:
		enabled = gAiActorVariables[0x02].stateFlags & 0x0008;
		if (enabled) {
			allowable_targets = 0x02;
		}
		break;
	case 1:
		enabled_left = gAiActorVariables[0x04].stateFlags & 0x0008;
		enabled_right = gAiActorVariables[0x01].stateFlags & 0x0008;
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
		enabled = gAiActorVariables[0x02].stateFlags & 0x0008;
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