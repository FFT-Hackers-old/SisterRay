#ifndef STAT_BOOSTS_H
#define STAT_BOOSTS_H

#include <SisterRay/SisterRay.h>

typedef struct {
    bool sign;
    u16 statIndex;
    u16 amount;
    u16 percentAmount;
} StatBoost;

/*Equip Effects, weapon stat differences, etc will be an instance of this*/
struct _ActorStatBoosts {
    StatBoost strBoost;
    StatBoost vitBoost;
    StatBoost magBoost;
    StatBoost magDefBoost;
    StatBoost dexBoost;
    StatBoost luckBoost;
    StatBoost maxHPBoost;
    StatBoost maxMPBoost;
    StatBoost coverChance;
    ActorStatBoosts();
};

StatBoost addStatBoosts(StatBoost& a, StatBoost& b);
void addActorBoosts(ActorStatBoosts& accum, ActorStatBoosts& added);

StatBoost& getStatBoostFromID(ActorStatBoosts& boosts, u8 statID);
StatBoost& getStatPenaltyFromID(ActorStatBoosts& boosts, u8 statID);

SISTERRAY_API setStrBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);
SISTERRAY_API setVitBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);
SISTERRAY_API setMagBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);
SISTERRAY_API setSprBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);
SISTERRAY_API setDexBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);
SISTERRAY_API setLuckBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);
SISTERRAY_API setHPBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);
SISTERRAY_API setMPBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);

#endif
