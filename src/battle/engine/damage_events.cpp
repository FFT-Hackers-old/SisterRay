#include "damage_events.h"

#define G_DAMAGE_DISPLAY_EVENT_QUEUE  ((DamageDisplayEvent*)0xBF2A40)
#define DISPLAY_QUEUE_CAPACITY        78
#define G_DAMAGE_DISPLAY_EVENT_TARGET_IDS ((u8*)0xC05E68)

DamageDisplayEvent* getDamageDisplayEvent(u8 queueIndex) {
    if (queueIndex > DISPLAY_QUEUE_CAPACITY) {
        return nullptr;
    }
    return &(G_DAMAGE_DISPLAY_EVENT_QUEUE[queueIndex]);
}

u8 getDisplayTargetID(u8 queueIndex) {
    if (queueIndex > DISPLAY_QUEUE_CAPACITY) {
        return 0xFF;
    }
    return G_DAMAGE_DISPLAY_EVENT_TARGET_IDS[queueIndex];
}

DamageDisplayEvent* getFirstTargetDisplayEvent(u8 targetID) {
    for (auto i = 0; i < DISPLAY_QUEUE_CAPACITY; i++) {
        if (getDisplayTargetID(i) == targetID) {
            return getDamageDisplayEvent(i);
        }
    }
    return nullptr;
}

#define G_DAMAGE_EVENT_QUEUE          ((DamageEvent*)0x9ACB98)
#define G_CURRENT_DAMAGE_QUEUE_IDX    ((u8*)0x9AEAA0)
DamageEvent* newDamageEvent() {
    DamageEvent* damageEvent = &G_DAMAGE_EVENT_QUEUE[*G_CURRENT_DAMAGE_QUEUE_IDX];
    G_DAMAGE_EVENT_QUEUE[*G_CURRENT_DAMAGE_QUEUE_IDX].impactEventQueueIdx = -1;
    if (*G_CURRENT_DAMAGE_QUEUE_IDX < 128)
        ++*G_CURRENT_DAMAGE_QUEUE_IDX;
    return damageEvent;
}


#define G_IMPACT_EVENT_QUEUE          ((ImpactEvent*)0x9ABA08)
#define G_CURRENT_IMPACT_QUEUE_IDX    ((u8*)0x9AEA98)
ImpactEvent* newImpactEvent(DamageEvent* damageEvent) {
    ImpactEvent* impactEvent = &G_IMPACT_EVENT_QUEUE[*G_CURRENT_IMPACT_QUEUE_IDX];
    damageEvent->impactEventQueueIdx = *G_CURRENT_IMPACT_QUEUE_IDX;
    impactEvent->targetID = damageEvent->targetID;
    if (*G_CURRENT_IMPACT_QUEUE_IDX < 128)
        ++*G_CURRENT_IMPACT_QUEUE_IDX;
    return impactEvent;
}

ImpactEvent* createImpactEvent(DamageEvent* damageEvent, u16 dealtDamage, u16 actionFlags, u16 impactSound, u16 impactEffect, u32 targetHP, u16 targetMP) {
    auto impactEvent = newImpactEvent(damageEvent);
    impactEvent->dealtDamage = dealtDamage;
    impactEvent->actionFlags = actionFlags;
    impactEvent->impactSoundID = impactSound;
    impactEvent->impactEffectID = impactEffect;
    impactEvent->currentTargetHP = targetHP;
    impactEvent->currentTargetMP = targetMP;
    return impactEvent;
}

#define G_ANIMATION_EVENT_QUEUE ((AnimationEvent*)0x9AAD70)
AnimationEvent* getAnimationEvent(u8 queueIndex) {
    if (queueIndex > 64)
        return nullptr;
    return &(G_ANIMATION_EVENT_QUEUE[queueIndex]);
}

#define G_CURRENT_ANIMATION_EVENT_QUEUE_IDX (u8*)0xBF2A38
AnimationEvent* getAnimationEventTop() {
    return getAnimationEvent(*G_CURRENT_ANIMATION_EVENT_QUEUE_IDX);
}

AnimationEvent* newAnimEvent() {
    AnimationEvent* animationEvent = &G_ANIMATION_EVENT_QUEUE[*G_CURRENT_ANIMATION_EVENT_QUEUE_IDX];
    G_ANIMATION_EVENT_QUEUE[*G_CURRENT_ANIMATION_EVENT_QUEUE_IDX].damageEventQueueIdx = *G_CURRENT_DAMAGE_QUEUE_IDX;
    animationEvent->commandIndex = 0;
    animationEvent->spellEffectID = 0;
    if (*G_CURRENT_ANIMATION_EVENT_QUEUE_IDX < 64)
        (*G_CURRENT_ANIMATION_EVENT_QUEUE_IDX)++;
    return animationEvent;
}


AnimationEvent* createAnimEvent(u8 attackerID, u8 activeAllies, u8 animScriptID, u8 commandIdx, u16 actionIdx, u8 spellEffectIdx, u8 specialDamageFlags, u16 cameraData) {
    auto animationEvent = newAnimEvent();
    animationEvent->attackerID = attackerID;
    animationEvent->activeAllies = activeAllies;
    animationEvent->animationScriptID = animScriptID;
    animationEvent->commandIndex = commandIdx;
    animationEvent->spellEffectID = spellEffectIdx;
    animationEvent->cameraData = cameraData;
    animationEvent->actionIndex = actionIdx;
    animationEvent->specialDamageFlags = specialDamageFlags;
    return animationEvent;
}

