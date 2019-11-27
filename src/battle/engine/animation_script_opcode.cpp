#include "animation_script_opcode.h"

u16 readOpCodeArg16(u8* scriptPtr, GameAnimationScriptContext ctx) {
    auto argPosition = scriptPtr + 1;
    auto wordReader = (u16*)argPosition;
    auto arg = *argPosition;
    scriptPtr += 2;
    return arg;
}

u16 readOpCodeArg32(u8* scriptPtr, GameAnimationScriptContext ctx) {
    auto argPosition = scriptPtr + 1;
    auto wordReader = (u32*)argPosition;
    auto arg = *argPosition;
    scriptPtr += 4;
    return arg
}

std::string assembleOpCodeKey(u16 opCode) {
    return std::to_string(opCode) + std::string("srff7Base");
}
