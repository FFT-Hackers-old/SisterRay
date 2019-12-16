#include "action_effect_callbacks.h"

#define displayActionStringEffect       ((PFNSREFFECTCALLBACK)0x42782A)
void srDisplayActionStringEffect() {
    displayActionStringEffect();
}

typedef void(*SRPFNSUB42519A)(u16, u8*, u16, PFNSREFFECTCALLBACK, u16, R3PointWord*);
#define gameDispatchActorMoveEffects    ((SRPFNSUB42519A)0x42519A)

void srDispatchMoveEffects(u16 actorID, u8* scriptPtr, u16 moveAmt, PFNSREFFECTCALLBACK moveCallback, u16 frameCount, R3PointWord* moveRelativeBase) {
    gameDispatchActorMoveEffects(actorID, scriptPtr, moveAmt, moveCallback, frameCount, moveRelativeBase);
}


typedef void(*SRPFNSUB5BFEC9)(u16, ModelPosition*);
#define gameComputeMultitargetMoveBase    ((SRPFNSUB5BFEC9)0x5BFEC9)

void srComputeMultitargetMoveBase(u16 targetMask, ModelPosition* ret) {
    gameComputeMultitargetMoveBase(targetMask, ret);
}
