#include "ai_script_engine.h"
#include "../../impl.h"


SrAIScriptOpCodeRegistry::SrAIScriptOpCodeRegistry() : SrNamedResourceRegistry<SRPFNAISCRIPTOPCODEHANDLER, std::string>() {
    addElement(assembleOpCodeKey(0x00), (SRPFNAISCRIPTOPCODEHANDLER)OpCode00);
    addElement(assembleOpCodeKey(0x01), (SRPFNAISCRIPTOPCODEHANDLER)OpCode01);
    addElement(assembleOpCodeKey(0x02), (SRPFNAISCRIPTOPCODEHANDLER)OpCode02);
    addElement(assembleOpCodeKey(0x03), (SRPFNAISCRIPTOPCODEHANDLER)OpCode03);
    addElement(assembleOpCodeKey(0x04), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x05), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x06), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x07), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x08), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x09), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x0A), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x0B), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x0C), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x0D), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x0E), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x0F), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);

    addElement(assembleOpCodeKey(0x10), (SRPFNAISCRIPTOPCODEHANDLER)OpCode10);
    addElement(assembleOpCodeKey(0x11), (SRPFNAISCRIPTOPCODEHANDLER)OpCode11);
    addElement(assembleOpCodeKey(0x12), (SRPFNAISCRIPTOPCODEHANDLER)OpCode12);
    addElement(assembleOpCodeKey(0x13), (SRPFNAISCRIPTOPCODEHANDLER)OpCode13);
    addElement(assembleOpCodeKey(0x14), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x15), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x16), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x17), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x18), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x19), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x1A), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x1B), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x1C), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x1D), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x1E), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x1F), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);


    addElement(assembleOpCodeKey(0x20), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x21), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x22), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x23), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x24), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x25), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x26), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x27), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x28), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x29), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x2A), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x2B), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x2C), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x2D), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x2E), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x2F), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);

    addElement(assembleOpCodeKey(0x30), (SRPFNAISCRIPTOPCODEHANDLER)OpCode30);
    addElement(assembleOpCodeKey(0x31), (SRPFNAISCRIPTOPCODEHANDLER)OpCode31);
    addElement(assembleOpCodeKey(0x32), (SRPFNAISCRIPTOPCODEHANDLER)OpCode32);
    addElement(assembleOpCodeKey(0x33), (SRPFNAISCRIPTOPCODEHANDLER)OpCode33);
    addElement(assembleOpCodeKey(0x34), (SRPFNAISCRIPTOPCODEHANDLER)OpCode34);
    addElement(assembleOpCodeKey(0x35), (SRPFNAISCRIPTOPCODEHANDLER)OpCode35);
    addElement(assembleOpCodeKey(0x36), (SRPFNAISCRIPTOPCODEHANDLER)OpCode36);
    addElement(assembleOpCodeKey(0x37), (SRPFNAISCRIPTOPCODEHANDLER)OpCode37);
    addElement(assembleOpCodeKey(0x38), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x39), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x3A), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x3B), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x3C), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x3D), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x3E), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x3F), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);

    addElement(assembleOpCodeKey(0x40), (SRPFNAISCRIPTOPCODEHANDLER)OpCode40);
    addElement(assembleOpCodeKey(0x41), (SRPFNAISCRIPTOPCODEHANDLER)OpCode41);
    addElement(assembleOpCodeKey(0x42), (SRPFNAISCRIPTOPCODEHANDLER)OpCode42);
    addElement(assembleOpCodeKey(0x43), (SRPFNAISCRIPTOPCODEHANDLER)OpCode43);
    addElement(assembleOpCodeKey(0x44), (SRPFNAISCRIPTOPCODEHANDLER)OpCode44);
    addElement(assembleOpCodeKey(0x45), (SRPFNAISCRIPTOPCODEHANDLER)OpCode45);
    addElement(assembleOpCodeKey(0x46), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x47), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x48), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x49), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x4A), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x4B), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x4C), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x4D), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x4E), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x4F), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);

    addElement(assembleOpCodeKey(0x50), (SRPFNAISCRIPTOPCODEHANDLER)OpCode50);
    addElement(assembleOpCodeKey(0x51), (SRPFNAISCRIPTOPCODEHANDLER)OpCode51);
    addElement(assembleOpCodeKey(0x52), (SRPFNAISCRIPTOPCODEHANDLER)OpCode52);
    addElement(assembleOpCodeKey(0x53), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x54), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x55), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x56), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x57), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x58), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x59), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x5A), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x5B), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x5C), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x5D), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x5E), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x5F), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);

    addElement(assembleOpCodeKey(0x60), (SRPFNAISCRIPTOPCODEHANDLER)OpCode60);
    addElement(assembleOpCodeKey(0x61), (SRPFNAISCRIPTOPCODEHANDLER)OpCode61);
    addElement(assembleOpCodeKey(0x62), (SRPFNAISCRIPTOPCODEHANDLER)OpCode62);
    addElement(assembleOpCodeKey(0x63), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x64), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x65), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x66), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x67), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x68), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x69), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x6A), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x6B), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x6C), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x6D), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x6E), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x6F), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);

    addElement(assembleOpCodeKey(0x70), (SRPFNAISCRIPTOPCODEHANDLER)OpCode70);
    addElement(assembleOpCodeKey(0x71), (SRPFNAISCRIPTOPCODEHANDLER)OpCode71);
    addElement(assembleOpCodeKey(0x72), (SRPFNAISCRIPTOPCODEHANDLER)OpCode72);
    addElement(assembleOpCodeKey(0x73), (SRPFNAISCRIPTOPCODEHANDLER)OpCode73);
    addElement(assembleOpCodeKey(0x74), (SRPFNAISCRIPTOPCODEHANDLER)OpCode74);
    addElement(assembleOpCodeKey(0x75), (SRPFNAISCRIPTOPCODEHANDLER)OpCode75);
    addElement(assembleOpCodeKey(0x76), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x77), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x78), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x79), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x7A), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x7B), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x7C), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x7D), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x7E), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x7F), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);

    addElement(assembleOpCodeKey(0x80), (SRPFNAISCRIPTOPCODEHANDLER)OpCode80);
    addElement(assembleOpCodeKey(0x81), (SRPFNAISCRIPTOPCODEHANDLER)OpCode81);
    addElement(assembleOpCodeKey(0x82), (SRPFNAISCRIPTOPCODEHANDLER)OpCode82);
    addElement(assembleOpCodeKey(0x83), (SRPFNAISCRIPTOPCODEHANDLER)OpCode83);
    addElement(assembleOpCodeKey(0x84), (SRPFNAISCRIPTOPCODEHANDLER)OpCode84);
    addElement(assembleOpCodeKey(0x85), (SRPFNAISCRIPTOPCODEHANDLER)OpCode85);
    addElement(assembleOpCodeKey(0x86), (SRPFNAISCRIPTOPCODEHANDLER)OpCode86);
    addElement(assembleOpCodeKey(0x87), (SRPFNAISCRIPTOPCODEHANDLER)OpCode87);
    addElement(assembleOpCodeKey(0x88), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x89), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x8A), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x8B), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x8C), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x8D), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x8E), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);
    addElement(assembleOpCodeKey(0x8F), (SRPFNAISCRIPTOPCODEHANDLER)nopCode);

    addElement(assembleOpCodeKey(0x90), (SRPFNAISCRIPTOPCODEHANDLER)OpCode90);
    addElement(assembleOpCodeKey(0x91), (SRPFNAISCRIPTOPCODEHANDLER)OpCode91);
    addElement(assembleOpCodeKey(0x92), (SRPFNAISCRIPTOPCODEHANDLER)OpCode92);
    addElement(assembleOpCodeKey(0x93), (SRPFNAISCRIPTOPCODEHANDLER)OpCode93);
    addElement(assembleOpCodeKey(0x94), (SRPFNAISCRIPTOPCODEHANDLER)OpCode94);
    addElement(assembleOpCodeKey(0x95), (SRPFNAISCRIPTOPCODEHANDLER)OpCode95);
    addElement(assembleOpCodeKey(0x96), (SRPFNAISCRIPTOPCODEHANDLER)OpCode96);
}

/*This is the handler for the 86 AI script opcode, rewritten to work correctly for new scenes*/
i32 getMPCost(i32 attackID) {
    u16 MPCost;

    MPCost = 0;
    if (attackID < 0xFFFF) {
        const AttackData& attackData = getAttackDataPtr(attackID);
        MPCost = attackData.MPCost;
    }
    return MPCost;
}

/*This now gets the MP cost of an enemy spell, or player spells with enemy indexes*/
const AttackData& getAttackDataPtr(i32 attackID) {
    const auto& attack = getCommandAction(CMD_ENEMY_ACTION, attackID).attackData;
    return attack;
}

#define G_SCRIPT_IDX_PTR  ((AIScriptContext**)0xC3F330)
void srRunAIScript(u8 actorID, const u8* scriptPtr, u8 charID) {
    bool scriptEnded = false;
    const u8* const runningScriptPtr = scriptPtr;
    auto scriptCtx = G_AI_SCRIPT_CTX;     // this is the same memory as the battle context struct, but the values are for the most part unrelated
    *G_SCRIPT_IDX_PTR = G_AI_SCRIPT_CTX;
    scriptCtx->actorID = actorID;
    scriptCtx->currentScriptIdx = 0;
    scriptCtx->stackPosition = 512;
    srLogWrite("Inside SR AI Script, actor ID: %i, characterID: %i", actorID, charID);
    while (!scriptEnded) {
        scriptCtx->currentOpCode = runningScriptPtr[scriptCtx->currentScriptIdx++];
        scriptCtx->opCodeLow = scriptCtx->currentOpCode & 0xF;
        scriptCtx->opCodeHigh = scriptCtx->currentOpCode >> 4;
        AIScriptEvent srEvent = { actorID, runningScriptPtr, scriptCtx, scriptCtx->currentScriptIdx, charID };
        srLogWrite("Running Opcode with high op: %x, low op: %x", scriptCtx->opCodeHigh, scriptCtx->opCodeLow);
        auto opcode = gContext.AIScriptOpcodes.getResource(scriptCtx->currentOpCode);
        auto control = opcode(&srEvent);
        switch (control) {
        case RUN_NEXT: {
            continue;
            break;
        }
        case BREAK: {
            return;
            break;
        }
        }
    }
}

u16 readAIScript16(const u8* const scriptPtr, AIScriptContext& scriptCtx) {
    auto wordReader = &(scriptPtr[scriptCtx.currentScriptIdx]);
    u16 value = *wordReader;
    scriptCtx.currentScriptIdx += 2;
    return value;
}

//Using this, we can modularize reads/writes to AI state variables
u16 getAIScriptBaseAddress(u8 actorID, u16 addressArg, u32** ret) {
    u32 relativeAddress; // [esp+0h] [ebp-4h]

    relativeAddress = addressArg;
    if (addressArg >= 0x2000) {
        if (addressArg >= 0x4000) {
            *ret = (u32*)(&AI_BATTLE_CONTEXT->actorAIStates[actorID]);
            relativeAddress = addressArg - 0x4000;
        }
        else {
            *ret = (u32*)AI_BATTLE_CONTEXT;
            relativeAddress = addressArg - 0x2000;
        }
    }
    else {
        *ret = (u32*)gContext.battleActors.getActorRandomVarBuffer(actorID); //now points to extended SR RAM space
    }
    return relativeAddress;
}

u32 srReadValue(u8 actorID, u8 readType, u16 addressArg) {
    u32* baseAddress = nullptr;
    u16 relativeAddress = getAIScriptBaseAddress(actorID, addressArg, &baseAddress);
    u32 readValue = 0;
    //Address overrides for modularized battle stats
    switch (addressArg) {
    case 0x4180: {
        return getSrBattleStat(actorID, "HP").activeValue;
    }
    case 0x4150: {
        return getSrBattleStat(actorID, "MP").activeValue;
    }
    case 0x4068: {
        return getSrBattleStat(actorID, "STR").activeValue;
    }
    case 0x4070: {
        return getSrBattleStat(actorID, "MAG").activeValue;
    }
    case 0x4100: {
        return getSrBattleStat(actorID, "VIT").activeValue;
    }
    case 0x4110: {
        return getSrBattleStat(actorID, "SPR").activeValue;
    }
    case 0x40A0: {
        return getSrBattleStat(actorID, "DEX").activeValue;
    }
    case 0x40A8: {
        return getSrBattleStat(actorID, "LCK").activeValue;
    }
    }

    switch (readType) {
    case 0:
        readValue = ((1 << (relativeAddress & 7)) & *(u8*)(baseAddress + (relativeAddress >> 3))) != 0;
        break;
    case 1:
        readValue = *(u8*)(baseAddress + (relativeAddress >> 3));
        break;
    case 2:
        readValue = *(u16*)(baseAddress + 2 * (relativeAddress >> 4));
        break;
    case 3:
        readValue = *(u32*)(baseAddress + 4 * (relativeAddress >> 5));
        break;
    default:
        return readValue;
    }
    return readValue;
}

void srWriteValue(u8 actorID, u8 writeType, u16 addressArg, u32 valueArg) {
    u32* baseAddress; // [esp+8h] [ebp-Ch]
    u32 relativeAddress; // [esp+24h] [ebp+10h]

    switch (addressArg) {
    case 0x4180: {
        setActorBattleStat(actorID, "HP", valueArg);
        return;
    }
    case 0x4150: {
        setActorBattleStat(actorID, "MP", valueArg);
        return;
    }
    case 0x4068: {
        setActorBattleStat(actorID, "STR", valueArg);
        return;
    }
    case 0x4070: {
        setActorBattleStat(actorID, "MAG", valueArg);
        return;
    }
    case 0x4100: {
        setActorBattleStat(actorID, "VIT", valueArg);
        return;
    }
    case 0x4110: {
        setActorBattleStat(actorID, "SPR", valueArg);
        return;
    }
    case 0x40A0: {
        setActorBattleStat(actorID, "DEX", valueArg);
        return;
    }
    case 0x40A8: {
        setActorBattleStat(actorID, "LCK", valueArg);
        return;
    }
    }

    relativeAddress = getAIScriptBaseAddress(actorID, addressArg, &baseAddress);

    u16* wordWriter = (u16*)(baseAddress + 2 * (relativeAddress / 16));
    u32* dwordWriter = (u32*)(baseAddress + 4 * (relativeAddress / 32));
    switch (writeType) {
    case 0: {
        u8* writeAddr = (u8*)((relativeAddress >> 3) + baseAddress);
        (*writeAddr) &= ~(1 << (relativeAddress & 7));
        if (valueArg)
            (*writeAddr) |= 1 << (relativeAddress & 7);
        break;
    }
    case 1: {
        u8* writeAddr = (u8*)(relativeAddress / 8 + baseAddress);
        *writeAddr = valueArg;
        break;
    }
    case 2: {
        *wordWriter = valueArg;
        break;
    }
    case 3:
        *dwordWriter = valueArg;
        break;
    default:
        return;
    }
}

typedef void(*SRPFN_GAMESTACKPUSH)(u8);
#define gameStackPush  ((SRPFN_GAMESTACKPUSH)0x5D8ECF)
void srStackPush(u8 argType) {
    gameStackPush(argType);
}


typedef u8(*SRPFN_GAMESTACKPOP)(u8);
#define gameStackPop  ((SRPFN_GAMESTACKPOP)0x5D9099)
u8 srStackPop(u8 popType) {
    return gameStackPop(popType);
}
