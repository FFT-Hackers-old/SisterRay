#ifndef ACTION_SPELL_EFFECTS
#define ACTION_SPELL_EFFECTS

#include <SisterRay/SisterRay.h>

void srBeatRushSpellEffect(u32 currentTargetMask, u32 actorID);
void srBeatSomersaultEffect(u32 currentTargetMask, u32 actorID);
void srWaterkickSpellEffect(u32 currentTargetMask, u32 actorID);
void srMeteodriveSpellEffect(u32 currentTargetMask, u32 actorID);
void srDoplhinBlowSpellEffect(u32 currentTargetMask, u32 actorID);
void srMeteorStrikeSpellEffect(u32 currentTargetMask, u32 actorID);
void srFinalHeavenSpellEffect(u32 currentTargetMask, u32 actorID);


u8 isEffectLoading();
void srLoadAnimationEffect(SRPFNSPELLLOADERCALLBACK loader);
SRPFNSPELLLOADERCALLBACK srGetLoaderCallback(u8 dispatchCommandIdx, u8 relativeSpellEffectID);
void srLoadActionSpellEffects(u8 actorID, u8 commandIdx, u16 actionIdx);
SRPFNSPELLEFFECTCALLBACK srGetDispatchCallback(u8 animationType, u8 animEffectID);
void srDispatchActionSpellEffects(u8 actorID, u8 commandIdx, u16 actionIdx);

#endif
