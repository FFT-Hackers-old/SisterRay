#ifndef ANIMATION_SCRIPT_OPCODE_H
#define ANIMATION_SCRIPT_OPCODE_H

#include <SisterRay/SisterRay.h>
#include "../../sr_named_registry.h"

u8 readOpCodeArg8(u8* scriptPtr, ScriptContext* ctx, BattleModelState* battleModelState);
u16 readOpCodeArg16(u8* scriptPtr, ScriptContext* ctx, BattleModelState* battleModelState);
u32 readOpCodeArg32(u8* scriptPtr, ScriptContext* ctx, BattleModelState* battleModelState);
std::string assembleOpCodeKey(u16 opCode);
SISTERRAY_API void initAnimScriptOpCodes();

class SrAnimOpCodeRegistry : public SrNamedResourceRegistry<SRPFNANMSCRIPTOPCODEHANDLER, std::string> {
public:
    SrAnimOpCodeRegistry();
};

#endif // !ANIMATION_OPCODE_H
