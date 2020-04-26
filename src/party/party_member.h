#ifndef PARTY_MEMBER_H
#define PARTY_MEMBER_H

#include <SisterRay/SisterRay.h>
#include "battle_stats.h"
#include <array>
#include <unordered_map>
#include <string>


#define MAGIC_COUNT 66
#define SUMMON_COUNT 16
#define ESKILL_COUNT 24
#define AUTO_ACTION_COUNT 10

typedef struct {
    std::unordered_map<std::string, SrActorStat> playerStats;
    std::array<EnabledSpell, MAGIC_COUNT> actorMagics;
    std::array<EnabledSpell, SUMMON_COUNT> actorSummons;
    std::array<EnabledSpell, ESKILL_COUNT> actorEnemySkills;
    std::array<SrAutoAction, AUTO_ACTION_COUNT> actorAutoActions;
    std::string modelName;
    std::array<u8, 10> partySwapBuffer;
    EnabledCommand toggleCommandArray[16];
    bool isLimitActive;
} SrPartyData;

typedef struct {
    PartyMember* gamePartyMember;
    SrPartyData* srPartyMember;
} PartyMemberState;

#endif
