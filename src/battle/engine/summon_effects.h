#ifndef SUMMON_EFFECTS_H
#define SUMMON_EFFECTS_H

#include <SisterRay/SisterRay.h>
#include "action_effects.h"
#include <string>

std::string getSummonModelNameFromEffectIdx(u8 animationEffectIdx);
void summonEffectMainDispatcher(u16 actionTargetMask, u8 casterIdx);
void summonMainEffect();
void pushSummonEffectCallback(u8 animationEffectIdx, u16 targetMask, u8 actorIdx);
void prepareIfrit(u16 actioNTargetMask, u8 casterIdx);
void setupIfritMain(u16 actionTargetMask, u8 casterIdx);
void srIfritMain();
#endif // !SUMMON_EFFECTS_H
