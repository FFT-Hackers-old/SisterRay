#ifndef ACTION_EFFECT_CALLBACKS_H
#define ACTION_EFFECT_CALLBACKS_H

#include <SisterRay/SisterRay.h>

typedef void(*PFNSREFFECTCALLBACK)(void);
void srDisplayActionStringEffect();
void srComputeMultitargetMoveBase(u16 targetMask, ModelPosition* ret);
void srDispatchMoveEffects(u16 actorID, u8* scriptPtr, u16 moveAmt, PFNSREFFECTCALLBACK moveCallback, u16 frameCount, ModelPosition* moveRelativeBase);
#endif // !ACTION_EFFECT_CALLBACKS_H
