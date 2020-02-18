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
    DamageEvent* damageEvent = &G_DAMAGE_EVENT_QUEUE[G_CURRENT_DAMAGE_QUEUE_IDX];
    G_DAMAGE_EVENT_QUEUE[G_CURRENT_DAMAGE_QUEUE_IDX].impactEventQueueIdx = -1;
    if (G_CURRENT_DAMAGE_QUEUE_IDX < 128)
        ++G_CURRENT_DAMAGE_QUEUE_IDX;
    return damageEvent;
}
