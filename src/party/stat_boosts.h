#ifndef STAT_BOOSTS_H
#define STAT_BOOSTS_H

#include <SisterRay/SisterRay.h>

typedef struct {
    u16 statIndex;
    u16 amount;
    u16 percentAmount;
} StatBoost;

/*Equip Effects exist in this form*/
typedef struct ActorStatBoosts {
    StatBoost strBoost;
    StatBoost strPenalty;
    StatBoost vitBoost;
    StatBoost vitPenalty;
    StatBoost magBoost;
    StatBoost magPenalty;
    StatBoost magDefBoost;
    StatBoost magDefPenalty;
    StatBoost dexBoost;
    StatBoost dexPenalty;
    StatBoost luckBoost;
    StatBoost luckPenalty;
    StatBoost maxHPBoost;
    StatBoost maxHPPenalty;
    StatBoost maxMPBoost;
    StatBoost maxMPPenalty;
    StatBoost coverChance;
    ActorStatBoosts();
} ActorStatBoosts;


StatBoost addStatBoosts(StatBoost& a, StatBoost& b);
void addActorBoosts(ActorStatBoosts& accum, ActorStatBoosts& added);

#endif
