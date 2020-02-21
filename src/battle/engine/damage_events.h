#ifndef DAMAGE_EVENTS_H
#define DAMAGE_EVENTS_H

#include <SisterRay/SisterRay.h>
DamageDisplayEvent* getDamageDisplayEvent(u8 queueIndex);
u8 getDisplayTargetID(u8 queueIndex);
DamageDisplayEvent* getFirstTargetDisplayEvent(u8 targetID);

DamageEvent* newDamageEvent();
ImpactEvent* newImpactEvent(DamageEvent* damageEvent);
ImpactEvent* createImpactEvent(DamageEvent* damageEvent, u16 dealtDamage, u16 actionFlags, u16 impactSound, u16 impactEffect, u32 targetHP, u16 targetMP);

#endif // !DAMAGE_EVENTS_H
