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
    i32 maxValue;
    i32 minValue;
    bool canBeNegative;
}SrStat;

/*The following registries contain enemy data and AI scripts indexed by the absolute ID of the enemy*/
class SrStatRegistry : public SrNamedResourceRegistry<SrStat, std::string> {
public:

    SrStatRegistry() : SrNamedResourceRegistry<SrStat, std::string>() {}
    explicit SrStatRegistry(bool initResistances);
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

//Static Stats only need a value, they are used for setting fixed properties of data resources
typedef struct {
    i32 statValue;
} SrStaticStat;

//Actors and Enemies have actor stats, have a base value and set of boosts which are applied at stat calculation time. 
typedef struct {
    i32 statValue; //The value of the stat factoring in active modifiers
    i32 baseValue; //The value of this stat without any modifiers from gear, materia, etc
    std::vector<StatBoost> boosts; //Always active boosts, like from equipment
} SrBoostedStat;


typedef struct {
    i32 newValue;
    i32 previousValue;
    i32 incrementTargetValue;
    i32 barDisplayValue;
} resourceIncrement;


typedef struct {
    i32 activeValue;
    i32 statValue;
    resourceIncrement incrementCtx;
    std::vector<StatModifier> modifiers; //consumed and decremented on V-Timer in battle, modifies active value
} SrBattleStat;


void initStats(bool initResistances);
#endif // !BATTLE_STATS_H
