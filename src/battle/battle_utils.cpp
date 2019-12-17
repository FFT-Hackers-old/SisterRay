#include "battle_utils.h"

u8 countTargets(u16 targetMask) {
    u8 count = 0;
    while (targetMask) {
        if (targetMask & 1)
            ++count;
        targetMask >>= 1;
    }
    return count;
}

u8 getBattleType() {
    u8* G_BATTLE_TYPE = (u8*)0xBFD0E0;
    return *G_BATTLE_TYPE;
}

u8 getTargetAllActive() {
    u8* G_TARGETING_ALL = (u8*)0xBF2E1C;
    return *G_TARGETING_ALL;
}

u16 getAnimatingActionTargetMask() {
    u16* G_TARGET_MASK = (u16*)0xBFD0F8;
    return *G_TARGET_MASK;
}


u8 getAnimatingActionTargetIdx() {
    u8* G_TARGET_IDX = (u8*)0xBFB198;
    return *G_TARGET_IDX;
}

u8 getActionActorIdx() {
    u8* G_ACTIVE_ACTOR = (u8*)0xBE1170;
    return *G_ACTIVE_ACTOR;
}

u8 getActorIdleAnimScript(u8 actorID) {
    u8* G_ACTOR_IDLE_SCRIPTS = (u8*)0xBF2DF8;
    return G_ACTOR_IDLE_SCRIPTS[actorID];
}
