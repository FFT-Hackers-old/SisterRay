#ifndef BATTLE_STATS_H
#define BATTLE_STATS_H

#include <SisterRay/SisterRay.h>
#include <map>
#include <vector>
#include <unordered_set>
#include "../EncodedString.h"

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

//Stats are modular. 
typedef struct {
    u32 statValue; //The value of the stat factoring in active modifiers
    u32 baseValue; //The value of this stat without any modifiers from gear, materia, etc
    u32 maxValue;
    EncodedString displayName;
    std::vector<StatBoost> boosts; //Always active boosts, like from equipment
} SrActorStat;

typedef struct {
    u32 activeValue;
    u32 statValue;
    u32 maxValue;
    std::vector<StatModifier> modifiers; //consumed and decremented on V-Timer in battle, modifies active value
} SrBattleStat;

#endif // !BATTLE_STATS_H
