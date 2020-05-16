#include "ai_script_opcodes.h"
#include "ai_script_engine.h"


OpCodeControlSequence nopCode(AIScriptEvent* srEvent) {
    return RUN_NEXT;
}

OpCodeControlSequence OpCode00(AIScriptEvent* srEvent) {
    pushScriptValues(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode01(AIScriptEvent* srEvent) {
    pushScriptValues(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode02(AIScriptEvent* srEvent) {
    pushScriptValues(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode03(AIScriptEvent* srEvent) {
    pushScriptValues(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode10(AIScriptEvent* srEvent) {
    pushScriptAddresses(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode11(AIScriptEvent* srEvent) {
    pushScriptAddresses(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode12(AIScriptEvent* srEvent) {
    pushScriptAddresses(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode13(AIScriptEvent* srEvent) {
    pushScriptAddresses(srEvent);
    return RUN_NEXT;
}

void pushScriptValues(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    u16 addressArg = readAIScript16(srEvent->scriptPtr, scriptCtx);
    if (addressArg < 0x4000) {
        scriptCtx.arguments.type1Array[0] = srReadValue(scriptCtx.actorID, scriptCtx.currentOpCode, addressArg);
        srStackPush(scriptCtx.currentOpCode);
    }
    else {
        for (auto actorIdx = 0; actorIdx < 10; ++actorIdx) {
            scriptCtx.arguments.type1Array[actorIdx] = srReadValue(scriptCtx.actorID, scriptCtx.currentOpCode, addressArg);
        }
        scriptCtx.popMasks.type1 = 1023;
        srStackPush(scriptCtx.opCodeLow + 32);
    }
}


void pushScriptAddresses(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    scriptCtx.arguments.type1Array[0] = readAIScript16(srEvent->scriptPtr, scriptCtx);
    srStackPush(scriptCtx.currentOpCode);
}


OpCodeControlSequence OpCode30(AIScriptEvent* srEvent) {
    handleMathOperations(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode31(AIScriptEvent* srEvent) {
    handleMathOperations(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode32(AIScriptEvent* srEvent) {
    handleMathOperations(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode33(AIScriptEvent* srEvent) {
    handleMathOperations(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode34(AIScriptEvent* srEvent) {
    handleMathOperations(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode35(AIScriptEvent* srEvent) {
    handleMathOperations(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode36(AIScriptEvent* srEvent) {
    handleMathOperations(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode37(AIScriptEvent* srEvent) {
    handleMathOperations(srEvent);
    return RUN_NEXT;
}


typedef i32(*SRPFN_GAMEPERFORMMATHOPS)(u8, u8);
#define gamePerformMathOps ((SRPFN_GAMEPERFORMMATHOPS)0x5D931D)
void handleMathOperations(AIScriptEvent* srEvent) {
    i32 result = 0;
    auto& scriptCtx = *srEvent->scriptContext;
    u8 pushType = 0;

    scriptCtx.lowTypes.type2 = 0;
    scriptCtx.highTypes.type2 = 0;
    scriptCtx.popMasks.type2 = 1023;
    if (scriptCtx.opCodeLow != 7)
        srStackPop(1);
    srStackPop(0);
    for (u8 argIdx = 0; argIdx < 10; ++argIdx) {
        if ((1 << argIdx) & (scriptCtx.popMasks.type2 & scriptCtx.popMasks.type1)) {
            result = gamePerformMathOps(argIdx, argIdx);
            scriptCtx.arguments.type1Array[argIdx] = result;
        }
    }
    if (scriptCtx.lowTypes.type1 <= scriptCtx.lowTypes.type2)
        pushType = scriptCtx.lowTypes.type2;
    else
        pushType = scriptCtx.lowTypes.type1;

    if (scriptCtx.highTypes.type1 || scriptCtx.highTypes.type2) {
        pushType |= 0x20u;
        scriptCtx.popMasks.type1 &= scriptCtx.popMasks.type2;
    }
    srStackPush(pushType);
}

OpCodeControlSequence OpCode40(AIScriptEvent* srEvent) {
    handleCompareOperations(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode41(AIScriptEvent* srEvent) {
    handleCompareOperations(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode42(AIScriptEvent* srEvent) {
    handleCompareOperations(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode43(AIScriptEvent* srEvent) {
    handleCompareOperations(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode44(AIScriptEvent* srEvent) {
    handleCompareOperations(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode45(AIScriptEvent* srEvent) {
    handleCompareOperations(srEvent);
    return RUN_NEXT;
}

typedef i32(*SRPFN_GAMEPERFORMCOMPAREOPS)(u8, u8);
#define gamePerformLogicOps ((SRPFN_GAMEPERFORMCOMPAREOPS)0x5D93DF)
void handleCompareOperations(AIScriptEvent* srEvent) {
    i32 result = 0;
    auto& scriptCtx = *srEvent->scriptContext;

    srStackPop(1);
    srStackPop(0);
    for (auto argIdx = 0; argIdx < 10; ++argIdx) {
        if ((1 << argIdx) & (scriptCtx.popMasks.type2 & scriptCtx.popMasks.type1) && gamePerformLogicOps(argIdx, argIdx)) {
            result |= 1 << argIdx;
        }
    }
    if (scriptCtx.highTypes.type1 || scriptCtx.highTypes.type2) {
        scriptCtx.arguments.type1Array[0] = result;
        srStackPush(2);
    }
    else {
        scriptCtx.arguments.type1Array[0] = (result != 0);
        srStackPush(0);
    }
}


typedef bool(*SRPFN_CHECKARGSEXIST)(u8);
#define gameCheckArgsExist ((SRPFN_CHECKARGSEXIST)0x5D94C3)
OpCodeControlSequence OpCode50(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    srStackPop(1);
    srStackPop(0);
    scriptCtx.arguments.type1Array[0] = gameCheckArgsExist(0) && gameCheckArgsExist(1);
    srStackPush(0);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode51(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    srStackPop(1);
    srStackPop(0);
    scriptCtx.arguments.type1Array[0] = gameCheckArgsExist(0) || gameCheckArgsExist(1);
    srStackPush(0);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode52(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    srStackPop(0);
    scriptCtx.arguments.type1Array[0] = (gameCheckArgsExist(0) == 0);
    srStackPush(0);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode60(AIScriptEvent* srEvent) {
    handlePushConstants(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode61(AIScriptEvent* srEvent) {
    handlePushConstants(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode62(AIScriptEvent* srEvent) {
    handlePushConstants(srEvent);
    return RUN_NEXT;
}

void handlePushConstants(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    u32 sizePushed = 0;
    auto scriptPtr = srEvent->scriptPtr;
    scriptCtx.arguments.type1Array[0] = 0;
    switch (scriptCtx.opCodeLow) {
    case 0: {
        const u8* byteReader = &(scriptPtr[scriptCtx.currentScriptIdx]);
        scriptCtx.arguments.type1Array[0] = *byteReader;
        scriptCtx.currentScriptIdx++;
        srStackPush(1);
        break;
    }
    case 1: {
        const u16* wordReader = (u16*)&(scriptPtr[scriptCtx.currentScriptIdx]);
        scriptCtx.arguments.type1Array[0] = *wordReader;
        scriptCtx.currentScriptIdx += 2;
        srStackPush(2);
        break;
    }
    case 2: {
        const u32* dwordReader = (u32*)&(scriptPtr[scriptCtx.currentScriptIdx]);
        scriptCtx.arguments.type1Array[0] = *dwordReader & 0x00FFFFFF;
        scriptCtx.currentScriptIdx += 3;
        srStackPush(3);
        break;
    }
    }
}

OpCodeControlSequence OpCode70(AIScriptEvent* srEvent) {
    handleJumps(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode71(AIScriptEvent* srEvent) {
    handleJumps(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode72(AIScriptEvent* srEvent) {
    return RUN_NEXT;
}

OpCodeControlSequence OpCode73(AIScriptEvent* srEvent) {
    return BREAK;
}

OpCodeControlSequence OpCode74(AIScriptEvent* srEvent) {
    handleJumps(srEvent);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode75(AIScriptEvent* srEvent) {
    handleJumps(srEvent);
    return RUN_NEXT;
}

typedef i32(*SRPFN_SUB5D7F16)(u32);
#define setSomeContextFromPop ((SRPFN_SUB5D7F16)0x5D7F16)

typedef void(*SRPFN_SUB5C8B65)(u32, u8);
#define gameLinkScripts  ((SRPFN_SUB5C8B65)0x5C8B65)
void handleJumps(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    switch (scriptCtx.opCodeLow) {
    case 0: {
        u16 jumpDst = readAIScript16(srEvent->scriptPtr, scriptCtx);
        srStackPop(0);
        setSomeContextFromPop(0);
        if (!scriptCtx.arguments.type1Array[0])
            scriptCtx.currentScriptIdx = jumpDst;
        break;
    }
    case 1: {
        u16 jumpDst = readAIScript16(srEvent->scriptPtr, scriptCtx);
        srStackPop(1);
        setSomeContextFromPop(1);

        auto oldPosition = scriptCtx.currentScriptIdx;
        srStackPop(0);
        scriptCtx.currentScriptIdx = oldPosition;
        setSomeContextFromPop(0);
        if (scriptCtx.arguments.type1Array[0] != scriptCtx.arguments.type2Array[0])
            scriptCtx.currentScriptIdx = jumpDst;
        break;
    }
    case 2: {
        scriptCtx.currentScriptIdx = readAIScript16(srEvent->scriptPtr, scriptCtx);
        break;
    }
    case 4: {
        srStackPop(0);
        break;
    }
    case 5: {
        srStackPop(0);
        if (srEvent->charID >= 0)
            gameLinkScripts(srEvent->charID, (u8)scriptCtx.arguments.type1Array[0]);
        break;
    }
    default: {
        break;
    }
    }
}

OpCodeControlSequence OpCode80(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;

    auto oldPosition = scriptCtx.currentScriptIdx;
    srStackPop(0);
    scriptCtx.currentScriptIdx = oldPosition;
    if (scriptCtx.highTypes.type1 != 1) {
        auto pushType = srStackPop(0);
        srStackPop(1);
        scriptCtx.popMasks.type1 &= (u16)scriptCtx.arguments.type2Array[0];
        srStackPush(pushType);
    }
    return RUN_NEXT;
}

typedef u16(*SRPFN_GETRANDOMWORD)();
#define gameGetRandomWord  ((SRPFN_GETRANDOMWORD)0x5D119A)
OpCodeControlSequence OpCode81(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    scriptCtx.arguments.type1Array[0] = gameGetRandomWord();
    srStackPush(2);
    return RUN_NEXT;
}


typedef u16(*SRPFN_GETRANDOMBIT)(u16);
#define getRandomBit  ((SRPFN_GETRANDOMBIT)0x5D19A4)
OpCodeControlSequence OpCode82(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    srStackPop(0);
    scriptCtx.arguments.type1Array[0] = (u16)getRandomBit(scriptCtx.arguments.type1Array[0]);
    srStackPush(2);
    return RUN_NEXT;
}


typedef u16(*SRPFN_GAMECOUNTTARGETS)(u16);
#define gameCountTargets  ((SRPFN_GAMECOUNTTARGETS)0x5D1960)
OpCodeControlSequence OpCode83(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    auto pushType = 1;
    srStackPop(0);
    if (setSomeContextFromPop(0))
        pushType = scriptCtx.lowTypes.type1;
    else
        scriptCtx.arguments.type1Array[0] = gameCountTargets(scriptCtx.arguments.type1Array[0] & 0x3FF);
    srStackPush(pushType);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode84(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    u32 maxValue = 0;
    u16 maxValueMask = 0;
    srStackPop(0);
    for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
        if ((1 << actorIdx) & scriptCtx.popMasks.type1) {
            if (scriptCtx.arguments.type1Array[actorIdx] == maxValue) {
                maxValueMask = (1 << actorIdx) | maxValueMask;
            }
            else if (scriptCtx.arguments.type1Array[actorIdx] > maxValue) {
                maxValue = scriptCtx.arguments.type1Array[actorIdx];
                maxValueMask = 1 << actorIdx;
            }
        }
    }
    scriptCtx.arguments.type1Array[0] = maxValueMask;
    srStackPush(2);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode85(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    u32 minValue = 0xFFFFFF;
    u16 minValueMask = 0;
    srStackPop(0);
    for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
        if ((1 << actorIdx) & scriptCtx.popMasks.type1) {
            if (scriptCtx.arguments.type1Array[actorIdx] == minValue) {
                minValueMask = (1 << actorIdx) | minValueMask;
            }
            else if (scriptCtx.arguments.type1Array[actorIdx] < minValue) {
                minValue = scriptCtx.arguments.type1Array[actorIdx];
                minValueMask = 1 << actorIdx;
            }
        }
    }
    scriptCtx.arguments.type1Array[0] = minValueMask;
    srStackPush(2);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode86(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    srStackPop(0);
    scriptCtx.arguments.type1Array[0] = getMPCost(scriptCtx.arguments.type1Array[0]);
    srStackPush(2);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode87(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    srStackPop(0);
    scriptCtx.arguments.type1Array[0] = 1 << scriptCtx.arguments.type1Array[0];
    srStackPush(2);
    return RUN_NEXT;
}

OpCodeControlSequence OpCode90(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    srStackPop(0);
    setSomeContextFromPop(0);
    srStackPop(1);
    u8 writeType = scriptCtx.lowTypes.type2;
    if (scriptCtx.arguments.type2Array[0] < 0x4000u) {
        srWriteValue(scriptCtx.actorID, writeType, scriptCtx.arguments.type2Array[0], scriptCtx.arguments.type1Array[0]);
    }
    else {
        u16 writeAddress = scriptCtx.arguments.type2Array[0];
        srStackPop(1);
        u16 writeMask = srReadValue(scriptCtx.actorID, scriptCtx.lowTypes.type2, scriptCtx.arguments.type2Array[0]);
        for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
            if (writeMask & (1 << actorIdx))
                srWriteValue(actorIdx, writeType, writeAddress, scriptCtx.arguments.type1Array[0]);
        }
    }
    return RUN_NEXT;
}

OpCodeControlSequence OpCode91(AIScriptEvent* srEvent) {
    srStackPop(0);
    return RUN_NEXT;
}

typedef u8(*PFNSR_SUB5C8041)(u16);
#define getActorIdxFromMask ((PFNSR_SUB5C8041)0x5C8041)
OpCodeControlSequence OpCode92(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    srStackPop(1);
    srStackPop(0);
    u16 attackIndex = scriptCtx.arguments.type2Array[0];
    u8 commandIndex = scriptCtx.arguments.type1Array[0];
    u8 actorID = getActorIdxFromMask(AI_BATTLE_CONTEXT->scriptOwnerMask);
    enqueueScriptAction(actorID, commandIndex, attackIndex);
    return RUN_NEXT;
}

typedef void(*pfnsub43258A)(BattleQueueEntry*);
#define enqueueBattleAction      ((pfnsub43258A)0x43258A)
/*Rewrite this function to expect an ABSOLUTE instead of relative id when executing enemy attacks*/
void enqueueScriptAction(u8 actorID, u8 commandIndex, u16 relAttackIndex) {
    u32* dword_C3F338 = (u32*)(0xC3F338);
    u16* word_9AB0AE = (u16*)(0x9AB0AE);

    gAiActorVariables[actorID].lastTargets = *word_9AB0AE;
    BattleQueueEntry queueEntry = { *(u8*)dword_C3F338, 0, actorID, commandIndex, relAttackIndex, *word_9AB0AE };

    enqueueBattleAction(&queueEntry);
}


typedef u16(*PFNSR_SETSTRINGARGS)(const char*, u16*);
#define setStrArgs ((PFNSR_SETSTRINGARGS)0x41D20A)
OpCodeControlSequence OpCode93(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    const char* stringPtr = (const char*)srEvent->scriptPtr[scriptCtx.currentScriptIdx];
    u16 strIndex = setStrArgs(stringPtr, &(AI_BATTLE_CONTEXT->stringArgs));
    enqueueScriptAction(scriptCtx.actorID, 33, strIndex + 256);
    while (1) {
        u8 currentChar = *stringPtr;
        ++scriptCtx.currentScriptIdx;
        if (currentChar == 0xFF)
            break;
        if (currentChar >= 234 && currentChar < 242)
            scriptCtx.currentScriptIdx += 2;
    }
    return RUN_NEXT;
}


typedef u8(*PFNSR_SUB5D97D5)(u32, u32);
#define copySomeAIState    ((PFNSR_SUB5D97D5)0x5D97D5)
OpCodeControlSequence OpCode94(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    srStackPop(1);
    srStackPop(0);
    for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
        if ((1 << actorIdx) & scriptCtx.arguments.type2Array[0]) {
            for (u8 otherActorIdx = 0; otherActorIdx < 10; ++otherActorIdx) {
                if ((1 << otherActorIdx) & scriptCtx.arguments.type1Array[0])
                    copySomeAIState(otherActorIdx, actorIdx);
            }
        }
    }
    return RUN_NEXT;
}


typedef u8(*PFNSR_SUB5D9799)(u32, u32);
#define gameWriteToBank    ((PFNSR_SUB5D9799)0x5D9799)
OpCodeControlSequence OpCode95(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    srStackPop(1);
    srStackPop(0);
    gameWriteToBank(scriptCtx.arguments.type1Array[0], scriptCtx.arguments.type2Array[0]);
    return RUN_NEXT;
}


typedef u8(*PFNSR_SUB4337F3)(u32, u32, u32);
#define gameSetElementalData    ((PFNSR_SUB4337F3)0x4337F3)
OpCodeControlSequence OpCode96(AIScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    srStackPop(1);
    srStackPop(0);
    gameSetElementalData(scriptCtx.arguments.type1Array[0], scriptCtx.arguments.type2Array[0], 0);
    return RUN_NEXT;
}
