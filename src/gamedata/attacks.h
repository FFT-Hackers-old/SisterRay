#ifndef ATTACK_H
#define ATTACK_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"
#include "../EncodedString.h"


typedef enum {
    MAGIC,
    SUMMON,
    ENEMY_SKILL,
    LIMIT,
    ENEMY_ATTACK
} srAttackType;

typedef struct {
    AttackData attackData;
    u16 attackID;
    EncodedString attackName;
    srAttackType attackType; //Either Magic, Summon, Enemy Skill, Limit, Enemy Attack
    srAttackType animationType;
    EncodedString attackDescription;
} SrAttackData;

/*Due to the structure/access patterns for player spells, they need to be contiguous
  Ergo, unlike enemy attacks, we employee a separate registry for additional data, and
  maintain attack typed lookups*/
class SrAttackRegistry : public SrNamedResourceRegistry<SrAttackData, std::string> {
public:
    SrAttackRegistry(SrKernelStream* stream);
    SrAttackRegistry() : SrNamedResourceRegistry<SrAttackData, std::string>() {}
};


std::string assemblekey(u8 commandIndex, u16 relativeAttackIndex);
SISTERRAY_API void initAttacks(SrKernelStream* stream);

#endif
