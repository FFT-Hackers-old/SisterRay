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
    add_element(assembleOpCodeKey(0x99), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode99);
    add_element(assembleOpCodeKey(0x9A), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode9A);
    add_element(assembleOpCodeKey(0x9B), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode9B);
    add_element(assembleOpCodeKey(0x9C), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode9C);
    add_element(assembleOpCodeKey(0x9D), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode9D);
    add_element(assembleOpCodeKey(0x9E), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode9E);
    add_element(assembleOpCodeKey(0x9F), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode9F);

    add_element(assembleOpCodeKey(0xA0), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA0);
    add_element(assembleOpCodeKey(0xA1), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA1);
    add_element(assembleOpCodeKey(0xA4), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA4);
    add_element(assembleOpCodeKey(0xA5), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA5);
    add_element(assembleOpCodeKey(0xA6), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA6);
    add_element(assembleOpCodeKey(0xA7), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA7);
    add_element(assembleOpCodeKey(0xA8), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA8);
    add_element(assembleOpCodeKey(0xA9), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA9);
    add_element(assembleOpCodeKey(0xAA), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeAA);
    add_element(assembleOpCodeKey(0xAB), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeAB);
    add_element(assembleOpCodeKey(0xAC), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeAC);
    add_element(assembleOpCodeKey(0xAD), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeAD);
    add_element(assembleOpCodeKey(0xAE), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeAE);
    add_element(assembleOpCodeKey(0xAF), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeAF);


    add_element(assembleOpCodeKey(0xB0), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeB0);
    add_element(assembleOpCodeKey(0xB1), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeB1);
    //add_element(assembleOpCodeKey(0xB2), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeB2); This is a NOP
    add_element(assembleOpCodeKey(0xB4), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeB4);
    add_element(assembleOpCodeKey(0xB7), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeB7);
    add_element(assembleOpCodeKey(0xB8), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeB8);
    add_element(assembleOpCodeKey(0xB9), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeB9);
    add_element(assembleOpCodeKey(0xBA), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeBA);
    add_element(assembleOpCodeKey(0xBC), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeBC);
    add_element(assembleOpCodeKey(0xBD), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeBD);
    add_element(assembleOpCodeKey(0xBE), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeBE);
    add_element(assembleOpCodeKey(0xBF), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeBF);

    add_element(assembleOpCodeKey(0xC1), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC1);
    add_element(assembleOpCodeKey(0xC2), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC2);
    add_element(assembleOpCodeKey(0xC3), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC3);
    add_element(assembleOpCodeKey(0xC4), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC4);
    add_element(assembleOpCodeKey(0xC5), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC5);
    add_element(assembleOpCodeKey(0xC6), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC6);
    add_element(assembleOpCodeKey(0xC7), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC7);
    add_element(assembleOpCodeKey(0xC8), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC8);
    //add_element(assembleOpCodeKey(0xC9), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC9); This is a NOP/JD
    add_element(assembleOpCodeKey(0xCA), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeCA);
    add_element(assembleOpCodeKey(0xCC), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeCC);
    //add_element(assembleOpCodeKey(0xCD), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeCD); This is a NOP/JD
    add_element(assembleOpCodeKey(0xCE), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeCE);
    add_element(assembleOpCodeKey(0xCF), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeCF);

    add_element(assembleOpCodeKey(0xD0), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeD0);
    add_element(assembleOpCodeKey(0xD1), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeD1);
    add_element(assembleOpCodeKey(0xD4), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeD4);
    add_element(assembleOpCodeKey(0xD5), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeD5);
    add_element(assembleOpCodeKey(0xD6), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeD6);
    add_element(assembleOpCodeKey(0xD7), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeD7);
    add_element(assembleOpCodeKey(0xD8), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeD8);
    add_element(assembleOpCodeKey(0xDC), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeDC);
    add_element(assembleOpCodeKey(0xDD), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeDD);
    add_element(assembleOpCodeKey(0xDE), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeDE);
    add_element(assembleOpCodeKey(0xDF), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeDF);

    add_element(assembleOpCodeKey(0xE0), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE0);
    add_element(assembleOpCodeKey(0xE1), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE1);
    add_element(assembleOpCodeKey(0xE2), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE2);
    add_element(assembleOpCodeKey(0xE3), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE3);
    add_element(assembleOpCodeKey(0xE4), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE4);
    //add_element(assembleOpCodeKey(0xE5), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE5); 
    add_element(assembleOpCodeKey(0xE6), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE6);
    add_element(assembleOpCodeKey(0xE7), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE7);
    add_element(assembleOpCodeKey(0xE8), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE8);
    add_element(assembleOpCodeKey(0xE9), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE9);
    add_element(assembleOpCodeKey(0xEA), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeEA);
    add_element(assembleOpCodeKey(0xEB), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeEB);
    add_element(assembleOpCodeKey(0xEC), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeEC);
    add_element(assembleOpCodeKey(0xED), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeED);
    add_element(assembleOpCodeKey(0xEE), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeEE);


    add_element(assembleOpCodeKey(0xF0), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF0);
    add_element(assembleOpCodeKey(0xF1), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF1);
    //add_element(assembleOpCodeKey(0xF2), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF2); This is a NOP
    add_element(assembleOpCodeKey(0xF3), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF3);
    add_element(assembleOpCodeKey(0xF4), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF4);
    add_element(assembleOpCodeKey(0xF5), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF5);
    add_element(assembleOpCodeKey(0xF6), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF6);
    add_element(assembleOpCodeKey(0xF7), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF7);
    add_element(assembleOpCodeKey(0xF8), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF8);
    add_element(assembleOpCodeKey(0xF9), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF9);
    add_element(assembleOpCodeKey(0xFA), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeFA);
    add_element(assembleOpCodeKey(0xFB), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode9A);
    //add_element(assembleOpCodeKey(0xFC), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeFC);
    add_element(assembleOpCodeKey(0xFD), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeFD);
    add_element(assembleOpCodeKey(0xFE), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeFE);
    add_element(assembleOpCodeKey(0xFF), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeEE);

}

SISTERRAY_API void initAnimScriptOpCodes() {
    gContext.animScriptOpcodes = SrAnimOpCodeRegistry();
}

u8 readOpCodeArg8(u8* scriptPtr, AnimScriptContext* ctx, BattleModelState* modelState) {
    auto argPosition = scriptPtr + modelState->currentScriptPosition;
    auto wordReader = (u8*)argPosition;
    auto arg = *argPosition;
    modelState->currentScriptPosition += 1;
    return arg;
}

u16 readOpCodeArg16(u8* scriptPtr, AnimScriptContext* ctx, BattleModelState* modelState) {
    auto argPosition = scriptPtr + modelState->currentScriptPosition;
    auto wordReader = (u16*)argPosition;
    auto arg = *argPosition;
    modelState->currentScriptPosition += 2;
    return arg;
}

u32 readOpCodeArg32(u8* scriptPtr, AnimScriptContext* ctx, BattleModelState* modelState) {
    auto argPosition = scriptPtr + modelState->currentScriptPosition;
    auto wordReader = (u32*)argPosition;
    auto arg = *argPosition;
    modelState->currentScriptPosition += 4;
    return arg;
}

std::string assembleOpCodeKey(u16 opCode) {
    return std::to_string(opCode) + std::string("srff7Base");
}
