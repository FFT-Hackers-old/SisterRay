#ifndef ACTION_EFFECT_TYPES_H
#define ACTION_EFFECT_TYPES_H

#include <SisterRay/SisterRay.h>
#include "action_effect_callbacks.h"

#pragma pack(push, 1)
typedef struct {
    u16 handlerControl;
    u16 field_2;
    u16 reactionDelay;
    u16 impactEventQueueIdx;
    u16 damagedActorIdx;
    u16 dealtDamage;
    u16 attackingActorIdx;
    u16 actionFlags;
    u32 impactSoundID;
    u32 impactEffectID;
    u8 field_18;
    u8 damageEventQueueIdx;
    u16 field_1A;
    u16 field_1C;
    u16 field_1E;
} TargetReactionEffectCtx;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u16 handlerControl;
    u16 elapsedFrames;
    u16 framesToDisplay;
    u16 x;
    u16 offsetY;
    u16 y;
    u16 displayOffsetIdx;
    u16 dealtDamage;
    u32 damagedActorIdx;
    u32 actionFlags;
    u16 field_18;
    u16 field_1A;
    u16 field_1C;
    u16 field_1E;
} DamageDisplayEffectCtx;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    u16 handlerControl;
    u16 elapsedFrames;
    u16 displayDelay;
    u16 field_6;
    u16 impactEventQueueIdx;
    u16 damageEventQueueIdx;
    u16 field_C;
    u16 field_A;
    u32 totalBones;
    u32 field_14;
    u16 field_18;
    u16 field_1A;
    u16 field_1C;
    u16 field_1E;
} TriggerDamageEffectCtx;
#pragma pack(pop)
#endif
