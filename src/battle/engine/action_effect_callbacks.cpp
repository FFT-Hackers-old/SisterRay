#include "action_effect_callbacks.h"
#include "../battle_utils.h"
#include "../../impl.h"
#include "action_effects.h"

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


typedef GameRotationMatrix*(*SRPFN_SETDISPLAYDAMAGE)(u8);
#define sub_5C0158     ((SRPFN_SETDISPLAYDAMAGE)0x5C0158)
void srDisplayDamageEvents(u32 damagedMask) {
    srLogWrite("Dispatch ERUPTION Events State: %x", damagedMask);
    for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
        if ((1 << actorIdx) & damagedMask) {
            srLogWrite("DISPATCHING A DAMAGE EVENT FOR ACTOR actorIdx");
            sub_5C0158(actorIdx);
        }
    }
}
