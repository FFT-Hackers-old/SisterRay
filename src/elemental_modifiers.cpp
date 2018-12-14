#include "elemental_modifiers.h"

/*reimplementaiton of elemental modifier mask*/
void ApplyElementalModifers() {
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
