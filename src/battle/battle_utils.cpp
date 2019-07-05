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
