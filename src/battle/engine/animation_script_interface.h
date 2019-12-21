#ifndef ANIMATION_SCRIPT_INTERFACE_H
#define ANIMATION_SCRIPT_INTERFACE_H

#include <SisterRay/SisterRay.h>

SISTERRAY_API void registerOpCode(SRPFNANMSCRIPTOPCODEHANDLER opcode, const char* modName, u16 modCodeIdx);
SISTERRAY_API u16 getOpCodeIdx(SRPFNANMSCRIPTOPCODEHANDLER opcode, const char* modName, u16 modCodeIdx);
#endif // !ANIMATION_SCRIPT_INTERFACE_H
