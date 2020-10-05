#include "module.h"

SISTERRAY_API GameModule getActiveGameModule() {
    u16* G_GAME_MODULE = (u16*)0xCBF9DC;
    return (GameModule)*G_GAME_MODULE;
}
