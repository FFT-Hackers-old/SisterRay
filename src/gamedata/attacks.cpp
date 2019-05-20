#include "attacks.h"
#include "../impl.h"

#define KERNEL_MAGIC_CUTOFF 56
#define KERNEL_SUMMON_CUTOFF 72
#define KERNEL_ESKILL_CUTOFF 95

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
            srAttackID = std::string("SPC") + std::to_string(attack.attackID);
            attack.attackID = 0xFFFF;
            attack.animationType = SUMMON;
            attack.attackType = SUMMON;
        }
        else if (idx < KERNEL_MAGIC_CUTOFF) {
            attack.attackID = idx;
            srAttackID = std::string("MAG") + std::to_string(attack.attackID);
            attack.animationType = MAGIC;
            attack.attackType = MAGIC;
        }
        else if (idx < KERNEL_SUMMON_CUTOFF) {
            attack.attackID = idx - 56;
            srAttackID = std::string("SUM") + std::to_string(attack.attackID);
            attack.animationType = SUMMON;
            attack.attackType = SUMMON;
        }
        else if (idx < KERNEL_ESKILL_CUTOFF) {
            attack.attackID = idx - 72;
            srAttackID = std::string("ESK") + std::to_string(attack.attackID);
            attack.animationType = ENEMY_SKILL;
            attack.attackType = ENEMY_SKILL;
        }
        else {
            attack.attackID - idx - 98;
            srAttackID = std::string("LIM") + std::to_string(attack.attackID);
            attack.animationType = LIMIT;
            attack.attackType = LIMIT;
        }
        add_element(std::string(srAttackID), attack);
        ++idx;
    }
}

SISTERRAY_API void initAttacks(SrKernelStream* stream) {
    gContext.attacks = SrAttackRegistry(stream);
    srLogWrite("kernel.bin: Loaded %lu attacks", (unsigned long)gContext.attacks.resource_count());
}
