#ifndef SOUND_EFFECTS_H
#define SOUND_EFFECTS_H

#include <SisterRay/SisterRay.h>

bool srLoadActionSoundEffect(u16 soundIdx, void* overrideLocation);
void srPlayActionSoundEffect(u32 unk, u8 unk2, u16 soundIdx);
void srFlushActionSoundEffect(u16 soundIdx, u32 unk);
u8 srComputeMixPoint(R3PointWord* basePoint);
#endif // !SOUND_EFFECTS_H
