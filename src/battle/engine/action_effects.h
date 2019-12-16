#ifndef ACTION_EFFECTS_H
#define ACTION_EFFECTS_H

#include <SisterRay/SisterRay.h>
#include "action_effect_callbacks.h"

// This is primarily used for passing arguments through effect callback chains/persisting effect variables between runs
// This is used for the effect pump which has a cap of 60 and the one with a cap of 100
#pragma pack(push, 1)
typedef struct {
    union {
        u16 contol;
        u16 handlerIndex;
    };
    union {
        u8 byteArray[30];
        u16 wordArray[15];
    };
} EffectContext;
#pragma pack(pop)

typedef u8(*PFNSRPUSHEFFECT)(PFNSREFFECTCALLBACK);
u8 srPushEffect60(PFNSREFFECTCALLBACK callback);
u8 srPushEffect100(PFNSREFFECTCALLBACK callback);
u8 srPushEffect10(PFNSREFFECTCALLBACK callback);
EffectContext* getEffectContext60(u16 effectIndex);
EffectContext* getEffectContext100(u16 effectIndex);
EffectContext* getEffectContext10(u16 effectIndex);
u16 getEffect100QueueSize();

typedef enum {
    EFFECT60,
    EFFECT100,
    EFFECT10
} GameEffectType;

EffectContext* srCreateEffect(PFNSREFFECTCALLBACK callback, GameEffectType effectType);

#endif 
