#ifndef DAMAGE_EVENTS_H
#define DAMAGE_EVENTS_H

#include <SisterRay/SisterRay.h>
struct SrExtendedImpactEvent_ {
    u16 staggerDamage;
};


DamageDisplayEvent* getDamageDisplayEvent(u8 queueIndex);
u8 getDisplayTargetID(u8 queueIndex);
DamageDisplayEvent* getFirstTargetDisplayEvent(u8 targetID);


void initAnimEventQueue();
AnimationEvent* getAnimationEvent(u8 queueIndex);
AnimationEvent* getAnimationEventTop();
void printAnimationEvent(AnimationEvent* animEvent);
void printAnimationEventQueue();


DamageEvent* newDamageEvent();
DamageEvent* getDamageEvent(u8 idx);
void printDamageEvent(DamageEvent* damageEvent);
void printDamageQueueState();


ImpactEvent* newImpactEvent(DamageEvent* damageEvent);
ImpactEvent* createImpactEvent(DamageEvent* damageEvent, u16 dealtDamage, u16 actionFlags, u16 impactSound, u16 impactEffect, u32 targetHP, u16 targetMP);
ImpactEvent* getImpactEvent(u8 impactEffectIdx);
SrExtendedImpactEvent* getExtendedImpactEvent(u8 idx);

AnimationEvent* newAnimEvent();
AnimationEvent* createAnimEvent(u8 attackerID, u8 activeAllies, u8 animScriptID, u8 commandIdx, u16 actionIdx, u8 spellEffectIdx, u8 actionFlags, u16 cameraData);

#endif // !DAMAGE_EVENTS_H
