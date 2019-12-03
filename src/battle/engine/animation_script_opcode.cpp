#include "animation_script_opcode.h"
#include "animation_script_opc_handlers.h"
#include "../../impl.h"

SrAnimOpCodeRegistry::SrAnimOpCodeRegistry() : SrNamedResourceRegistry<SRPFNANMSCRIPTOPCODEHANDLER, std::string>() {
    add_element(assembleOpCodeKey(0x8E), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode8E);
    add_element(assembleOpCodeKey(0x8F), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode8F);
    add_element(assembleOpCodeKey(0x90), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode90);
    add_element(assembleOpCodeKey(0x91), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode91);
    add_element(assembleOpCodeKey(0x92), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode92);
    add_element(assembleOpCodeKey(0x93), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode93);
    add_element(assembleOpCodeKey(0x94), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode94);
    add_element(assembleOpCodeKey(0x95), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode95);
    add_element(assembleOpCodeKey(0x96), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode96);
    add_element(assembleOpCodeKey(0x97), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode97);
    add_element(assembleOpCodeKey(0x98), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode98);
}

SISTERRAY_API void initAnimScriptOpCodes() {
    gContext.animScriptOpcodes = SrAnimOpCodeRegistry();
}

u8 readOpCodeArg8(u8* scriptPtr, AnimScriptContext* ctx, BattleModelState* modelState) {
    auto argPosition = scriptPtr + modelState->currentScriptPosition + 1;
    auto wordReader = (u8*)argPosition;
    auto arg = *argPosition;
    modelState->currentScriptPosition += 1;
    return arg;
}

u16 readOpCodeArg16(u8* scriptPtr, AnimScriptContext* ctx, BattleModelState* modelState) {
    auto argPosition = scriptPtr + modelState->currentScriptPosition + 1;
    auto wordReader = (u16*)argPosition;
    auto arg = *argPosition;
    modelState->currentScriptPosition += 2;
    return arg;
}

u32 readOpCodeArg32(u8* scriptPtr, AnimScriptContext* ctx, BattleModelState* modelState) {
    auto argPosition = scriptPtr + modelState->currentScriptPosition + 1;
    auto wordReader = (u32*)argPosition;
    auto arg = *argPosition;
    modelState->currentScriptPosition += 4;
    return arg;
}

std::string assembleOpCodeKey(u16 opCode) {
    return std::to_string(opCode) + std::string("srff7Base");
}
