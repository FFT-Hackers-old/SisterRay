#ifndef ANIMATION_PLAYER_H
#define ANIMATION_PLAYER_H


#include <SisterRay/SisterRay.h>
#include "animation_script_opcode.h"

#define GAME_ANGLE_MAX ((u16)4096)

void srPlayModelAnimation(u8 actorID);
void srHandleAnimateModel(u8 actorID);

i64 srCalculateXVectorComponent(i32 distance);
i64 srCalculateZVectorComponent(i32 distance);
void srComputeEnemyPartyCenter(u16 targetMask, R3PointWord* ret);

#endif // !ANIMATION_PLAYER_H
