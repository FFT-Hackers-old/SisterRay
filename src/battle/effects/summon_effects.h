#ifndef SUMMON_EFFECTS_H
#define SUMMON_EFFECTS_H

#include <SisterRay/SisterRay.h>
#include <string>

std::string getSummonModelNameFromEffectIdx(u8 animationEffectIdx);
void summonEffectMainDispatcher(u16 actionTargetMask, u8 casterIdx);
void endSummonHandler();
void summonMainEffect();
void pushSummonEffectCallback(u8 animationEffectIdx, u16 targetMask, u8 actorIdx);
void playSummonAnimationScript();
std::string getSummonModelNameFromEffectIdx(u8 animationEffectIdx);

#endif // !SUMMON_EFFECTS_H
