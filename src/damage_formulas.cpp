#include "damage_formulas.h"

SISTERRAY_API void PhysicalFormulaRewrite() {
    u32 atk;
    u32 lvl;
    u32 base_damage;
    u32 defense;
    u32 ability_power;
    u32 attacker_status;

    /*auto critical check*/
    if ((gDamageContextPtr->specialAbilityFlags & 0x2000) == 0) {
        gDamageContextPtr->abilityFlags2 = ((gDamageContextPtr->abilityFlags2) | 0x02);
    }

    atk = gDamageContextPtr->attackerAtk;
    lvl = gDamageContextPtr->attackerLevel;
    defense = gDamageContextPtr->targetDefense;
    ability_power = gDamageContextPtr->abilityPower;
    attacker_status = gDamageContextPtr->attackerStatusMask;

    base_damage = atk + ((atk + lvl) / 32)*((atk*lvl) / 32);

    /*apply defense*/
    base_damage = ((264 - defense)*base_damage) / 256; //scale to 95% damage reduction
    base_damage = (ability_power*base_damage) / 16;

    /*check crit*/
    if (gDamageContextPtr->abilityFlags2 & 2) {
        base_damage = (3 * base_damage)/2;
		applyWounds();
    }

    /*berserk check*/
    if (attacker_status & 0x00800000) {
        base_damage = (4*base_damage)/3;
    }

    /*row modification, now respects enemy short-rangedness*/
    bool attacker_row = ((gAiActorVariables[gDamageContextPtr->attackerID].stateFlags) & 0x40);
    bool target_row = ((gAiActorVariables[gDamageContextPtr->targetID].stateFlags) & 0x40);
    bool is_long_range = (gDamageContextPtr->targetStateFlags & 0x20);

    if (target_row) {
        if (!(is_long_range)) {
            base_damage = base_damage / 2;
        }
    }
    else if(attacker_row) {
        base_damage = base_damage / 2;
    }

    /*defend check*/
    bool is_defending = ((gAiActorVariables[gDamageContextPtr->attackerID].stateFlags) & 0x20);
    if (is_defending) {
        base_damage = base_damage / 2;
    }

    /*back_attack_handling*/
    bool is_back_attack = (gDamageContextPtr->targetStateFlags & 0x01);
    if (is_back_attack) {
        u32 back_damage_mult = gAiActorVariables[gDamageContextPtr->targetID].backDamageMult;
        base_damage = (base_damage*back_damage_mult) / 8;
    }

    /*frog modifier*/
    if (attacker_status & 0x800) {
        base_damage = base_damage / 5;
    }

    /*apply checks from existing modifiers*/
    base_damage = sadnessModifier(base_damage);
    base_damage = spltDamageQuadraMagicModifier(base_damage, 0);
    base_damage = barriersModifier(base_damage);


    /*mini modifier*/
    if (attacker_status & 0x1000) {
        base_damage = base_damage / 10;
    }

    /*randomVariation*/
    base_damage = randomVariation(base_damage);

    /*set the base damage in the context object*/
    gDamageContextPtr->currentDamage = base_damage;
}

SISTERRAY_API void MagicFormulaRewrite() {
    u32 atk;
    u32 lvl;
    u32 base_damage;
    u32 defense;
    u32 ability_power;
    u32 attacker_status;
    u32 quadra_magic_arg;

    atk = gDamageContextPtr->attackerAtk;
    lvl = gDamageContextPtr->attackerLevel;
    defense = gDamageContextPtr->targetDefense;
    ability_power = gDamageContextPtr->abilityPower;
    attacker_status = gDamageContextPtr->attackerStatusMask;
    quadra_magic_arg = 0;

    if (!((gDamageContextPtr->targetStateMask & 0xC)^0x04)) {
        quadra_magic_arg = 1;
    }

    base_damage = (6*(atk+lvl));

    /*apply defense*/
    base_damage = ((264 - defense)*base_damage) / 256; //Scale to 95% damage reduction
    base_damage = (ability_power*base_damage) / 16;

    /*apply checks from existing modifiers*/
    base_damage = mpTurbo(base_damage);
    base_damage = sadnessModifier(base_damage);
    base_damage = spltDamageQuadraMagicModifier(base_damage, 0);
    base_damage = barriersModifier(base_damage);

    /*randomVariation*/
    base_damage = randomVariation(base_damage);

    /*set the base damage in the context object*/
    gDamageContextPtr->currentDamage = base_damage;
}

SISTERRAY_API void applyWounds() {
	u32 target_id = gDamageContextPtr->targetID;
	u16 attack_elements_mask = gDamageContextPtr->attackElementsMask;

	/*Set the wounded bit in AI data and */
	if (attack_elements_mask & (ELM_CUT_BIT | ELM_SHOOT_BIT)) {
		gAiActorVariables[target_id].unused10 = gAiActorVariables[target_id].unused10 | 0x4000;
		statusConstantArray[target_id].SeriousWoundCount = (statusConstantArray[target_id].SeriousWoundCount <= 4) ? (statusConstantArray[target_id].SeriousWoundCount++) : (statusConstantArray[target_id].SeriousWoundCount);
	}
	else if (attack_elements_mask & ELM_PUNCH_BIT) {
		gAiActorVariables[target_id].unused10 = gAiActorVariables[target_id].unused10 | 0x4000;
		statusConstantArray[target_id].GrievousWoundCount = (statusConstantArray[target_id].GrievousWoundCount <= 2) ? (statusConstantArray[target_id].GrievousWoundCount++) : (statusConstantArray[target_id].GrievousWoundCount);
	}
}
