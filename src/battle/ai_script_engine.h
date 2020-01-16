#ifndef AI_SCRIPT_ENGINE_H
#define AI_SCRIPT_ENGINE_H

#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "ai_script_opcodes.h"

i32 getMPCost(i32 attackID);
const AttackData& getAttackDataPtr(i32 attackID);

class SrAIScriptOpCodeRegistry : public SrNamedResourceRegistry<SRPFNAISCRIPTOPCODEHANDLER, std::string> {
public:
    SrAIScriptOpCodeRegistry();
};

void srRunAIScript(u8 actorID, u8* scriptPtr, u8 charID);
u16 readAIScript16(const u8* const scriptPtr, AIScriptContext& scriptCtx);
u32 srReadValue(u8 actorID, u8 readType, u16 addressArg);
void srWriteValue(u8 actorID, u8 writeType, u16 addressArg, u32 valueArg);
void srStackPush(u8 argType);
u8 srStackPop(u8 popType);

#endif
