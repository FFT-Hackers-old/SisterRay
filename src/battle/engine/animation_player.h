#ifndef ANIMATION_PLAYER_H
#define ANIMATION_PLAYER_H


#include <SisterRay/SisterRay.h>
#include "animation_script_opcode.h"

#define GAME_ANGLE_MAX 4096

void srPlayModelAnimation(u16 actorID);
void srHandleAnimateModel(u16 actorID);

AnimationEvent* getAnimationEvent(u8 queueIndex);
AnimationEvent* getAnimationEventTop();

i64 srCalculateXVectorComponent(i32 distance);
i64 srCalculateZVectorComponent(i32 distance);

#endif // !ANIMATION_PLAYER_H
