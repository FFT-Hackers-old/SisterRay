#ifndef IFRIT_H
#define IFRIT_H

#include <SisterRay/SisterRay.h>

void prepareIfrit(u16 actioNTargetMask, u8 casterIdx);
void setupIfritMain(u16 actionTargetMask, u8 casterIdx);
void playSummonAnimationScript();
void srIfritMain();
void descendIfritEffect();
void pushRing();
void srFireWaves();
void prepareHellfire(u16 actioNTargetMask, u8 casterIdx);
void setupHellfireMain(u16 actionTargetMask, u8 casterIdx);
void srHellfireEffectMain();

typedef struct {
    u16 control;
    u16 frameCount;
    R3PointWord displayPoint;
    u16 field_A;
    R3PointWord euler;
    u16 field_12;
    u16 field_14;
    u16 field_16;
    u16 field_18;
    u16 field_1A;
    u16 field_1C;
    u16 field_1E;
} FireRingCtx;

typedef struct {
    u16 control;
    u16 frameCount;
    R3PointWord position;
    u16 field_A;
    R3PointWord velocity;
    u16 field_12;
    u16 identityScale;
    u16 field_16;
    u16 field_18;
    u16 field_1A;
    u16 field_1C;
    u16 field_1E;
} BreathPuffCtx;

typedef struct {
    GameRotationMatrix rotationMatrix;
    R3PointDWord position;
    u16 field_2A;
    u16 field_2C;
    u16 field_2E;
    u32 field_30;
} IfritBreathCtx;
#endif // !IFRIT_H
