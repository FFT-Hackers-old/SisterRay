#ifndef ACTION_EFFECT_CALLBACKS_H
#define ACTION_EFFECT_CALLBACKS_H

#include <SisterRay/SisterRay.h>

typedef void(*PFNSREFFECTCALLBACK)(void);
void srDisplayActionStringEffect();
void srDispatchMoveEffects(u16 actorID, u8* scriptPtr, u16 moveAmt, PFNSREFFECTCALLBACK moveCallback, u16 frameCount, R3PointWord* moveRelativeBase);
void srDisplayDamageEvents(u32 damagedMask);
void targetReactionMain();
void triggerScreenFlash(u8 red, u8 green, u8 blue);
#endif // !ACTION_EFFECT_CALLBACKS_H
