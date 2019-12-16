#ifndef DAMAGE_EVENTS_H
#define DAMAGE_EVENTS_H

#include <SisterRay/SisterRay.h>
DamageDisplayEvent* getDamageDisplayEvent(u8 queueIndex);
u8 getDisplayTargetID(u8 queueIndex);
DamageDisplayEvent* getFirstTargetDisplayEvent(u8 targetID);

#endif // !DAMAGE_EVENTS_H
