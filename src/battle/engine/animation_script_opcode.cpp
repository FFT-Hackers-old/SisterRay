#include "animation_script_opcode.h"
#include "animation_script_opc_handlers.h"
#include "../../impl.h"

SrAnimOpCodeRegistry::SrAnimOpCodeRegistry() : SrNamedResourceRegistry<SRPFNANMSCRIPTOPCODEHANDLER, std::string>() {
    addElement(assembleOpCodeKey(0x8E), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode8E);
    addElement(assembleOpCodeKey(0x8F), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode8F);
    addElement(assembleOpCodeKey(0x90), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode90);
    addElement(assembleOpCodeKey(0x91), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode91);
    addElement(assembleOpCodeKey(0x92), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode92);
    addElement(assembleOpCodeKey(0x93), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode93);
    addElement(assembleOpCodeKey(0x94), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode94);
    addElement(assembleOpCodeKey(0x95), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode95);
    addElement(assembleOpCodeKey(0x96), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode96);
    addElement(assembleOpCodeKey(0x97), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode97);
    addElement(assembleOpCodeKey(0x98), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode98);
    addElement(assembleOpCodeKey(0x99), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode99);
    addElement(assembleOpCodeKey(0x9A), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode9A);
    addElement(assembleOpCodeKey(0x9B), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode9B);
    addElement(assembleOpCodeKey(0x9C), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode9C);
    addElement(assembleOpCodeKey(0x9D), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode9D);
    addElement(assembleOpCodeKey(0x9E), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode9E);
    addElement(assembleOpCodeKey(0x9F), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode9F);

    addElement(assembleOpCodeKey(0xA0), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA0);
    addElement(assembleOpCodeKey(0xA1), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA1);
    addElement(assembleOpCodeKey(0xA4), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA4);
    addElement(assembleOpCodeKey(0xA5), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA5);
    addElement(assembleOpCodeKey(0xA6), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA6);
    addElement(assembleOpCodeKey(0xA7), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA7);
    addElement(assembleOpCodeKey(0xA8), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA8);
    addElement(assembleOpCodeKey(0xA9), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeA9);
    addElement(assembleOpCodeKey(0xAA), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeAA);
    addElement(assembleOpCodeKey(0xAB), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeAB);
    addElement(assembleOpCodeKey(0xAC), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeAC);
    addElement(assembleOpCodeKey(0xAD), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeAD);
    addElement(assembleOpCodeKey(0xAE), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeAE);
    addElement(assembleOpCodeKey(0xAF), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeAF);


    addElement(assembleOpCodeKey(0xB0), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeB0);
    addElement(assembleOpCodeKey(0xB1), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeB1);
    //addElement(assembleOpCodeKey(0xB2), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeB2); This is a NOP
    addElement(assembleOpCodeKey(0xB4), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeB4);
    addElement(assembleOpCodeKey(0xB7), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeB7);
    addElement(assembleOpCodeKey(0xB8), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeB8);
    addElement(assembleOpCodeKey(0xB9), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeB9);
    addElement(assembleOpCodeKey(0xBA), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeBA);
    addElement(assembleOpCodeKey(0xBC), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeBC);
    addElement(assembleOpCodeKey(0xBD), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeBD);
    addElement(assembleOpCodeKey(0xBE), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeBE);
    addElement(assembleOpCodeKey(0xBF), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeBF);

    addElement(assembleOpCodeKey(0xC1), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC1);
    addElement(assembleOpCodeKey(0xC2), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC2);
    addElement(assembleOpCodeKey(0xC3), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC3);
    addElement(assembleOpCodeKey(0xC4), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC4);
    addElement(assembleOpCodeKey(0xC5), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC5);
    addElement(assembleOpCodeKey(0xC6), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC6);
    addElement(assembleOpCodeKey(0xC7), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC7);
    addElement(assembleOpCodeKey(0xC8), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC8);
    //addElement(assembleOpCodeKey(0xC9), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeC9); This is a NOP/JD
    addElement(assembleOpCodeKey(0xCA), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeCA);
    addElement(assembleOpCodeKey(0xCC), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeCC);
    //addElement(assembleOpCodeKey(0xCD), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeCD); This is a NOP/JD
    addElement(assembleOpCodeKey(0xCE), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeCE);
    addElement(assembleOpCodeKey(0xCF), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeCF);

    addElement(assembleOpCodeKey(0xD0), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeD0);
    addElement(assembleOpCodeKey(0xD1), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeD1);
    addElement(assembleOpCodeKey(0xD4), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeD4);
    addElement(assembleOpCodeKey(0xD5), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeD5);
    addElement(assembleOpCodeKey(0xD6), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeD6);
    addElement(assembleOpCodeKey(0xD7), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeD7);
    addElement(assembleOpCodeKey(0xD8), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeD8);
    addElement(assembleOpCodeKey(0xDC), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeDC);
    addElement(assembleOpCodeKey(0xDD), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeDD);
    addElement(assembleOpCodeKey(0xDE), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeDE);
    addElement(assembleOpCodeKey(0xDF), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeDF);

    addElement(assembleOpCodeKey(0xE0), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE0);
    addElement(assembleOpCodeKey(0xE1), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE1);
    addElement(assembleOpCodeKey(0xE2), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE2);
    addElement(assembleOpCodeKey(0xE3), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE3);
    addElement(assembleOpCodeKey(0xE4), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE4);
    //addElement(assembleOpCodeKey(0xE5), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE5); 
    addElement(assembleOpCodeKey(0xE6), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE6);
    addElement(assembleOpCodeKey(0xE7), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE7);
    addElement(assembleOpCodeKey(0xE8), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE8);
    addElement(assembleOpCodeKey(0xE9), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeE9);
    addElement(assembleOpCodeKey(0xEA), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeEA);
    addElement(assembleOpCodeKey(0xEB), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeEB);
    addElement(assembleOpCodeKey(0xEC), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeEC);
    addElement(assembleOpCodeKey(0xED), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeED);
    addElement(assembleOpCodeKey(0xEE), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeEE);


    addElement(assembleOpCodeKey(0xF0), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF0);
    addElement(assembleOpCodeKey(0xF1), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF1);
    //addElement(assembleOpCodeKey(0xF2), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF2); This is a NOP
    addElement(assembleOpCodeKey(0xF3), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF3);
    addElement(assembleOpCodeKey(0xF4), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF4);
    addElement(assembleOpCodeKey(0xF5), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF5);
    addElement(assembleOpCodeKey(0xF6), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF6);
    addElement(assembleOpCodeKey(0xF7), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF7);
    addElement(assembleOpCodeKey(0xF8), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF8);
    addElement(assembleOpCodeKey(0xF9), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeF9);
    addElement(assembleOpCodeKey(0xFA), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeFA);
    addElement(assembleOpCodeKey(0xFB), (SRPFNANMSCRIPTOPCODEHANDLER)OpCode9A);
    //addElement(assembleOpCodeKey(0xFC), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeFC);
    addElement(assembleOpCodeKey(0xFD), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeFD);
    addElement(assembleOpCodeKey(0xFE), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeFE);
    addElement(assembleOpCodeKey(0xFF), (SRPFNANMSCRIPTOPCODEHANDLER)OpCodeEE);

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
