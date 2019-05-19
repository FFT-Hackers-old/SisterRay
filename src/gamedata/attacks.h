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
    u16 attackID;
    srAttackType attackType; //Either Magic, Summon, Enemy Skill, Limit, Enemy Attack
    u8 animationType;
} SrExtendedAttackData;

/*Due to the structure/access patterns for player spells, they need to be contiguous
  Ergo, unlike enemy attacks, we employee a separate registry for additional data, and
  maintain attack typed lookups*/
class SrPlayerAttackRegistry : public SrNamedResourceRegistry<AttackData, std::string> {
public:
    SrPlayerAttackRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<AttackData, std::string>(stream) {}
    SrPlayerAttackRegistry() : SrNamedResourceRegistry<AttackData, std::string>() {}
protected:
    std::vector<u16> magicIndexLookup;
};

#endif
