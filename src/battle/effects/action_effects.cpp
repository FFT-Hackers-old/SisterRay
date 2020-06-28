#include "action_effects.h"
#include "../../impl.h"


#define gamePushEffect100        ((PFNSRPUSHEFFECT)0x5BEC50)
#define gamePushEffect60         ((PFNSRPUSHEFFECT)0x5BED92)
#define gamePushEffect10         ((PFNSRPUSHEFFECT)0x5BECF1)

u8 srPushEffect60(PFNSREFFECTCALLBACK callback) {
    return gamePushEffect60(callback);
}

u8 srPushEffect100(PFNSREFFECTCALLBACK callback) {
    srLogWrite("PUSHED EFFECT CALLBACK: %x", callback);
    return gamePushEffect100(callback);
}

u8 srPushEffect10(PFNSREFFECTCALLBACK callback) {
    return gamePushEffect10(callback);
}

#define GAME_EFFECT_60_QUEUE     ((EffectContext*)0xBFC3A0)
EffectContext* getEffectContext60(u16 effectIndex) {
    return &(GAME_EFFECT_60_QUEUE[effectIndex]);
}

#define GAME_EFFECT_100_QUEUE     ((EffectContext*)0xBFB718)
EffectContext* getEffectContext100(u16 effectIndex) {
    return &(GAME_EFFECT_100_QUEUE[effectIndex]);
}

u16 getEffect100QueueSize() {
    auto EFFECT100_QUEUE_SIZE = (u16*)0xC05EBC;
    return *EFFECT100_QUEUE_SIZE;
}

EffectContext* getEffect100QueueTop() {
    u16* effect100QueueTop = (u16*)0xBF23B8;
    return getEffectContext100(*effect100QueueTop);
}

EffectContext* getEffect60QueueTop() {
    u16* effect60QueueTop = (u16*)0xBF2DF4;
    return getEffectContext60(*effect60QueueTop);
}


#define GAME_EFFECT_10_QUEUE     ((EffectContext*)0xBF2E70)
EffectContext* getEffectContext10(u16 effectIndex) {
    return &(GAME_EFFECT_10_QUEUE[effectIndex]);
}

EffectContext* srCreateEffect(PFNSREFFECTCALLBACK callback, GameEffectType effectType) {
    if (effectType == EFFECT60) {
        auto effectCtxIdx = srPushEffect60(callback);
        return getEffectContext60(effectCtxIdx);
    }
    if (effectType == EFFECT100) {
        auto effectCtxIdx = srPushEffect100(callback);
        return getEffectContext100(effectCtxIdx);
    }
    if (effectType == EFFECT10) {
        auto effectCtxIdx = srPushEffect10(callback);
        return getEffectContext10(effectCtxIdx);
    }
    return nullptr;
}

void flushSrEffects100() {
    EffectContext* effectQueue = (EffectContext*)0xBFB718;
    u8 size = 100;
    for (auto idx = 0; idx < 100; idx++) {
        effectQueue[idx].handlerIndex = 0xFFFF;
    }
}
