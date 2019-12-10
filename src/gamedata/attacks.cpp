#include "attacks.h"
#include "../impl.h"

#define KERNEL_MAGIC_CUTOFF 56
#define KERNEL_SUMMON_CUTOFF 72
#define KERNEL_ESKILL_CUTOFF 96

SrAttackRegistry::SrAttackRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<SrAttack, std::string>() {
    size_t read_size;
    SrAttack attack;
    AttackData baseAttack;
    std::string srAttackID;
    SrAttack attack96;
    SrAttack attack97;

    auto idx = 0;
    while (1) {
        auto cmdIdx = 0;
        read_size = srKernelStreamRead(stream, &baseAttack, sizeof(baseAttack));
        if (read_size != sizeof(baseAttack))
            break;
        attack.attackData = baseAttack;
        attack.attackName = gContext.gameStrings.magic_names.get_resource(idx);
        attack.attackDescription = gContext.gameStrings.magic_descriptions.get_resource(idx);
        if (idx == 96 || idx == 97) {
            attack.attackID = idx;
            cmdIdx = CMD_SUMMON;
            attack.attackID = 0xFFFF;
            attack.animationType = SUMMON;
            attack.attackType = SUMMON;
            attack.animScriptIndex = 0xFFFF;
            attack.useOverride = 0;
            attack.useMulti = 0;
            if (idx == 96)
                attack96 = attack;
            if (idx == 97)
                attack97 = attack;
            continue;
        }
        else if (idx < KERNEL_MAGIC_CUTOFF) {
            attack.attackID = idx;
            cmdIdx = CMD_MAGIC;
            attack.animationType = MAGIC;
            attack.attackType = MAGIC;
            attack.animScriptIndex = 0xFFFF;
            attack.useOverride = 0;
            attack.useMulti = getDefaultMagicUseMulti(idx);
            attack.multiEffect = getDefaultMagicMultiEffects(idx);
        }
        else if (idx < KERNEL_SUMMON_CUTOFF) {
            attack.attackName = gContext.gameStrings.summon_attack_names.get_resource(idx);
            attack.attackID = idx;
            cmdIdx = CMD_SUMMON;
            attack.animationType = SUMMON;
            attack.attackType = SUMMON;
            attack.animScriptIndex = 0xFFFF;
            attack.useOverride = 0;
            attack.useMulti = 0;
        }
        else if (idx < KERNEL_ESKILL_CUTOFF) {
            attack.attackID = idx;
            cmdIdx = CMD_ENEMY_SKILL;
            attack.animationType = ENEMY_SKILL;
            attack.attackType = ENEMY_SKILL;
            attack.animScriptIndex = 0xFFFF;
            attack.useOverride = 0;
            attack.useMulti = 0;
        }
        else {
            attack.attackID = idx;
            cmdIdx = CMD_LIMIT;
            attack.animationType = LIMIT;
            attack.attackType = LIMIT;
            attack.animScriptIndex = idx;
            attack.useOverride = 0;
            attack.useMulti = 0;
        }
        add_element(assembleGDataKey(attack.attackID), attack);
        addCommandAction(assembleGDataKey(cmdIdx), assembleGDataKey(attack.attackID));
        ++idx;
    }
    // Init gunge lance and fat chocobo
    add_element(assembleGDataKey(attack96.attackID), attack96);
    addCommandAction(assembleGDataKey(CMD_SUMMON), assembleGDataKey(attack96.attackID));
    add_element(assembleGDataKey(attack97.attackID), attack96);
    addCommandAction(assembleGDataKey(CMD_SUMMON), assembleGDataKey(attack97.attackID));
}

void initAttacks(SrKernelStream* stream) {
    gContext.attacks = SrAttackRegistry(stream);
    srLogWrite("kernel.bin: Loaded %lu attacks", (unsigned long)gContext.attacks.resource_count());
}

 u16 getDefaultMagicUseMulti(u16 actionID) {
     switch (actionID) {
     case 0x1D:
     case 0x20:
     case 0x23:
     case 0x29:
     case 0x2C: {
         return 1;
         break;
     }
     default: {
         return 0;
         break;
     }
     }
}

 SpellEffect getDefaultMagicMultiEffects(u16 actionID) {
     auto multiEffect = SpellEffect();
     switch (actionID) {
     case 0x1D: {
         multiEffect.dispatchCallback = (SRPFNSPELLEFFECTCALLBACK)0x56D910;
         multiEffect.loadCallback = (SRPFNSPELLLOADERCALLBACK)0x56D880;
         break;
     }
     case 0x20: {
         multiEffect.dispatchCallback = (SRPFNSPELLEFFECTCALLBACK)0x49B0D0;
         multiEffect.loadCallback = (SRPFNSPELLLOADERCALLBACK)0x49B040;
         break;
     }
     case 0x23: {
         multiEffect.dispatchCallback = (SRPFNSPELLEFFECTCALLBACK)0x5B6930;
         multiEffect.loadCallback = (SRPFNSPELLLOADERCALLBACK)0x5B68A0;
         break;
     }
     case 0x29: {
         multiEffect.dispatchCallback = (SRPFNSPELLEFFECTCALLBACK)0x5A2650;
         multiEffect.loadCallback = (SRPFNSPELLLOADERCALLBACK)0x5A2570;
         break;
     }
     case 0x2C: {
         multiEffect.dispatchCallback = (SRPFNSPELLEFFECTCALLBACK)0x5A7E60;
         multiEffect.loadCallback = (SRPFNSPELLLOADERCALLBACK)0x5A7DD0;
     }
     default: {
         break;
     }
     }
     return multiEffect;
 }


