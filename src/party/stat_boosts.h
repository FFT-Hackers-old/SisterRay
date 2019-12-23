#ifndef STAT_BOOSTS_H
#define STAT_BOOSTS_H

#include <SisterRay/SisterRay.h>
#include "../EncodedString.h"
#include <vector>
#include <unordered_set>

typedef struct {
    u32 sign;
    u16 amount;
    u16 isPercent;
    std::unordered_set<std::string> tags;
} StatBoost;

typedef struct {
    StatBoost statBoost;
    u16 currentDuration;
    u16 totalDuration;
} StatModifier;

//Stats are modular
typedef struct {
    u16 statValue;
    u16 maxValue;
    EncodedString displayName;
    std::vector<StatBoost> boosts; //consumed once on stat calculation, modifies stat value
} SrActorStat;

typedef struct {
    u16 activeValue;
    u16 statValue;
    u16 maxValue;
    std::vector<StatModifier> modifiers; //consumed and decremented on V-Timer in battle, modifies active value
} SrBattleStat;


#endif
