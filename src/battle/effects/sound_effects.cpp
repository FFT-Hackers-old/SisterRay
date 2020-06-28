#include "sound_effects.h"


typedef bool(*PFNSR_GAMELOADSOUNDEFFECT)(u16, void*);
static const auto gameLoadActionSoundEffect = ((PFNSR_GAMELOADSOUNDEFFECT)0x745CF3);
bool srLoadActionSoundEffect(u16 soundIdx, void* overrideLocation) {
    return gameLoadActionSoundEffect(soundIdx, overrideLocation);
}


typedef void(*PFNSR_GAMEPLAYSOUNDEFFECT)(u32, u8, u16);
static const auto gamePlayActionSoundEffect = ((PFNSR_GAMEPLAYSOUNDEFFECT)0x5BFFF3);
void srPlayActionSoundEffect(u32 unk, u8 unk2, u16 soundIdx) {
    gamePlayActionSoundEffect(unk, unk2, soundIdx);
}


typedef void(*PFNSR_GAMERELEASESOUNDEFFECT)(u16, u32);
static const auto gameReleaseActionSoundEffect = ((PFNSR_GAMERELEASESOUNDEFFECT)0x745DBB);
void srFlushActionSoundEffect(u16 soundIdx, u32 unk) {
    gameReleaseActionSoundEffect(soundIdx, unk);
}


typedef u8(*PFNSR_COMPUTEMIXPOINT)(R3PointWord*);
static const auto gameComputeMixPoint = (PFNSR_COMPUTEMIXPOINT)0x5C00C5;
u8 srComputeMixPoint(R3PointWord* basePoint) {
    return gameComputeMixPoint(basePoint);
}
