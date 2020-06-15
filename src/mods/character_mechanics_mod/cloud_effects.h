#ifndef CLOUD_EFFECTS_H
#define CLOUD_EFFECTS_H

#include <SisterRay/SisterRay.h>
#include <string>
#include <vector>
#include "../../battle/effects/action_effect_textures.h"

typedef struct {
    u16 handlerControl;
    u16 frameCount;
    u16 targetIdx;
    u16 field_6;
    R3PointWord displayPoint;
    u16 field_E;
    u32 field_10;
    u16 field_14;
    u16 field_16;
    u16 scale;
    u16 field_1A;
    u32 field_1C;
} CrossMainEffectCtx;

typedef struct {
    u16 wasDispatchedFrame;
    u16 frameCount;
    u16 field_4;
    u16 field_6;
    R3PointWord displayPoint;
    u16 field_E;
    TextureSPT* effectSPT;
    GameDrawable* effectDrawable;
    u16 scale;
    u16 field_1A;
    u16 field_1C;
    u16 field_1E;
} SlashCtx;


void setupSrCrossSlash(u16 actionTargetMask, u8 casterIdx);
void srNewCrossSlashSetup(u16 targetIdx, u16 casterIdx);
void srCrossSlashMain();
void slashEffectHandler();
#endif
