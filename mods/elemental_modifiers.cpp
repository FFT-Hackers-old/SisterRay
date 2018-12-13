#include <elemental_modifiers.h>

/*reimplementaiton of elemental modifier mask*/
void ApplyElementalModifers() {
    elm_def_mask = (*DamageContextPtr).elemental_defense_mask

    /*handle absorb*/
    if (elm_def_mask & 0x40) {
        DamageContextPtr->byte220h = DamageContextPtr->byte220h ^ 0x0001);
    }
    else {
        /*handle weakness*/
        if (elm_def_mask & 0x04) {
            DamageContextPtr->current_damage = (DamageContextPtr->current_damage) * 2;
        }
        /*handle resistance*/
        if (elm_def_mask & 0x10) {
            DamageContextPtr->current_damage = (DamageContextPtr->current_damage + 1) / 2;
        }
    }

    /*handle_death*/
    if (elm_def_mask & 0x01) {
        if (DamageContextPtr->targetStatusMask & 0x01){
            DamageContextPtr->attackFlags1= ((DamageContextPtr->attackFlags1) | 3));
            nullMasks();
        }
        else {
            /*handle the flags*/
            DamageContextPtr -> attackFlags1 = ((DamageContextPtr -> attackFlags1) & 0xfffffffd);
            DamageContextPtr -> attackFlags2 = ((DamageContextPtr -> attackFlags2 & 0xfffffffe);
            DamageContextPtr -> addStatusMask = ((DamageContextPtr -> addStatusMask) | 0x01);
            DamageContextPtr -> rmStatusMask = ((DamageContextPtr -> rmStatusMask) & 0xfffffffe)
            DamageContextPtr -> killRecoverDamage = 0xfffffffe;
        }
    }
    /*handle_recovery*/
    else if (elm_def_mask & 80) {
        target_id = DamageContextPtr->target_id;
        DamageContextPtr->currentHP = DamageContextPtr->maxHP;

        DamageContextPtr->currentMP = DamageContextPtr->maxMP;

        /*handle le flags*/
        DamageContextPtr->attackFlags1 = ((DamageContextPtr->attackFlags1) & 0xfffffffd);
        DamageContextPtr->attackFlags2 = 0x01;
        DamageContextPtr->addStatusMask = ((DamageContextPtr->addStatusMask) & 0xfffffffe);
        DamageContextPtr->killRecoverDamage = 0xfffffffd;
    }
    else {
        /*handle nullification*/
        if (elm_def_mask & 0x20){
            if ((DamageContextPtr->combinedStatusMask) != 0x00 || (DamageContextPtr -> actionElementMask) & 0x01) != 0) {
                DamageContextPtr->attackFlags1 = ((DamageContextPtr->attackFlags1) | 00000001);
            }
            nullMasks();
        }
    }
}
