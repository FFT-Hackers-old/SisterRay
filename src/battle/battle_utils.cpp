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
