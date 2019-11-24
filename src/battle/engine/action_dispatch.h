#ifndef ACTION_DISPATCH_H
#define ACTION_DISPATCH_H

#include <SisterRay/SisterRay.h>

void dispatchBattleAction(u8 targetingType, u16 actorID, u16 commandID, u16 actionID, u16 targetMask, u16 targetIndex, u8 actionRow);
void dispatchChosenBattleAction();
void dispatchFirstWAction();

#endif 
