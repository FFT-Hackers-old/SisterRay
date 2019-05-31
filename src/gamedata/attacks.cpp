#include "attacks.h"
#include "../impl.h"

#define KERNEL_MAGIC_CUTOFF 56
#define KERNEL_SUMMON_CUTOFF 72
#define KERNEL_ESKILL_CUTOFF 96

SrAttackRegistry::SrAttackRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<SrAttackData, std::string>() {
    size_t read_size;
    SrAttackData attack;
    AttackData baseAttack;
    std::string srAttackID;

    auto idx = 0;
    while (1) {
        read_size = srKernelStreamRead(stream, &baseAttack, sizeof(baseAttack));
        if (read_size != sizeof(baseAttack))
            break;
        attack.attackData = baseAttack;
        attack.attackName = gContext.gameStrings.magic_names.get_resource(idx);
        attack.attackDescription = gContext.gameStrings.magic_descriptions.get_resource(idx);
        if (idx == 96 || idx == 97) {
            attack.attackID = idx - 80;
            srAttackID = assemblekey(CMD_SUMMON, attack.attackID);
            attack.attackID = 0xFFFF;
            attack.animationType = SUMMON;
            attack.attackType = SUMMON;
        }
        else if (idx < KERNEL_MAGIC_CUTOFF) {
            attack.attackID = idx;
            srAttackID = assemblekey(CMD_MAGIC, attack.attackID);;
            attack.animationType = MAGIC;
            attack.attackType = MAGIC;
        }
        else if (idx < KERNEL_SUMMON_CUTOFF) {
            attack.attackID = idx - 56;
            srAttackID = assemblekey(CMD_SUMMON, attack.attackID);
            attack.animationType = SUMMON;
            attack.attackType = SUMMON;
        }
        else if (idx < KERNEL_ESKILL_CUTOFF) {
            attack.attackID = idx - 72;
            srAttackID = assemblekey(CMD_ENEMY_SKILL, attack.attackID);;
            attack.animationType = ENEMY_SKILL;
            attack.attackType = ENEMY_SKILL;
        }
        else {
            attack.attackID  = idx; //Limit indexes are always absolute, and the index is used to dispatch the animation script, so it will not change
            srAttackID = assemblekey(CMD_LIMIT, attack.attackID);;
            attack.animationType = LIMIT;
            attack.attackType = LIMIT;
        }
        add_element(srAttackID, attack);
        ++idx;
    }
}

std::string assemblekey(u8 commandIndex, u16 relativeAttackIndex) {
    std::string prefix;
    switch (commandIndex) {
        case 2:
        case 21: {
            prefix = std::string("MAG");
            break;
        }
        case 20: {
            prefix = std::string("LIM");
        }
        case 13: {
            prefix = std::string("ESK");
            break;
        }
        case 3:
        case 22: {
            prefix = std::string("SUM");
            break;
        }
        case 32:
            prefix = std::string("ETK");
            break;
        default: {
        }
    }
    auto abilityKey = prefix + std::to_string(relativeAttackIndex);
    return abilityKey;
}

SISTERRAY_API void initAttacks(SrKernelStream* stream) {
    gContext.attacks = SrAttackRegistry(stream);
    srLogWrite("kernel.bin: Loaded %lu attacks", (unsigned long)gContext.attacks.resource_count());
}


