#ifndef BATTLE_STATS_H
#define BATTLE_STATS_H

#include <SisterRay/SisterRay.h>
#include <map>
#include <vector>
#include <unordered_set>
#include "../EncodedString.h"
#include "../sr_named_registry.h"

typedef struct {
    EncodedString displayName;
    u32 maxValue;
    bool canBeNegative;
}SrStat;

/*The following registries contain enemy data and AI scripts indexed by the absolute ID of the enemy*/
class SrStatRegistry : public SrNamedResourceRegistry<SrStat, std::string> {
public:
    SrStatRegistry();
};

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
    i32 statValue; //The value of the stat factoring in active modifiers
    i32 baseValue; //The value of this stat without any modifiers from gear, materia, etc
    std::vector<StatBoost> boosts; //Always active boosts, like from equipment
} SrActorStat;

typedef struct {
    i32 activeValue;
    i32 statValue;
    std::vector<StatModifier> modifiers; //consumed and decremented on V-Timer in battle, modifies active value
} SrBattleStat;

#endif // !BATTLE_STATS_H
