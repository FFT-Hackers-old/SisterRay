#include "battle_engine_interface.h"


u16 getDexNormalization() {
    u16* G_DEX_NORMALIZATION = (u16*)0x9AAD00;
    return *G_DEX_NORMALIZATION;
}

void setBattleSpeed(u8 speedFactor) {
    u16* G_BATTLE_SPEED = (u16*)0x9AACFE;
    *G_BATTLE_SPEED = 0x10000 / (2 * (480 * speedFactor / 256 + 120));
}

u16 getBattleSpeed() {
    u16* G_BATTLE_SPEED = (u16*)0x9AACFE;
    return *G_BATTLE_SPEED;
}
