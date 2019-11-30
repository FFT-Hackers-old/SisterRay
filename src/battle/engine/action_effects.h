#ifndef ACTION_EFFECTS_H
#define ACTION_EFFECTS_H

#include <SisterRay/SisterRay.h>

typedef void(*PFNSREFFECTCALLBACK)(void);
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

typedef u16(*PFNSRPUSHEFFECT)(PFNSREFFECTCALLBACK);
u16 srPushEffect60(PFNSREFFECTCALLBACK callback);
u16 srPushEffect100(PFNSREFFECTCALLBACK callback);
EffectContext* getEffectContext60(u16 effectIndex);
EffectContext* getEffectContext100(u16 effectIndex);

#endif 
