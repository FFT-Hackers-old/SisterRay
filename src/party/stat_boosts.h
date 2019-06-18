#ifndef STAT_BOOSTS_H
#define STAT_BOOSTS_H

#include <SisterRay/SisterRay.h>

SISTERRAY_API ActorStatBoosts createActorStatBoosts();

StatBoost addStatBoosts(StatBoost& a, StatBoost& b);
void addActorBoosts(ActorStatBoosts& accum, ActorStatBoosts& added);

StatBoost& getStatBoostFromID(ActorStatBoosts& boosts, u8 statID);
StatBoost& getStatPenaltyFromID(ActorStatBoosts& boosts, u8 statID);

SISTERRAY_API void setStrBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);
SISTERRAY_API void setVitBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);
SISTERRAY_API void setMagBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);
SISTERRAY_API void setSprBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);
SISTERRAY_API void setDexBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);
SISTERRAY_API void setLuckBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);
SISTERRAY_API void setHPBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);
SISTERRAY_API void setMPBoost(ActorStatBoosts* boost, u16 amount, u16 pcntAmount, u8 sign);

#endif
