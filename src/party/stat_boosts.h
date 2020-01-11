#ifndef STAT_BOOSTS_H
#define STAT_BOOSTS_H

#include <SisterRay/SisterRay.h>
#include "../EncodedString.h"
#include <vector>
#include <map>
#include <unordered_set>

typedef struct {
    u32 sign;
    u16 amount;
    u16 isPercent;
    std::unordered_set<std::string> tags;
} StatBoost;

typedef std::map<std::string, std::vector<StatBoost>> StatBoostModifiers;

typedef struct {
    StatBoost statBoost;
    u16 currentDuration;
    u16 totalDuration;
} StatModifier;

//Stats are modular
typedef struct {
    u32 statValue;
    u32 baseValue; //The value of this stat without any modifiers from gear, materia, etc
    u32 maxValue;
    EncodedString displayName;
} SrActorStat;

typedef struct {
    u32 activeValue;
    u32 statValue;
    u32 maxValue;
    std::vector<StatModifier> modifiers; //consumed and decremented on V-Timer in battle, modifies active value
} SrBattleStat;

void calculateActorStats(SrPartyData& srPartyMember, const CharacterRecord& charRecord, const StatBoostModifiers& statModifiers);


#endif
