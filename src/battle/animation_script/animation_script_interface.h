#ifndef ANIMATION_SCRIPT_INTERFACE_H
#define ANIMATION_SCRIPT_INTERFACE_H

#include <SisterRay/SisterRay.h>

SISTERRAY_API void registerOpCode(SRPFNANMSCRIPTOPCODEHANDLER opcode, const char* modName, u16 modCodeIdx);
SISTERRAY_API u16 getOpCodeIdx(const char* modName, u16 modCodeIdx);
SISTERRAY_API void activateActorStall(u8 actorIdx, u16 stallAmt);
SISTERRAY_API void deactivateActorStall(u8 actorIdx);
#endif // !ANIMATION_SCRIPT_INTERFACE_H
