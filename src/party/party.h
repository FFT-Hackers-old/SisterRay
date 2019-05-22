#ifndef PARTY_H
#define PARTY_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include <array>

#define MAGIC_COUNT 64
#define SUMMON_COUNT 16
#define ESKILL_COUNT 24

typedef struct {
    std::array<EnabledSpell, MAGIC_COUNT> actorMagics;
    std::array<EnabledSpell, SUMMON_COUNT> actorSummons;
    std::array<EnabledSpell, ESKILL_COUNT> actorEnemySkills;
} SrPartyData;

/*Holds extensible Enabled command arrays for active party members
  Will extend to hold data for all party members later to facilitate character swapping*/
class SrPartyDataRegistry : public SrNamedResourceRegistry<SrPartyData, std::string> {
public:
    SrPartyDataRegistry();
};


void srUpdatePartyMember(u32 partyIndex);

#endif
