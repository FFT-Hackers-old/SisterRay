#ifndef BATTLE_UTILS_H
#define BATTLE_UTILS_H

#include <SisterRay/SisterRay.h>

u8 countTargets(u16 targetMask);
u8 getBattleType();
u8 getTargetAllActive();
u16 getAnimatingActionTargetMask();
u8 getAnimatingActionTargetIdx();
u8 getActionActorIdx();
u8 getActorIdleAnimScript(u8 actorID);
void setPoint(R3PointWord* point, i16 x, i16 y, i16 z);
void setPoint(R3Point* point, float x, float y, float z);
void nullGameMatrix(GameRotationMatrix* matrix);
void setDiagonal(GameRotationMatrix* matrix, i16 e11, i16 e22, i16 e33);
void computeTargetsCentroid(u16 targetMask, R3PointWord* ret);
GameRotationMatrix* setAnimationTransitionState(u32 tranistionState);

#endif
