#include "damage_formulas.h"

void PhysicalFormulaRewrite() {
    i32 atk;
    i32 lvl;
    i32 base_damage;
    i32 final_damage;
    u32 defense;
    u32 ability_power;
    u32 attacker_status;

    /*auto critical check*/
    if ((DamageContextPtr->specialAbilityFlags & 0x2000) == 0) {
        DamageContextPtr->abilityFlags2 = ((DamageContextPtr->abilityFlags2) | 0x02);
    }

    atk = DamageContextPtr->attackerAtk;
    lvl = DamageContextPtr->attackerLevel;
    defense = DamageContextPtr->targetDefense;
    ability_power = DamageContextPtr->abilityPower;
    attacker_status = DamageContextPtr->attackerStatusMask;

    base_damage = atk + ((atk + lvl) / 32)*((atk*lvl) / 32);

    /*apply defense*/
    base_damage = ((256 - defense)*base_damage) / 256;
    base_damage = (ability_power*base_damage) / 16;

    /*check crit*/
    if (DamageContextPtr->abilityFlags2 & 2) {
        base_damage = 1.5f * base_damage;
    }

    /*berserk check*/
    if (attacker_status & 0x00800000) {
        base_damage = 1.3f * base_damage;
    }

    /*row modification, now respects enemy short-rangedness*/
    bool attacker_row = ((AIActorVariables[DamageContextPtr->attackerID].stateFlags) & 0x40);
    bool target_row = ((AIActorVariables[DamageContextPtr->targetID].stateFlags) & 0x40);
    bool is_short_range = (DamageContextPtr->targetStateFlags & 0x20);

    if (target_row) {
        if (is_short_range) {
            base_damage = base_damage / 2;
        }
    }
    else if(attacker_row) {
        base_damage = base_damage / 2;
    }

    /*defend check*/
    bool is_defending = ((AIActorVariables[DamageContextPtr->attackerID].stateFlags) & 0x20);
    if (is_defending) {
        base_damage = base_damage / 2;
    }

    /*back_attack_handling*/
    bool is_back_attack = (DamageContextPtr->targetStateFlags & 0x01);
    if (is_back_attack) {
        u32 back_damage_mult = AIActorVariables[DamageContextPtr->targetID].backDamageMult;
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
    DamageContextPtr->currentDamage = base_damage;

    /*set damage to 1 for testing purposes*/
    DamageContextPtr->currentDamage = 1;
}
