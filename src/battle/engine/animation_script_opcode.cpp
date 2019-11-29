#include "animation_script_opcode.h"

u8 readOpCodeArg8(u8* scriptPtr, GameAnimationScriptContext* ctx, BattleModelState* modelState) {
    auto argPosition = scriptPtr + modelState->currentScriptPosition + 1;
    auto wordReader = (u8*)argPosition;
    auto arg = *argPosition;
    modelState->currentScriptPosition += 1;
    return arg;
}

u16 readOpCodeArg16(u8* scriptPtr, GameAnimationScriptContext* ctx, BattleModelState* modelState) {
    auto argPosition = scriptPtr + modelState->currentScriptPosition + 1;
    auto wordReader = (u16*)argPosition;
    auto arg = *argPosition;
    modelState->currentScriptPosition += 2;
    return arg;
}

u32 readOpCodeArg32(u8* scriptPtr, GameAnimationScriptContext* ctx, BattleModelState* modelState) {
    auto argPosition = scriptPtr + modelState->currentScriptPosition + 1;
    auto wordReader = (u32*)argPosition;
    auto arg = *argPosition;
    modelState->currentScriptPosition += 4;
    return arg;
}

std::string assembleOpCodeKey(u16 opCode) {
    return std::to_string(opCode) + std::string("srff7Base");
}
