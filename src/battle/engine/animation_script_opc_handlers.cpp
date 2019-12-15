#include "animation_script_opc_handlers.h"
#include "../battle_models/battle_model_state_interface.h"
#include "../battle_models/model_skeleton.h"
#include "action_effects.h"
#include "../../party/party_utils.h"
#include "action_spell_effects.h"
#include "../../impl.h"
#include "animation_player.h"
#include "damage_events.h"
#include "../battle_utils.h"

OpCodeControlSequence OpCode8E(AnimScriptEvent* srEvent) {
    u8* byte_BF8710 = (u8*)0xBFB710;
    u8* byte_BF2150 = (u8*)0xBF2150;
    if (!*byte_BF8710) {
        *byte_BF2150 = 1;
        *byte_BF8710 = 1;
    }
    return RUN_NEXT;
}

OpCodeControlSequence OpCode8F(AnimScriptEvent* srEvent) {
    u8* byte_BF2150 = (u8*)0xBF2150;
    *byte_BF2150 = 0;
    return RUN_NEXT;
}

typedef void(*SRPFNSUB41FB1C)(u32, PolygonSet*);
#define sub_41FB1C    ((SRPFNSUB41FB1C)0x41FB1C)

typedef void(*SRPFNSUB685028)(u32, SRPFNSUB41FB1C, ModelSkeleton*);
#define loadEmeraldTextureSet    ((SRPFNSUB685028)0x685028)

typedef void(*SRPFNSUB68924B)(u32, u32, u8*, u32, TextureSet*);
#define sub_68924B    ((SRPFNSUB68924B)0x68924B)

OpCodeControlSequence OpCode90(AnimScriptEvent* srEvent) {
    u32* off_C05FEC = (u32*)0xC05FEC;
    u32* off_C05FE8 = (u32*)0xC05FE8;
    u32* dword_9ADEF0 = (u32*)0x9ADEF0;
    u32* dword_9ADEF4 = (u32*)0x9ADEF4;
    u32* dword_9ADEE8 = (u32*)0x9ADEE8;
    TextureSet** dword_9ADEE4 = (TextureSet**)(0x9ADEE4);
    TextureSet** dword_9ADEE0 = (TextureSet**)(0x9ADEE0);

    auto modelDataArray = srEvent->battleModelState;
    auto scriptCtx = srEvent->scriptContext;
    auto scriptPtr = srEvent->scriptPtr;
    auto actorID = srEvent->actorID;
    scriptCtx->field_15 = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID));
    switch (actorID) {
    case 6: {
        *off_C05FE8 = 0;
        *off_C05FEC = scriptCtx->field_15;
        break;
    }
    case 7: {
        *off_C05FE8 = 0;
        *off_C05FEC = scriptCtx->field_15 + 8;
        break;
    }
    case 8: {
        *off_C05FE8 = 1;
        *off_C05FEC = scriptCtx->field_15;
        break;
    }
    case 9: {
        *off_C05FE8 = 1;
        *off_C05FEC = scriptCtx->field_15 + 8;
        break;
    }
    default:
        break;
    }
    auto gameContext = getGraphicsCtx();
    GraphicsDriverCallbacks* graphicsCallBacks = gameContext->graphicsDriverCallbacks;
    auto wordArg = readOpCodeArg16(scriptPtr, scriptCtx, getBattleModelState(actorID));
    u8 v72 = 8 * (wordArg & 0x1F);
    u8 v71 = (wordArg & 0x3E0) >> 2;
    u8 v70 = (wordArg & 0x7C00) >> 7;
    u8* v76 = &v70;
    auto v73 = -1;
    auto unkPaletteSwapArg = *off_C05FEC;
    auto v77 = 0;
    *dword_9ADEE8 = 0;
    *dword_9ADEE0 = 0;
    *dword_9ADEE4 = 0;
    *dword_9ADEF0 = 0;
    *dword_9ADEF4 = 0;
    loadEmeraldTextureSet(0, sub_41FB1C, modelDataArray[4].modelDataPtr->skeletonData);

    auto emeraldTextureSet = *dword_9ADEE0;
    if (actorID >= 8)
        emeraldTextureSet = *dword_9ADEE4;

    if (emeraldTextureSet) {
        sub_68924B(0, 1, v76, unkPaletteSwapArg, emeraldTextureSet);
        graphicsCallBacks->paletteChanged(unkPaletteSwapArg, 1, unkPaletteSwapArg, emeraldTextureSet->palette, emeraldTextureSet);
    }
    return RUN_NEXT;
}

#define opCode91EffectHead       ((PFNSREFFECTCALLBACK)0x4255B7)
OpCodeControlSequence OpCode91(AnimScriptEvent* srEvent) {
    u32* off_C05FE8 = (u32*)0xC05FE8;
    auto scriptCtx = srEvent->scriptContext;
    auto scriptPtr = srEvent->scriptPtr;
    auto actorID = srEvent->actorID;
    *off_C05FE8 = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID));
    scriptCtx->field_14 = srPushEffect60(opCode91EffectHead);
    auto& effectCtx = *getEffectContext60(scriptCtx->field_14);
    effectCtx.wordArray[1] = *(u16*)off_C05FE8;
    return RUN_NEXT;
}

OpCodeControlSequence OpCode92(AnimScriptEvent* srEvent) {
    u8* byte_BFB2EC = (u8*)0xBFB2EC;
    *byte_BFB2EC = 1;
    return RUN_NEXT;
}

#define opCode93EffectHead        ((PFNSREFFECTCALLBACK)0x5BCD42)
OpCodeControlSequence OpCode93(AnimScriptEvent* srEvent) {
    srPushEffect60(opCode93EffectHead);
    return RUN_NEXT;
}

#define opCode94EffectHead       ((PFNSREFFECTCALLBACK)0x426899) //rotates model arg1-arg2 in arg3 step-sizes
OpCodeControlSequence OpCode94(AnimScriptEvent* srEvent) {
    u32* off_C05FE8 = (u32*)0xC05FE8;
    u32* off_C05FEC = (u32*)0xC05FEC;
    u32* off_C05FF0 = (u32*)0xC05FF0;
    auto scriptCtx = srEvent->scriptContext;
    auto scriptPtr = srEvent->scriptPtr;
    auto actorID = srEvent->actorID;

    *off_C05FE8 = readOpCodeArg16(scriptPtr, scriptCtx, getBattleModelState(actorID));
    *off_C05FEC = readOpCodeArg16(scriptPtr, scriptCtx, getBattleModelState(actorID));
    *off_C05FF0 = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID));;

    scriptCtx->field_14 = srPushEffect10(opCode94EffectHead);
    auto& effectCtx = *getEffectContext60(scriptCtx->field_14);
    effectCtx.wordArray[3] = actorID;
    effectCtx.wordArray[4] = *(u16*)off_C05FE8;
    effectCtx.wordArray[5] = *(u16*)off_C05FEC;
    effectCtx.wordArray[6] = (*off_C05FEC - *off_C05FE8) / *off_C05FF0;
    effectCtx.wordArray[1] = *(u16*)off_C05FF0;
    return RUN_NEXT;
}

OpCodeControlSequence OpCode95(AnimScriptEvent* srEvent) {
    u8* byte_BFD0E0 = (u8*)(0xBFD0E0);
    auto& actorModelState = *getBattleModelState(srEvent->actorID);
    if (*byte_BFD0E0 != 1 && *byte_BFD0E0 == 2 && srEvent->actorID < 4) {
        actorModelState.initialYRotation = 2048;
    }
    actorModelState.restingYRotation = actorModelState.initialYRotation;
    return RUN_NEXT;
}


typedef void(*SRLOADMACHINEGUNEFFECT)(void);
#define gameLoadMachineGunEffect        ((SRLOADMACHINEGUNEFFECT)0x4390F0)
#define opCode96EffectHead       ((PFNSREFFECTCALLBACK)0x43937F) //machine-gun cb-chain head
OpCodeControlSequence OpCode96(AnimScriptEvent* srEvent) {
    u16* someBarretWeaponData = (u16*)0x7C10B8;
    u32** dword_9AEAF0 = (u32**)0x9AEAF0; //pointer to some struct
    u32* dword_D8E490 = (u32*)0xD8E490;
    u8* byte_BFCDE0 = (u8*)0xBFCDE0;
    auto scriptCtx = srEvent->scriptContext;
    auto scriptPtr = srEvent->scriptPtr;
    auto actorID = srEvent->actorID;

    scriptCtx->opCodeArgs[0] = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID));
    scriptCtx->opCodeArgs[1] = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID));
    auto actorData = getActivePartyMember(actorID);
    if (actorData) {
        u16 weaponModelIdx = actorData->weaponData.weapon_model & 0xF;
        scriptCtx->opCodeArgs[2] = someBarretWeaponData[weaponModelIdx];
        *dword_9AEAF0 = dword_D8E490;
        gameLoadMachineGunEffect();
        auto& effectCtx = *getEffectContext60(srPushEffect60(opCode96EffectHead));
        effectCtx.wordArray[1] = *byte_BFCDE0;
        effectCtx.wordArray[2] = actorID;
        *(u32 *)&(effectCtx.wordArray[13]) = 0xFFFFFFFF;
        effectCtx.wordArray[12] = scriptCtx->opCodeArgs[2];
        effectCtx.wordArray[3] = scriptCtx->opCodeArgs[0];
        effectCtx.wordArray[4] = scriptCtx->opCodeArgs[1];
    }
    return RUN_NEXT;
}

typedef void(*SRPFNSUB42567E)(u16, u8, u8);
#define sub_42567E         ((SRPFNSUB42567E)0x42567E)
OpCodeControlSequence OpCode97(AnimScriptEvent* srEvent) {
    u32* off_C05FE8 = (u32*)0xC05FE8;
    u32* off_C05FEC = (u32*)0xC05FEC;
    auto scriptCtx = srEvent->scriptContext;
    auto scriptPtr = srEvent->scriptPtr;
    auto actorID = srEvent->actorID;

    *off_C05FE8 = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID));
    *off_C05FEC = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID));
    sub_42567E(actorID, *(u8*)off_C05FE8, *(u8*)off_C05FEC);
    return RUN_NEXT;
}

//Displays strings with a delay
OpCodeControlSequence OpCode98(AnimScriptEvent* srEvent) {
    u32* off_C05FE8 = (u32*)0xC05FE8;
    u32* off_C05FEC = (u32*)0xC05FEC;
    u8* byte_DC0E11 = (u8*)0xDC0E11;
    auto scriptCtx = srEvent->scriptContext;
    auto scriptPtr = srEvent->scriptPtr;
    auto actorID = srEvent->actorID;

    *off_C05FE8 = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID)); //number of frames to wait
    scriptCtx->field_14 = srPushEffect100((PFNSREFFECTCALLBACK)srDisplayActionStringEffect);;
    auto& effectCtx = *getEffectContext100(scriptCtx->field_14);
    effectCtx.wordArray[1] = ((u16)(*byte_DC0E11 >> 2)) + 4; //number of frames to display string
    effectCtx.wordArray[2] = *(u16*)off_C05FE8; //delay before display begins
    return RUN_NEXT;
}

#define moveCallback426A26  ((PFNSREFFECTCALLBACK)0x426A26)
OpCodeControlSequence OpCode99(AnimScriptEvent* srEvent) {
    u32* off_C05FE8 = (u32*)0xC05FE8;
    u32* off_C05FEC = (u32*)0xC05FEC;
    u8* byte_BE10B4 = (u8*)0xBE10B4;
    u8* isTargetingAll = (u8*)0xBF2E1C;
    u16* currentTargetMask = (u16*)0xBFD0F8;
    auto scriptCtx = srEvent->scriptContext;
    auto scriptPtr = srEvent->scriptPtr;
    auto actorID = srEvent->actorID;

    *byte_BE10B4 = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID));
    scriptCtx->opCodeArgs[4] = readOpCodeArg16(scriptPtr, scriptCtx, getBattleModelState(actorID));
    *off_C05FE8 = readOpCodeArg16(scriptPtr, scriptCtx, getBattleModelState(actorID));
    scriptCtx->opCodeArgs[3] = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID));
    if (*isTargetingAll) {
        srComputeMultitargetMoveBase(*currentTargetMask, (ModelPosition*)&scriptCtx->field_16);
        srDispatchMoveEffects(
            actorID,
            scriptPtr,
            scriptCtx->opCodeArgs[4],
            moveCallback426A26,
            scriptCtx->opCodeArgs[3],
            (ModelPosition*)&(scriptCtx->field_16));
    }
    else {
        u8* byte_BFB198 = (u8*)0xBFB198;
        auto targetID = *byte_BFB198;
        srDispatchMoveEffects(
            actorID,
            scriptPtr,
            scriptCtx->opCodeArgs[4],
            moveCallback426A26,
            scriptCtx->opCodeArgs[3],
            &(getBattleModelState(targetID))->restingPosition);
    }
    return RUN_NEXT;
}

OpCodeControlSequence OpCode9B(AnimScriptEvent* srEvent) {
    u8* byte_BF2370 = (u8*)0xBF2370;
    *byte_BF2370 = 1;
    return RUN_NEXT;
}


typedef void(*SRPFNSUB430C9F)(void);
#define gameSetVolumeLimits        ((SRPFNSUB430C9F)0x430C9F)
OpCodeControlSequence OpCode9C(AnimScriptEvent* srEvent) {
    gameSetVolumeLimits();
    return RUN_NEXT;
}

/*This opcode dispatches the effects for tifa's limit breaks*/
OpCodeControlSequence OpCode9D(AnimScriptEvent* srEvent) {
    u32* off_C05FE8 = (u32*)0xC05FE8;
    auto scriptCtx = srEvent->scriptContext;
    auto actorID = srEvent->actorID;
    u16* currentTargetMask = (u16*)0xBFD0F8;

    *off_C05FE8 = readOpCodeArg8(srEvent->scriptPtr, scriptCtx, getBattleModelState(srEvent->actorID));
    switch (*off_C05FE8) {
    case 0:
        srBeatRushSpellEffect(*currentTargetMask, actorID);
        break;
    case 1:
        srBeatSomersaultEffect(*currentTargetMask, actorID);
        break;
    case 2:
        srWaterkickSpellEffect(*currentTargetMask, actorID);
        break;
    case 3:
        srMeteodriveSpellEffect(*currentTargetMask, actorID);
        break;
    case 4:
        srDoplhinBlowSpellEffect(*currentTargetMask, actorID);
        break;
    case 5:
        srMeteorStrikeSpellEffect(*currentTargetMask, actorID);
        break;
    case 6:
        srFinalHeavenSpellEffect(*currentTargetMask, actorID);
        break;
    default:
        return RUN_NEXT;
    }
    return RUN_NEXT;
}

OpCodeControlSequence OpCode9E(AnimScriptEvent* srEvent) {
    u8* byte_BFCDE0 = (u8*)0xBFCDE0; //some actor ID
    auto& actorModelState = *srEvent->battleModelState;
    if (srEvent->actorID == *byte_BFCDE0) {
        if (getEffect100QueueSize()) {
            --actorModelState.currentScriptPosition;
        }
        else {
            actorModelState.field_26 = 1;
        }
    }
    else if (getBattleModelState(*byte_BFCDE0)->field_26 != 1 || getEffect100QueueSize()){
        actorModelState.field_26 = 1;
        --actorModelState.currentScriptPosition;
    }
    else{
        actorModelState.field_26 = 1;
    }
    return BREAK;
}

OpCodeControlSequence OpCode9F(AnimScriptEvent* srEvent) {
    u8* byte_BF2370 = (u8*)0xBF2370;
    *byte_BF2370 = 0;
    return RUN_NEXT;
}


OpCodeControlSequence OpCodeA0(AnimScriptEvent* srEvent) {
    u8* G_TARGET_INDEX = (u8*)0xBFB198;
    u32* off_C05FE8 = (u32*)0xC05FE8;
    auto& targetModelState74 = *getBattleModelState74(*G_TARGET_INDEX);
    auto& targetModelState = *getBattleModelState(*G_TARGET_INDEX);
    targetModelState74.field_C |= 1;
    *off_C05FE8 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    targetModelState.field_170 = (&(srEvent->battleModelState->field_174) + 52 * (*off_C05FE8)); // this is some kind of array
    targetModelState.restingYRotation = 0;
    targetModelState.restingXRotation = 0;
    targetModelState.restingPosition.zCoordinate = 0;
    targetModelState.restingPosition.yCoordinate = 0;
    targetModelState.restingPosition.xCoordinate = 0;
    targetModelState.restingZRotation = 2048;
    targetModelState.field_25 |= 0x10u;
    targetModelState.field_1AE4 = srEvent->actorID;
    targetModelState.field_1AE8 = *off_C05FE8;
    return RUN_NEXT;
}

typedef void(*PFNSRSUB5BEB27)(u16, u16);
#define gamePushPeriodicDamage    ((PFNSRSUB5BEB27)0x5BEB27)
OpCodeControlSequence OpCodeA1(AnimScriptEvent* srEvent) {
    u8* byte_BF23BC = (u8*)0xBF23BC;
    u32* off_C05FE8 = (u32*)0xC05FE8;
    u32* off_C05FEC = (u32*)0xC05FEC;
    *byte_BF23BC = 0;
    *off_C05FE8 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C05FEC = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState); //period
    gamePushPeriodicDamage(*off_C05FE8, *off_C05FEC);
    return RUN_NEXT;
}

#define gamePushSpellAura            ((PFNSREFFECTCALLBACK)0x425520)
OpCodeControlSequence OpCodeA4(AnimScriptEvent* srEvent) {
    auto effectCtx = srCreateEffect(gamePushSpellAura, EFFECT60);
    effectCtx->wordArray[0] = 2;
    effectCtx->wordArray[1] = 496;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeA5(AnimScriptEvent* srEvent) {
    auto effectCtx = srCreateEffect(gamePushSpellAura, EFFECT60);
    effectCtx->wordArray[0] = 3;
    effectCtx->wordArray[1] = 489;
    return RUN_NEXT;
}


OpCodeControlSequence OpCodeA6(AnimScriptEvent* srEvent) {
    u8* G_ATTACKER_IDX = (u8*)0xBFCB68;
    R3PointWord* actorPositionArray = (R3PointWord*)(0xBFD0A0);
    auto& attackerModelState = *getBattleModelState(*G_ATTACKER_IDX);
    const auto& actorPosition = actorPositionArray[*G_ATTACKER_IDX];
    attackerModelState.restingPosition.xCoordinate = actorPosition.x;
    attackerModelState.restingPosition.yCoordinate = actorPosition.y;
    attackerModelState.restingPosition.zCoordinate = actorPosition.z;
    return RUN_NEXT;
}


OpCodeControlSequence OpCodeA7(AnimScriptEvent* srEvent) {
    auto& scriptCtx = *(srEvent->scriptContext);
    auto& actorModelState = *(srEvent->battleModelState);
    auto& actorModelState74 = *getBattleModelState74(srEvent->actorID);
    auto& actorRotationData = *getBattleModelRotationData(srEvent->actorID);
    actorModelState.runningAnimIdx = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    actorModelState.field_74 = 0;
    actorRotationData.field_0 = 0;
    if (actorModelState74.field_36){
        for (auto frame = 0; frame < actorModelState74.field_36; ++frame)
            srPlayModelAnimation(srEvent->actorID);
    }
    srEvent->scriptContext->isScriptActive = 0;
    return RUN_NEXT;
}


typedef void(*PFNSRSUB426C9B)(u16, u16, u8);
#define gameMoveActorToRestingPosition    ((PFNSRSUB426C9B)0x426C9B)
OpCodeControlSequence OpCodeA8(AnimScriptEvent* srEvent) {
    u32* off_C05FE8 = (u32*)0xC05FE8;
    u32* off_C05FEC = (u32*)0xC05FEC;
    *off_C05FE8 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C05FEC = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    gameMoveActorToRestingPosition(srEvent->actorID, *off_C05FEC, *off_C05FE8);
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeA9(AnimScriptEvent* srEvent) {
    u8* byte_C05F80 = (u8*)0xC05F80;
    u32* off_C05FE8 = (u32*)0xC05FE8;
    auto& scriptCtx = *(srEvent->scriptContext);
    auto& actorModelState = *(srEvent->battleModelState);
    auto& actorModelState74 = *getBattleModelState74(srEvent->actorID);
    auto& actorRotationData = *getBattleModelRotationData(srEvent->actorID);
    actorModelState.runningAnimIdx = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    actorModelState.currentScriptPosition++;
    actorModelState.field_74 = 0;
    actorModelState74.field_36 = 0;
    actorRotationData.field_0 = 0;
    *byte_C05F80 += 3; //Unsure why this code is needed, may modify
    *off_C05FE8 = *byte_C05F80 & 0xF;
    for (u32 frame = 0; frame < *off_C05FE8; ++frame){
        ++actorModelState74.field_36;
        srPlayModelAnimation(srEvent->actorID);
    }
    scriptCtx.isScriptActive = 0;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeAA(AnimScriptEvent* srEvent) {
    u8* byte_BF2A2C = (u8*)0xBF2A2C;
    *byte_BF2A2C = 0;
    return RUN_NEXT;
}

//Moves the actor during cover to the position of the attacker
OpCodeControlSequence OpCodeAB(AnimScriptEvent* srEvent) {
    u32* off_C05FE8 = (u32*)0xC05FE8;
    u32* off_C05FEC = (u32*)0xC05FEC;
    u8* G_ATTACKER_IDX = (u8*)0xBFCB68;
    u8* G_CURRENT_ACTION_ACTOR = (u8*)0xBE1170;
    R3PointWord* actorPositionArray = (R3PointWord*)(0xBFD0A0);
    auto& actorBattleState = *srEvent->battleModelState;
    auto& attackerModelState = *getBattleModelState(*G_ATTACKER_IDX);
    auto& scriptCtx = *(srEvent->scriptContext);

    *off_C05FE8 = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    actorBattleState.restingPosition.xCoordinate = actorPositionArray[*G_ATTACKER_IDX].x;
    actorBattleState.restingPosition.yCoordinate = actorPositionArray[*G_ATTACKER_IDX].y;
    actorBattleState.restingPosition.zCoordinate = actorPositionArray[*G_ATTACKER_IDX].z;
    actorBattleState.restingYRotation = attackerModelState.initialYRotation;
    scriptCtx.opCodeArgs[4] = *(u16*)off_C05FE8;
    scriptCtx.opCodeArgs[1] = 0;
    scriptCtx.field_8 = GAME_ANGLE_MAX - (getBattleModelState(*G_CURRENT_ACTION_ACTOR)->restingYRotation + 2048);
    scriptCtx.opCodeArgs[0] = (actorBattleState.field_6 * actorBattleState.field_12 >> 12) + (attackerModelState.field_6 * scriptCtx.opCodeArgs[4] >> 12);

    i64 xDelta = (scriptCtx.opCodeArgs[0]*(srCalculateXVectorComponent((i16)scriptCtx.field_8))) / GAME_ANGLE_MAX;
    attackerModelState.restingPosition.xCoordinate = actorBattleState.restingPosition.xCoordinate - xDelta;
    i64 zDelta = (scriptCtx.opCodeArgs[0]*(srCalculateZVectorComponent((i16)scriptCtx.field_8))) / GAME_ANGLE_MAX;
    attackerModelState.restingPosition.zCoordinate = zDelta + actorBattleState.restingPosition.zCoordinate;
    attackerModelState.restingPosition.yCoordinate = scriptCtx.opCodeArgs[1];
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeAC(AnimScriptEvent* srEvent) {
    u32* dword_9ADED8 = (u32*)0x9ADED8;
    auto& scriptCtx = *srEvent->scriptContext;
    auto& ownerModelState = *srEvent->battleModelState;
    auto& ownerModelState74 = *getBattleModelState74(srEvent->actorID);
    ownerModelState.modelEffectFlags |= 0x20u;
    ownerModelState74.field_0 = UNK_ACTOR_STRUCT_ARRAY[srEvent->actorID].field_8;
    ownerModelState.restingYRotation = ownerModelState.initialYRotation;
    scriptCtx.opCodeArgs[0] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    u8 modelIdx = 4;
    if (scriptCtx.opCodeArgs[0] == 10) {
        const auto& vincentData = getActivePartyMember(srEvent->actorID);
        u8 vincentModel = vincentData->weaponData.weapon_model & 0xF0;
        if (vincentModel == 0x10) {
            modelIdx = 5;
        }
        else if (vincentModel == 0x20) {
            modelIdx = 6;
        }
        *dword_9ADED8 = modelIdx;
    }
    else {
        *dword_9ADED8 = scriptCtx.opCodeArgs[0];
    }
    return RUN_NEXT;
}


OpCodeControlSequence OpCodeAD(AnimScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    u32* dword_9AEAF0 = (u32*)0x9AEAF0;
    u32* dword_D8E490 = (u32*)0xD8E490;
    u8* byte_BFCDE0 = (u8*)0xBFCDE0;
    scriptCtx.opCodeArgs[3] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[2] = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[0] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[1] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);


    *dword_9AEAF0 = *dword_D8E490;
    gameLoadMachineGunEffect();
    auto& effectCtx = *getEffectContext60(srPushEffect60(opCode96EffectHead));
    effectCtx.wordArray[1] = *byte_BFCDE0;
    effectCtx.wordArray[2] = srEvent->actorID;
    *(u32 *)&(effectCtx.wordArray[13]) = srEvent->battleModelState->bData0x12[scriptCtx.opCodeArgs[3]];
    effectCtx.wordArray[12] = scriptCtx.opCodeArgs[2];
    effectCtx.wordArray[3] = scriptCtx.opCodeArgs[0];
    effectCtx.wordArray[4] = scriptCtx.opCodeArgs[1];
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeAE(AnimScriptEvent* srEvent) {
    u8* G_TARGET_IDX = (u8*)0xBFB198;
    R3PointWord* actorPositionArray = (R3PointWord*)(0xBFD0A0);
    auto& targetModelState = *getBattleModelState(*G_TARGET_IDX);
    auto& targetModelState74 = *getBattleModelState74(*G_TARGET_IDX);
    targetModelState74.field_C &= 0xFFFEu;
    targetModelState.field_170 = (u32*)0xBF2168; //This is a constant pointe
    targetModelState.restingPosition.xCoordinate = actorPositionArray[*G_TARGET_IDX].x;
    targetModelState.restingPosition.yCoordinate = actorPositionArray[*G_TARGET_IDX].y;
    targetModelState.restingPosition.zCoordinate = actorPositionArray[*G_TARGET_IDX].z;
    targetModelState.restingZRotation = 0;
    targetModelState.restingYRotation = 0;
    targetModelState.restingXRotation = 0;
    targetModelState.field_25 &= 0xEFu;
    return RUN_NEXT;
}

//Used by Carry Armor
OpCodeControlSequence OpCodeAF(AnimScriptEvent* srEvent) {
    u8* G_TARGET_INDEX = (u8*)0xBFB198;
    u32* off_C05FE8 = (u32*)0xC05FE8;
    auto& targetModelState74 = *getBattleModelState74(*G_TARGET_INDEX);
    auto& targetModelState = *getBattleModelState(*G_TARGET_INDEX);
    targetModelState74.field_C |= 1;
    *off_C05FE8 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    targetModelState.field_170 = (&(srEvent->battleModelState->field_174) + 52 * (*off_C05FE8)); // this is some kind of array
    targetModelState.restingYRotation = 0;
    targetModelState.restingXRotation = 0;
    targetModelState.restingZRotation = 0;
    targetModelState.restingPosition.zCoordinate = 0;
    targetModelState.restingPosition.yCoordinate = 0;
    targetModelState.restingPosition.xCoordinate = 0;
    targetModelState.field_25 |= 0x10u;
    targetModelState.field_1AE4 = srEvent->actorID;
    targetModelState.field_1AE8 = *off_C05FE8;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeB0(AnimScriptEvent* srEvent) {
    u32* off_C05FE8 = (u32*)0xC05FE8;
    auto& battleModelState = *getBattleModelState(srEvent->actorID);
    *off_C05FE8 = battleModelState.restingYRotation + 2048;
    battleModelState.restingPosition.xCoordinate -= (258 * srCalculateXVectorComponent(*off_C05FE8)) / GAME_ANGLE_MAX;
    battleModelState.restingPosition.zCoordinate -= (258 * srCalculateZVectorComponent(*off_C05FE8)) / GAME_ANGLE_MAX;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeB1(AnimScriptEvent* srEvent) {
    u32* off_C05FE8 = (u32*)0xC05FE8;
    auto& battleModelState = *getBattleModelState(srEvent->actorID);
    *off_C05FE8 = battleModelState.restingYRotation;
    battleModelState.restingPosition.xCoordinate -= (258 * srCalculateXVectorComponent(*off_C05FE8)) / GAME_ANGLE_MAX;
    battleModelState.restingPosition.zCoordinate -= (258 * srCalculateZVectorComponent(*off_C05FE8)) / GAME_ANGLE_MAX;
    return RUN_NEXT;
}


OpCodeControlSequence OpCodeB4(AnimScriptEvent* srEvent) {
    auto& battleModelState = *getBattleModelState(srEvent->actorID);
    auto battleType = getBattleType();
    if (battleType == 2) {
        battleModelState.initialYRotation = 2048;
    }
    else if (battleType == 4) {
        battleModelState.initialYRotation = 0;
    }
    battleModelState.restingYRotation = battleModelState.initialYRotation;
    return RUN_NEXT;
}

typedef i16(*SRPFNSUB425890)(u16, u8);
#define gamePushDeathEffects  ((SRPFNSUB425890)0x425890)
OpCodeControlSequence OpCodeB7(AnimScriptEvent* srEvent) {
    gamePushDeathEffects(srEvent->actorID, 1);
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeB8(AnimScriptEvent* srEvent) {
    auto& battleModelState = *getBattleModelState(srEvent->actorID);
    battleModelState.field_25 &= 0x7Fu;
    battleModelState.field_25 |= 4u;
    return RUN_NEXT;
}

typedef void(*SRPFNSUB430AA7)();
#define setupAnimationCameraData ((SRPFNSUB430AA7)0x430AA7)
OpCodeControlSequence OpCodeB9(AnimScriptEvent* srEvent) {
    u8* byte_BF211C = (u8*)0xBF211C;
    *byte_BF211C = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);;
    getAnimationEventTop()->cameraData = -2;
    setupAnimationCameraData();
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeBA(AnimScriptEvent* srEvent) {
    auto& battleModelState = *getBattleModelState(srEvent->actorID);
    auto battleType = getBattleType();
    battleModelState.restingYRotation = battleModelState.initialYRotation;
    battleModelState.restingYRotation += readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    if (battleType == 2) {
        battleModelState.initialYRotation = 2048;
    }
    else if (battleType == 4) {
        battleModelState.initialYRotation = 0;
    }
    battleModelState.restingYRotation = battleModelState.initialYRotation;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeBC(AnimScriptEvent* srEvent) {
    u8* G_IDLE_CAMERA_INDEX = (u8*)0xBFCB28;
    *G_IDLE_CAMERA_INDEX = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    return RUN_NEXT;
}

typedef void(*SRPFNSUB424FF1)(u8, u8, u16, u16);
#define gameRotateToTarget  ((SRPFNSUB424FF1)0x424FF1)
OpCodeControlSequence OpCodeBD(AnimScriptEvent* srEvent) {
    u8* byte_BF2370 = (u8*)0xBF2370;
    u8* G_TARGET_IDX = (u8*)0xBFB198;
    auto& scriptCtx = *srEvent->scriptContext;

    *byte_BF2370 = 1;
    scriptCtx.opCodeArgs[4] = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[1] = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    gameRotateToTarget(srEvent->actorID, *G_TARGET_IDX, scriptCtx.opCodeArgs[4], scriptCtx.opCodeArgs[1]);
    return RUN_NEXT;
}


OpCodeControlSequence OpCodeBF(AnimScriptEvent* srEvent) {
    u32* off_C05FE8 = (u32*)0xC05FE8;
    u32* off_C05FEC = (u32*)0xC05FEC;
    u8* byte_BFCDF0 = (u8*)0xBFCDF0;
    u16* word_BF2E08 = (u16*)0xBF2E08;

    *off_C05FE8 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C05FEC = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    byte_BFCDF0[*off_C05FE8] = *(u8*)off_C05FEC;
    if (byte_BFCDF0[0])
        *word_BF2E08 |= 1u;
    else
        *word_BF2E08 &= 0xFFFEu;
    return RUN_NEXT;
}

// Jump to first C9
OpCodeControlSequence OpCodeC1(AnimScriptEvent* srEvent) {
    auto& ownerModelState = *srEvent->battleModelState;
    u8 position = 0;
    ownerModelState.currentScriptPosition = 0;
    do {
        position = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    } while (position != 0xC9);

    return RUN_NEXT;
}

typedef void(*PFNSRSUB425D25)();
#define gamePushDisplayDamage ((PFNSRSUB425D25)0x425D25)

OpCodeControlSequence OpCodeC2(AnimScriptEvent* srEvent) {
    u32* off_C05FE8 = (u32*)0xC05FE8;
    u32* off_C05FEC = (u32*)0xC05FEC;
    u8* byte_BFCDE0 = (u8*)0xBFCDE0;

    *off_C05FE8 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C05FEC = srPushEffect100(gamePushDisplayDamage);
    auto& effectCtx = *getEffectContext100(*off_C05FEC);
    effectCtx.wordArray[3] = *byte_BFCDE0;
    auto& displayEvent = *getFirstTargetDisplayEvent(*byte_BFCDE0);
    effectCtx.wordArray[4] = displayEvent.dealtDamage;
    effectCtx.wordArray[6] = displayEvent.actionFlags;
    effectCtx.wordArray[2] = displayEvent.impactEventQueueIdx;
    effectCtx.byteArray[23] = displayEvent.damageEventQueueIdx;
    effectCtx.wordArray[1] = *(u16*)off_C05FE8;
    return RUN_NEXT;
}

#define sub_5BBF31       ((PFNSREFFECTCALLBACK)0x5BBF31)
OpCodeControlSequence OpCodeC3(AnimScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    auto& ownerModelState74 = *getBattleModelState74(srEvent->actorID);
    srEvent->battleModelState->field_26 = 1;
    ownerModelState74.field_3C = GAME_ANGLE_MAX;
    ownerModelState74.field_3A = GAME_ANGLE_MAX;
    ownerModelState74.field_38 = GAME_ANGLE_MAX;
    ownerModelState74.field_C |= 0x20u;
    scriptCtx.field_14 = srPushEffect10(sub_5BBF31);
    auto& effectCtx = *getEffectContext10(scriptCtx.field_14);
    effectCtx.wordArray[3] = srEvent->actorID;
    effectCtx.wordArray[4] = 0;
    effectCtx.wordArray[2] = 1;
    return RUN_NEXT;
}


#define sub_426941       ((PFNSREFFECTCALLBACK)0x426941)
typedef void(*PFNSRSUB430D32)(u16, u16, u8);
#define sub_430D32      ((PFNSRSUB430D32)0x430D32)
OpCodeControlSequence OpCodeC4(AnimScriptEvent* srEvent) {
    u32* off_C05FE8 = (u32*)0xC05FE8;
    i32* off_C05FEC = (i32*)0xC05FEC;
    u32* off_C05FF0 = (u32*)0xC05FF0;
    u32* off_C05FF4 = (u32*)0xC05FF4;
    u32* off_C05FF8 = (u32*)0xC05FF8;
    u8* byte_9ADF08 = (u8*)0x9ADF08;
    u16* word_9A88BC = (u16*)0x9A88BC;

    if (!*byte_9ADF08) {
        sub_430D32(0x185u, -1, 64); //dunno what this does
        *byte_9ADF08 = 1;
    }
    *off_C05FEC = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C05FF0 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C05FF4 = srPushEffect10(sub_426941);
    auto& effectCtx = *getEffectContext10(*off_C05FF4);
    effectCtx.wordArray[3] = srEvent->actorID;
    effectCtx.wordArray[4] = 0;
    effectCtx.wordArray[1] = *(u16*)off_C05FF0;
    switch (getBattleType()) {
    case 0:
    case 1:
    case 8:
        *off_C05FF8 = *off_C05FEC;
        break;
    case 2:
        *off_C05FF8 = -*off_C05FEC;
        break;
    case 3:
    case 5:
    case 6:
    case 7:
        if (srEvent->battleModelState->initialYRotation)
            *off_C05FF8 = -*off_C05FEC;
        else
            *off_C05FF8 = *off_C05FEC;
        break;
    case 4:
        if (*word_9A88BC){
            srEvent->battleModelState->restingYRotation = 0;
            *off_C05FF8 = *off_C05FEC;
        }
        else {
            srEvent->battleModelState->restingYRotation = 2048;
            *off_C05FF8 = -*off_C05FEC;
        }
        break;
    default:
        break;
    }
    effectCtx.wordArray[5] = *(u16*)off_C05FF8; 
    return RUN_NEXT;
}


OpCodeControlSequence OpCodeC5(AnimScriptEvent* srEvent) {
    u8* G_SCRIPT_WAIT_FRAMES = (u8*)0xBFD0F0;
    srEvent->battleModelState->waitFrames = *G_SCRIPT_WAIT_FRAMES;
    return RUN_NEXT;
}


OpCodeControlSequence OpCodeC6(AnimScriptEvent* srEvent) {
    u8* G_SCRIPT_WAIT_FRAMES = (u8*)0xBFD0F0;
    *G_SCRIPT_WAIT_FRAMES = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeC7(AnimScriptEvent* srEvent) {
    u8* byte_BFD0DC = (u8*)0xBFD0DC;
    auto& scriptCtx = *srEvent->scriptContext;
    scriptCtx.opCodeArgs[2] = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState) + srEvent->actorID;
    scriptCtx.opCodeArgs[3] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    auto& relActorModelState = *getBattleModelState(scriptCtx.opCodeArgs[2]);
    auto& relActorModelState74 = *getBattleModelState74(scriptCtx.opCodeArgs[2]);

    if (scriptCtx.opCodeArgs[2] < 10 && scriptCtx.opCodeArgs[2] >= 4) { // If relative index is an enemy one
        if ((relActorModelState74.field_C & 4) && (relActorModelState74.field_C & 8)) {
            if (getTargetAllActive()){
                if (*byte_BFD0DC == srEvent->actorID && relActorModelState.field_25 & 0x80){
                    relActorModelState.animScriptIndex = scriptCtx.opCodeArgs[3];
                    relActorModelState.modelEffectFlags |= 1u;
                    relActorModelState.field_26 = 0;
                    relActorModelState74.field_C |= 4u;
                }
            }
            else if (relActorModelState.field_25 & 0x80) {
                relActorModelState.animScriptIndex = scriptCtx.opCodeArgs[3];
                relActorModelState.modelEffectFlags |= 1u;
                relActorModelState.field_26 = 0;
                relActorModelState74.field_C |= 4u;
            }
        }
        else if (relActorModelState.field_25 & 0x80) {
            relActorModelState.initialYRotation = srEvent->battleModelState->initialYRotation;
            relActorModelState.restingYRotation = relActorModelState.initialYRotation;
            relActorModelState.animScriptIndex = scriptCtx.opCodeArgs[3];
            relActorModelState.modelEffectFlags |= 1u;
            relActorModelState.field_26 = 0;
            relActorModelState74.field_C |= 4u;
        }
    }
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeC8(AnimScriptEvent* srEvent) {
    u32* off_C05FE8 = (u32*)0xC05FE8;
    u32* off_C05FEC = (u32*)0xC05FEC;
    u32* off_C05FF0 = (u32*)0xC05FF0;
    u32* off_C05FF4 = (u32*)0xC05FF4;
    *off_C05FE8 = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C05FEC = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C05FF0 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);;
    *off_C05FF4 = srPushEffect10(sub_426941);
    auto& effectCtx = *getEffectContext10(*off_C05FF4);
    effectCtx.wordArray[3] = srEvent->actorID;
    effectCtx.wordArray[4] = *(u16*)off_C05FE8;
    effectCtx.wordArray[5] = *(u16*)off_C05FEC;
    effectCtx.wordArray[1] = *(u16*)off_C05FF0;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeE1(AnimScriptEvent* srEvent) {
    setModelAppaer(srEvent->actorID);
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeE2(AnimScriptEvent* srEvent) {
    setModelVanish(srEvent->actorID);
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeE6(AnimScriptEvent* srEvent) {
    auto effectCtx = srCreateEffect(gamePushSpellAura, EFFECT60);
    effectCtx->wordArray[0] = 0;
    effectCtx->wordArray[1] = 13;
    return RUN_NEXT;
}


OpCodeControlSequence OpCodeE8(AnimScriptEvent* srEvent) {
    auto& modelState = *srEvent->battleModelState;
    auto& modelState74 = *getBattleModelState74(srEvent->actorID);
    srLoadActionSpellEffects(srEvent->actorID, modelState.commandID, modelState74.actionIdx);
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeEB(AnimScriptEvent* srEvent) {
    u8* G_EFFECT_LOADING = (u8*)0xBF2A30;
    auto& modelState = *srEvent->battleModelState;
    auto& modelState74 = *getBattleModelState74(srEvent->actorID);
    if (*G_EFFECT_LOADING) {
        modelState.currentScriptPosition--;
        srEvent->scriptContext->isScriptActive = 0;
        return RUN_NEXT;
    }
    srLogWrite("Dispatching actions for actor: d, command %d, actionId: %d",
        srEvent->actorID,
        modelState.commandID,
        modelState74.actionIdx
    );
    srDispatchActionSpellEffects(srEvent->actorID, modelState.commandID, modelState74.actionIdx);
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeEC(AnimScriptEvent* srEvent) {
    u8* G_EFFECT_LOADING = (u8*)0xBF2A30;
    u8* byte_9ADF04 = (u8*)0x9ADF04; // now that I've made dispatch generic this byte is the only differrence between the opcodes EB & EC
    auto& modelState = *srEvent->battleModelState;
    auto& modelState74 = *getBattleModelState74(srEvent->actorID);
    if (*G_EFFECT_LOADING) {
        modelState.currentScriptPosition--;
        srEvent->scriptContext->isScriptActive = 0;
        *byte_9ADF04 = 1;
        return RUN_NEXT;
    }
    srLogWrite("Dispatching actions for actor: d, command %d, actionId: %d",
        srEvent->actorID,
        modelState.commandID,
        modelState74.actionIdx
    );
    srDispatchActionSpellEffects(srEvent->actorID, modelState.commandID, modelState74.actionIdx);
    *byte_9ADF04 = 0;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeEE(AnimScriptEvent* srEvent) {
    u8* G_ACTOR_IDLE_SCRIPTS = (u8*)0xBF2DF8;
    auto& scriptCtx = *srEvent->scriptContext;
    auto& actorModelState74 = *getBattleModelState74(srEvent->actorID);
    auto& actorModelState = *getBattleModelState(srEvent->actorID);
    actorModelState74.field_C &= 0xFFFBu;
    actorModelState.field_26 = 1;
    actorModelState74.field_C &= 0xFFF7u;
    actorModelState.animScriptIndex = G_ACTOR_IDLE_SCRIPTS[srEvent->actorID];
    scriptCtx.scriptPtr = srEvent->animationScriptTable[actorModelState.animScriptIndex];
    actorModelState.isScriptExecuting;
    actorModelState.currentScriptPosition = 0;
    actorModelState.waitFrames = 0;
    return RUN_NEXT;
}

#define gameFootDustCallback            ((PFNSREFFECTCALLBACK)0x5BE4E2)
OpCodeControlSequence OpCodeF0(AnimScriptEvent* srEvent) {
    auto effectCtx = srCreateEffect(gameFootDustCallback, EFFECT60);
    effectCtx->handlerIndex = srEvent->actorID;
    effectCtx->wordArray[7] = srEvent->battleModelState->field_6;
    effectCtx->wordArray[6] = effectCtx->wordArray[7];
    return RUN_NEXT;
}

/*
 case 0x9Au:
 case 0xFBu:
     v13 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->opCodeArgs[4] = v13;
     v14 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->opCodeArgs[1] = v14;
     *(_WORD*)&scriptContextPtr->field_8 = 4096 - (*(_WORD*)&smallModelDataArray[byte_BFB198].field_2C + 2048);
     scriptContextPtr->opCodeArgs[0] = (*(signed __int16*)&gBigAnimBlock[byte_BFB198].field_6
         * gBigAnimBlock[byte_BFB198].field_12 >> 12)
         + (*(signed __int16*)&gBigAnimBlock[actor_id].field_6
             * scriptContextPtr->opCodeArgs[4] >> 12);
     v15 = gBigAnimBlock[byte_BFB198].restingPosition.xCoordinate;
     gBigAnimBlock[actor_id].restingPosition.xCoordinate = v15
         - ((signed int)(scriptContextPtr->opCodeArgs[0]
             * (unsigned int)calculateXDelta(*(signed __int16*)&scriptContextPtr->field_8)) >> 12);
     v16 = gBigAnimBlock[byte_BFB198].restingPosition.zCoordinate;
     gBigAnimBlock[actor_id].restingPosition.zCoordinate = ((signed int)(scriptContextPtr->opCodeArgs[0]
         * (unsigned int)calculateZDelta(*(signed __int16*)&scriptContextPtr->field_8)) >> 12)
         + v16;
     gBigAnimBlock[actor_id].restingPosition.yCoordinate = scriptContextPtr->opCodeArgs[1];
     goto LABEL_20;
 case 0xA2u:
     scriptContextPtr->opCodeArgs[2] = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     if (vincentTransformationIndex < 4)
         setPlayerModelData(0, 1, actor_id, (&vincentTransformationNames)[vincentTransformationIndex]);
     else
         setPlayerModelData(0, 0, actor_id, (&vincentTransformationNames)[vincentTransformationIndex]);
     if (BATTLE_MODEL_PTRS[actor_id])
         gBigAnimBlock[actor_id].modelDataPtr = BATTLE_MODEL_PTRS[actor_id];
     gBigAnimBlock[actor_id].executingAnimation = 0;
     gBigAnimBlock[actor_id].playedAnimFrames = 0;
     gBigAnimBlock[actor_id].currentPlayingFrame = 0;
     gBigAnimBlock[actor_id].tableRelativeModelAnimIdx = 0;
     gBigAnimBlock[actor_id].setForLimitBreaks = 0;
     sub_42C21B(actor_id);
     flagImmuneToStatuses(actor_id);
     newModelDataPtr = BATTLE_MODEL_PTRS[actor_id]->animationScriptsStruct;// get the start value
     for (vinAnimScriptIdx = 0; vinAnimScriptIdx < 74; ++vinAnimScriptIdx)// modify the script ptrs to be block relative
         newModelDataPtr[vinAnimScriptIdx + 26] += BATTLE_MODEL_PTRS[actor_id]->animationScriptsStruct;
     copyPlayerDataFromBFile(actor_id);
     gBigAnimBlock[actor_id].animScriptIndex = scriptContextPtr->opCodeArgs[2];
     gUnkBattleStructArray[actor_id].field_1 = gBigAnimBlock[actor_id].animScriptIndex;
     actorIdleAnimScripts[actor_id] = gUnkBattleStructArray[actor_id].field_1;
     gBigAnimBlock[actor_id].modelEffectFlags |= 1u;
     gBigAnimBlock[actor_id].modelEffectFlags |= 4u;
     gBigAnimBlock[actor_id].field_25 &= 0xFDu;
     sub_430D32(0x2D9u, actor_id, 0);
     return;
 case 0xA3u:
     v34 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     sub_740D80(0xA4u, v34, 0, 0, 0, 0);
     goto LABEL_20;
 case 0xB2u:
 case 0xC9u:
 case 0xF2u:
     goto LABEL_20;
 case 0xB3u:
     if (*(_DWORD*)&smallModelDataArray[actor_id].field_0 & 0x1000)
     {
         do
             v45 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
         while (v45 != 178);
     }
     goto LABEL_20;
 case 0xB4u:
     goto LABEL_47;
 case 0xB5u:
     v56 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->field_16 = v56;
     v57 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->field_18 = v57;
     v58 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->field_1A = v58;
     scriptContextPtr->opCodeArgs[0] = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     v59 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->opCodeArgs[1] = v59;
     v60 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->opCodeArgs[2] = v60;
     sub_5BE844(
         actor_id,
         &scriptContextPtr->field_16,
         scriptContextPtr->opCodeArgs[0],
         scriptContextPtr->opCodeArgs[1],
         scriptContextPtr->opCodeArgs[2]);
     goto LABEL_20;
 case 0xB6u:
     setCastEffectHandler((int)sub_42517B);
     gBigAnimBlock[actor_id].executingAnimation = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *(_DWORD*)&gBigAnimBlock[actor_id].field_74 = 0;
     dword_BE0E28[16 * actor_id] = 0;
     if (smallModelDataArray[actor_id].field_36)
     {
         for (vinAnimScriptIdx = 0;
             vinAnimScriptIdx < (unsigned __int8)smallModelDataArray[actor_id].field_36;
             ++vinAnimScriptIdx)
         {
             playModelAnimation(actor_id);
         }
     }
     scriptContextPtr->isScriptActive = 0;
     goto LABEL_20;
 case 0xBDu:
     byte_BF2370 = 1;
     v26 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->opCodeArgs[4] = v26;
     v27 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->opCodeArgs[1] = v27;
     sub_424FF1(actor_id, byte_BFB198, scriptContextPtr->opCodeArgs[4], scriptContextPtr->opCodeArgs[1]);
     goto LABEL_20;
 case 0xBEu:
     byte_BF23BC = 1;                    // multi-hit damage queue
     *newAxisPosition = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     if (gUnkBattleStructArray[actorID_0].characterID != 2
         || *newAxisPosition != 8
         || !(gUnkBattleStructArray[byte_BFCDE0].field_8 & 0x800))
     {
         delayedEffectHandlerPush100(currentTargetMask, *newAxisPosition, 1);
     }
     goto LABEL_20;
 case 0xBFu:
     *newAxisPosition = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C05FEC = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     byte_BFCDF0[*newAxisPosition] = *(_BYTE*)off_C05FEC;
     sub_4254E8();
     goto LABEL_20;
 case 0xC1u:
     gBigAnimBlock[actor_id].currentScriptPosition = 0;
     do
         v46 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     while (v46 != 201);
     goto LABEL_20;
 case 0xC2u:
     *newAxisPosition = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C05FEC = pushEffect100Handler(sub_425D29);
     struct100ContextArray[*off_C05FEC].targetMask = byte_BFCDE0;
     vinAnimScriptIdx = (signed __int16)sub_42DE25(byte_BFCDE0);
     struct100ContextArray[*off_C05FEC].field_A = word_BF2A40[6 * vinAnimScriptIdx];
     struct100ContextArray[*off_C05FEC].field_E = word_BF2A42[6 * vinAnimScriptIdx];
     struct100ContextArray[*off_C05FEC].field_6 = word_BF2A48[6 * vinAnimScriptIdx];
     byte_BFB731[32 * *off_C05FEC] = word_BF2A4A[6 * vinAnimScriptIdx];
     struct100ContextArray[*off_C05FEC].framesToWait = *(_WORD*)newAxisPosition;
     goto LABEL_20;
 case 0xC3u:
     gBigAnimBlock[actor_id].field_26 = 1;
     sub_425628(actor_id);
     v24 = sub_5BECF1((int)sub_5BBF31);
     scriptContextPtr->field_14 = v24;
     word_BF2E78[16 * scriptContextPtr->field_14] = actor_id;
     word_BF2E7A[16 * scriptContextPtr->field_14] = 0;
     word_BF2E76[16 * scriptContextPtr->field_14] = 1;
     goto LABEL_20;
 case 0xC4u:
     if (!byte_9ADF08)
     {
         sub_430D32(0x185u, -1, 64);
         byte_9ADF08 = 1;
     }
     v25 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     *off_C05FEC = v25;
     *off_C05FF0 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C05FF4 = sub_5BECF1((int)sub_426941);
     word_BF2E78[16 * *off_C05FF4] = actor_id;
     word_BF2E7A[16 * *off_C05FF4] = 0;
     word_BF2E74[16 * *off_C05FF4] = *(_WORD*)off_C05FF0;
     switch (byte_BFD0E0)
     {
     case 0:
     case 1:
     case 8:
         *off_C05FF8 = *off_C05FEC;
         break;
     case 2:
         *off_C05FF8 = -*off_C05FEC;
         break;
     case 3:
     case 5:
     case 6:
     case 7:
         if (gBigAnimBlock[actor_id].field_18)
             *off_C05FF8 = -*off_C05FEC;
         else
             *off_C05FF8 = *off_C05FEC;
         break;
     case 4:
         if (word_9A88BC)
         {
             gBigAnimBlock[actor_id].field_160 = 0;
             *off_C05FF8 = *off_C05FEC;
         }
         else
         {
             gBigAnimBlock[actor_id].field_160 = 2048;
             *off_C05FF8 = -*off_C05FEC;
         }
         break;
     default:
         break;
     }
     word_BF2E7C[16 * *off_C05FF4] = *(_WORD*)off_C05FF8;
     goto LABEL_20;
 case 0xC5u:
     gBigAnimBlock[actor_id].waitFrames = byte_BFD0F0;
     goto LABEL_20;
 case 0xC6u:
     byte_BFD0F0 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     goto LABEL_20;
 case 0xC7u:
     v7 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->opCodeArgs[2] = v7 + actor_id;
     scriptContextPtr->opCodeArgs[3] = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     if (scriptContextPtr->opCodeArgs[2] < 10 && scriptContextPtr->opCodeArgs[2] >= 4)
     {
         if (smallModelDataArray[actor_id].field_C & 4)
         {
             if (smallModelDataArray[actor_id].field_C & 8)
             {
                 if (isTargetingAll)
                 {
                     if (byte_BFD0DC == actor_id && gBigAnimBlock[scriptContextPtr->opCodeArgs[2]].field_25 & 0x80)
                     {
                         gBigAnimBlock[scriptContextPtr->opCodeArgs[2]].animScriptIndex = scriptContextPtr->opCodeArgs[3];
                         gBigAnimBlock[scriptContextPtr->opCodeArgs[2]].modelEffectFlags |= 1u;
                         gBigAnimBlock[scriptContextPtr->opCodeArgs[2]].field_26 = 0;
                         smallModelDataArray[scriptContextPtr->opCodeArgs[2]].field_C |= 4u;
                     }
                 }
                 else if (gBigAnimBlock[scriptContextPtr->opCodeArgs[2]].field_25 & 0x80)
                 {
                     gBigAnimBlock[scriptContextPtr->opCodeArgs[2]].animScriptIndex = scriptContextPtr->opCodeArgs[3];
                     gBigAnimBlock[scriptContextPtr->opCodeArgs[2]].modelEffectFlags |= 1u;
                     gBigAnimBlock[scriptContextPtr->opCodeArgs[2]].field_26 = 0;
                     smallModelDataArray[scriptContextPtr->opCodeArgs[2]].field_C |= 4u;
                 }
             }
         }
         else if (gBigAnimBlock[scriptContextPtr->opCodeArgs[2]].field_25 & 0x80)
         {
             gBigAnimBlock[scriptContextPtr->opCodeArgs[2]].field_18 = gBigAnimBlock[actor_id].field_18;
             gBigAnimBlock[scriptContextPtr->opCodeArgs[2]].field_160 = gBigAnimBlock[scriptContextPtr->opCodeArgs[2]].field_18;
             gBigAnimBlock[scriptContextPtr->opCodeArgs[2]].animScriptIndex = scriptContextPtr->opCodeArgs[3];
             gBigAnimBlock[scriptContextPtr->opCodeArgs[2]].modelEffectFlags |= 1u;
             gBigAnimBlock[scriptContextPtr->opCodeArgs[2]].field_26 = 0;
             smallModelDataArray[scriptContextPtr->opCodeArgs[2]].field_C |= 4u;
         }
     }
     goto LABEL_20;
 case 0xC8u:
     v22 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     *newAxisPosition = v22;
     v23 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     *off_C05FEC = v23;
     *off_C05FF0 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C05FF4 = sub_5BECF1((int)sub_426941);
     word_BF2E78[16 * *off_C05FF4] = actor_id;
     word_BF2E7A[16 * *off_C05FF4] = *(_WORD*)newAxisPosition;
     word_BF2E7C[16 * *off_C05FF4] = *(_WORD*)off_C05FEC;
     word_BF2E74[16 * *off_C05FF4] = *(_WORD*)off_C05FF0;
     goto LABEL_20;
 case 0xCAu:
     if (byte_BF2A30)
     {
         gBigAnimBlock[actor_id].currentScriptPosition = 0;
         do
             v47 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
         while (v47 != 201);
     }
     goto LABEL_20;
 case 0xCBu:
     *newAxisPosition = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     v35 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     *off_C05FEC = v35;
     *off_C05FF0 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C06000 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C06004 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C05FF4 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C05FF8 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C05FFC = *off_C06004 & 0xFF | 32 * (*off_C06000 & 0xFF) | ((*off_C05FF0 & 0xFF) << 10);
     if (*newAxisPosition == 255)
         sub_5BF388(actor_id, 0, *off_C05FEC, *off_C05FFC, *off_C05FF4, *off_C05FF8);
     else
         sub_5BF388(actor_id, *newAxisPosition, *off_C05FEC, *off_C05FFC, *off_C05FF4, *off_C05FF8);
     goto LABEL_20;
 case 0xCCu:
     scriptContextPtr->opCodeArgs[4] = 0;
     *newAxisPosition = 0;
     scriptContextPtr->opCodeArgs[3] = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     sub_5BFEC9(currentTargetMask, &scriptContextPtr->field_16);
     scriptContextPtr->field_16 *= 2;
     scriptContextPtr->field_1A *= 2;
     setupInterpolatedMove(
         actor_id,
         scriptContextPtr->scriptPtr,
         scriptContextPtr->opCodeArgs[4],
         (int)sub_426A26,
         scriptContextPtr->opCodeArgs[3],
         &scriptContextPtr->field_16);
     goto LABEL_20;
 case 0xCEu:
     if (actor_id >= 4)
     {
         do
             v44 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
         while (v44 != 205);
     }
     goto LABEL_20;
 case 0xCFu:
     v39 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->opCodeArgs[4] = v39;
     v40 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     *off_C05FEC = v40;
     v41 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     *newAxisPosition = v41;
     *off_C05FF0 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C05FF4 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     scriptContextPtr->opCodeArgs[3] = *(_WORD*)off_C05FF4;
     setupInterpolatedMove(
         actor_id,
         scriptContextPtr->scriptPtr,
         scriptContextPtr->opCodeArgs[4],
         (int)sub_42739D,
         scriptContextPtr->opCodeArgs[3],
         &gBigAnimBlock[byte_BFB198].restingPosition.xCoordinate);
     goto LABEL_20;
 case 0xD0u:
     v43 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->opCodeArgs[4] = v43;
     *newAxisPosition = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     switch (*newAxisPosition)
     {
     case 0:
     case 1:
     case 2:
     case 3:
         scriptContextPtr->opCodeArgs[3] = 5;
         break;
     case 4:
     case 5:
     case 6:
     case 7:
         scriptContextPtr->opCodeArgs[3] = 8;
         break;
     default:
         break;
     }
     if (isTargetingAll)
     {
         sub_5BFEC9(currentTargetMask, &scriptContextPtr->field_16);
         setupInterpolatedMove(
             actor_id,
             scriptContextPtr->scriptPtr,
             scriptContextPtr->opCodeArgs[4],
             (int)sub_426F58,
             scriptContextPtr->opCodeArgs[3],
             &scriptContextPtr->field_16);
     }
     else
     {                                   // this appears to move along the ground
         setupInterpolatedMove(
             actor_id,
             scriptContextPtr->scriptPtr,
             scriptContextPtr->opCodeArgs[4],
             (int)sub_426F58,
             scriptContextPtr->opCodeArgs[3],
             &gBigAnimBlock[byte_BFB198].restingPosition.xCoordinate);
     }
     goto LABEL_20;
 case 0xD1u:
     v20 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->opCodeArgs[4] = v20;
     v21 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     *newAxisPosition = v21;
     scriptContextPtr->opCodeArgs[3] = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     if (isTargetingAll)
     {
         sub_5BFEC9(currentTargetMask, &scriptContextPtr->field_16);
         setupInterpolatedMove(
             actor_id,
             scriptContextPtr->scriptPtr,
             scriptContextPtr->opCodeArgs[4],
             (int)sub_426A26,
             scriptContextPtr->opCodeArgs[3],
             &scriptContextPtr->field_16);
     }
     else
     {
         setupInterpolatedMove(
             actor_id,
             scriptContextPtr->scriptPtr,
             scriptContextPtr->opCodeArgs[4],
             (int)sub_426A26,
             scriptContextPtr->opCodeArgs[3],
             &gBigAnimBlock[byte_BFB198].restingPosition.xCoordinate);
     }
     goto LABEL_20;
 case 0xD4u:                           // move opcode
     xCoordinate = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->opCodeArgs[4] = xCoordinate;
     scriptContextPtr->opCodeArgs[3] = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *newAxisPosition = scriptContextPtr->opCodeArgs[4] / scriptContextPtr->opCodeArgs[3];
     v9 = sub_5BECF1((int)sub_4267F1);
     scriptContextPtr->field_14 = v9;
     word_BF2E78[16 * scriptContextPtr->field_14] = actor_id;
     word_BF2E7A[16 * scriptContextPtr->field_14] = *(_WORD*)newAxisPosition;
     word_BF2E74[16 * scriptContextPtr->field_14] = scriptContextPtr->opCodeArgs[3];
     goto LABEL_20;
 case 0xD5u:
     v36 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->opCodeArgs[4] = v36;
     v37 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     *off_C05FEC = v37;
     v38 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     *newAxisPosition = v38;
     *off_C05FF0 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C05FF4 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     scriptContextPtr->opCodeArgs[3] = *(_WORD*)off_C05FF4;
     setupInterpolatedMove(
         actor_id,
         scriptContextPtr->scriptPtr,
         scriptContextPtr->opCodeArgs[4],
         (int)sub_4270DE,
         scriptContextPtr->opCodeArgs[3],
         &gBigAnimBlock[byte_BFB198].restingPosition.xCoordinate);
     goto LABEL_20;
 case 0xD6u:
     scriptContextPtr->opCodeArgs[0] = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     v62 = setCastEffectHandler((int)sub_4277B1);
     scriptContextPtr->opCodeArgs[1] = v62;
     word_BFC3A6[16 * scriptContextPtr->opCodeArgs[1]] = actor_id;
     word_BFC3A4[16 * scriptContextPtr->opCodeArgs[1]] = scriptContextPtr->opCodeArgs[0];
     goto LABEL_20;
 case 0xD7u:
     *newAxisPosition = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C05FEC = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     v48 = setCastEffectHandler((int)sub_4276B6);
     scriptContextPtr->opCodeArgs[0] = v48;
     word_BFC3A4[16 * scriptContextPtr->opCodeArgs[0]] = *(_WORD*)newAxisPosition;
     word_BFC3A6[16 * scriptContextPtr->opCodeArgs[0]] = *(_WORD*)off_C05FEC;
     word_BFC3A8[16 * scriptContextPtr->opCodeArgs[0]] = actor_id;
     goto LABEL_20;
 case 0xD8u:                           // play Sound
     *newAxisPosition = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     v49 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     *off_C05FEC = v49;
     v50 = setCastEffectHandler((int)sub_427737);
     scriptContextPtr->opCodeArgs[0] = v50;
     word_BFC3A4[16 * scriptContextPtr->opCodeArgs[0]] = *(_WORD*)newAxisPosition;
     word_BFC3A6[16 * scriptContextPtr->opCodeArgs[0]] = *(_WORD*)off_C05FEC;
     goto LABEL_20;
 case 0xDAu:
     gBigAnimBlock[actor_id].additionalEffect = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     gBigAnimBlock[actor_id].commandID = 2;
     goto LABEL_20;
 case 0xDBu:
     v55 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->opCodeArgs[2] = v55;
     scriptContextPtr->opCodeArgs[0] = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     scriptContextPtr->opCodeArgs[1] = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     sub_43930D(
         byte_BFCDE0,
         actor_id,
         -1,
         scriptContextPtr->opCodeArgs[2],
         scriptContextPtr->opCodeArgs[0],
         scriptContextPtr->opCodeArgs[1]);
     goto LABEL_20;
 case 0xDCu:
     scriptContextPtr->opCodeArgs[0] = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     v53 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     *(&gBigAnimBlock[actor_id].field_1A + scriptContextPtr->opCodeArgs[0]) = *(signed __int16*)&gBigAnimBlock[actor_id].field_6
         * v53 >> 12;
     goto LABEL_20;
 case 0xDDu:
     scriptContextPtr->opCodeArgs[0] = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     scriptContextPtr->opCodeArgs[1] = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     sub_439946(byte_BFCDE0, actor_id, scriptContextPtr->opCodeArgs[0], scriptContextPtr->opCodeArgs[1]);
     goto LABEL_20;
 case 0xDEu:
     scriptContextPtr->opCodeArgs[0] = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     scriptContextPtr->opCodeArgs[1] = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     sub_439907(byte_BFCDE0, actor_id, scriptContextPtr->opCodeArgs[0], scriptContextPtr->opCodeArgs[1]);
     goto LABEL_20;
 case 0xDFu:
     sub_5BFEC9(currentTargetMask, &scriptContextPtr->field_16);
     v10 = scriptContextPtr->field_1A - gBigAnimBlock[actor_id].restingPosition.zCoordinate;
     gBigAnimBlock[actor_id].field_160 = (unsigned __int64)sub_662573(
         scriptContextPtr->field_16
         - (signed int)gBigAnimBlock[actor_id].restingPosition.xCoordinate)
         + 2048;
     goto LABEL_20;
 case 0xE0u:                           // limit charge effect
     vinAnimScriptIdx = setCastEffectHandler((int)sub_425520);
     word_BFC3A2[16 * vinAnimScriptIdx] = 1;
     word_BFC3A4[16 * vinAnimScriptIdx] = 36;
     goto LABEL_20;
 case 0xE3u:
     actorPositionArray[actorID_0].xCoordinate = gBigAnimBlock[actorID_0].restingPosition.xCoordinate;
     actorPositionArray[actorID_0].zCoordinate = gBigAnimBlock[actorID_0].restingPosition.zCoordinate;
     goto LABEL_20;
 case 0xE4u:                           // called when Change is issued, changes resting model position
     *newAxisPosition = gBigAnimBlock[actor_id].field_160 + 2048;
     gBigAnimBlock[actor_id].restingPosition.xCoordinate -= (signed int)(516
         * (unsigned __int64)calculateXDelta(*newAxisPosition)) >> 12;
     gBigAnimBlock[actor_id].restingPosition.zCoordinate -= (signed int)(516
         * (unsigned __int64)calculateZDelta(*newAxisPosition)) >> 12;
     goto LABEL_20;
 case 0xE5u:
     switch (byte_BFD0E0)
     {
     case 1:
         if (actor_id >= 4)
             gBigAnimBlock[actor_id].field_18 = 2048;
         break;
     case 2:
         if (actor_id < 4)
             gBigAnimBlock[actor_id].field_18 = 2048;
         break;
     case 3:
     case 5:
     case 6:
         if (actor_id >= 4)
         {
             if (actor_id == actorID_0)
             {
                 if (gBigAnimBlock[byte_BFCDE0].field_18)
                     gBigAnimBlock[actor_id].field_18 = 0;
                 else
                     gBigAnimBlock[actor_id].field_18 = 2048;
             }
             else if (gBigAnimBlock[actorID_0].field_18)
             {
                 gBigAnimBlock[actor_id].field_18 = 0;
             }
             else
             {
                 gBigAnimBlock[actor_id].field_18 = 2048;
             }
         }
         break;
     case 4:
         if (actor_id < 4 && (actor_id != byte_BFCDE0 || actor_id != actorID_0) && !isTargetingAll)
         {
             if (actor_id == actorID_0)
             {
                 if (gBigAnimBlock[byte_BFCDE0].field_18)
                     gBigAnimBlock[actor_id].field_18 = 0;
                 else
                     gBigAnimBlock[actor_id].field_18 = 2048;
             }
             else if (gBigAnimBlock[actorID_0].field_18)
             {
                 gBigAnimBlock[actor_id].field_18 = 0;
             }
             else
             {
                 gBigAnimBlock[actor_id].field_18 = 2048;
             }
         }
         break;
     default:
         break;
     }
     gBigAnimBlock[actor_id].field_160 = gBigAnimBlock[actor_id].field_18;
     goto LABEL_20;
 case 0xE7u:
     *off_C05FF0 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     if (*off_C05FF0)
         v42 = gBigAnimBlock[actor_id].field_25 & 0xFB;
     else
         v42 = gBigAnimBlock[actor_id].field_25 | 4;
     gBigAnimBlock[actor_id].field_25 = v42;
     goto LABEL_20;
 case 0xE9u:
     v17 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     scriptContextPtr->opCodeArgs[4] = v17;
     scriptContextPtr->opCodeArgs[3] = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *newAxisPosition = smallModelDataArray[actor_id].field_28;
     setupInterpolatedMove(
         actor_id,
         scriptContextPtr->scriptPtr,
         scriptContextPtr->opCodeArgs[4],
         (int)sub_426A26,
         scriptContextPtr->opCodeArgs[3],
         &gBigAnimBlock[byte_BFB198].restingPosition.xCoordinate);
     goto LABEL_20;
 case 0xEAu:                           // displayActionName
     v65 = pushEffect100Handler(functionAddr);
     scriptContextPtr->field_14 = v65;
     v66 = sub_5BE475();
     struct100ContextArray[scriptContextPtr->field_14].framesToWait = v66;
     struct100ContextArray[scriptContextPtr->field_14].field_6 = 0;
     goto LABEL_20;
 case 0xEDu:                           // opcode hops forward
     *newAxisPosition = gBigAnimBlock[actor_id].field_160;
     gBigAnimBlock[actor_id].restingPosition.xCoordinate -= (signed int)(516
         * (unsigned __int64)calculateXDelta(*newAxisPosition)) >> 12;
     gBigAnimBlock[actor_id].restingPosition.zCoordinate -= (signed int)(516
         * (unsigned __int64)calculateZDelta(*newAxisPosition)) >> 12;
     goto LABEL_20;
 case 0xF1u:
     smallModelDataArray[actor_id].field_C &= 0xFFFBu;
     gBigAnimBlock[actor_id].field_26 = 1;
     v51 = smallModelDataArray[actor_id].field_C;
     LOBYTE(v51) = v51 & 0xF7;
     smallModelDataArray[actor_id].field_C = v51;
     --gBigAnimBlock[actor_id].currentScriptPosition;
     return;
 case 0xF3u:                           // decrement wait timer
     if (!gBigAnimBlock[actor_id].waitFrames)
         goto LABEL_20;
     --gBigAnimBlock[actor_id].waitFrames;
     --gBigAnimBlock[actor_id].currentScriptPosition;
     scriptContextPtr->isScriptActive = 0;
     return;
 case 0xF4u:
     gBigAnimBlock[actor_id].waitFrames = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     break;
 case 0xF5u:
     *off_C05FF4 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     EnemyAnimScriptIndex[6 * (actor_id - 4)] = *(_WORD*)off_C05FF4 + 3;
     initEnemyModelState(actor_id);
     gBigAnimBlock[actor_id].modelEffectFlags |= 1u;
     break;
 case 0xF6u:
     if (byte_BFC398)
     {
         if (smallModelDataArray[actor_id].field_C & 0x10)
             sub_425890(actor_id, 1);
     }
     else if ((1 << actor_id) & word_BFCB20)
     {
         sub_425890(actor_id, 1);
     }
     break;
 case 0xF7u:                           // display damage in arg frame
     byte_BF23BC = 0;
     *newAxisPosition = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     delayedEffectHandlerPush100(currentTargetMask, *newAxisPosition, 1);
     break;
 case 0xF8u:
     *newAxisPosition = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     v3 = setCastEffectHandler((int)sub_425AAD);
     scriptContextPtr->opCodeArgs[0] = v3;
     if (actor_id >= 4)
         *off_C05FF0 = EnemyAnimScriptIndex[6 * (actor_id - 4)];
     else
         *off_C05FF0 = actor_id;
     v84 = BATTLE_MODEL_PTRS[*off_C05FF0]->animationScriptsStruct;
     dword_BFC3B0[8 * scriptContextPtr->opCodeArgs[0]] = v84[*newAxisPosition + 9];
     word_BFC3A6[16 * scriptContextPtr->opCodeArgs[0]] = actor_id;
     byte_BFC3B8[32 * scriptContextPtr->opCodeArgs[0]] = 0;
     word_BFC3A4[16 * scriptContextPtr->opCodeArgs[0]] = 0;
     word_BFC3A2[16 * scriptContextPtr->opCodeArgs[0]] = (unsigned __int8)++smallModelDataArray[actor_id].field_34;
     break;
 case 0xF9u:
     gBigAnimBlock[actor_id].field_160 = gBigAnimBlock[actor_id].field_18;
     break;
 case 0xFAu:
     sub_425119(actor_id);
     break;
 case 0xFCu:
     if (isTargetingAll)
     {
         switch (byte_BFD0E0)
         {
         case 3:
         case 5:
         case 6:
         case 7:
             for (vinAnimScriptIdx = 4; vinAnimScriptIdx < 10; ++vinAnimScriptIdx)
             {
                 if (gBigAnimBlock[actor_id].field_18)
                     gBigAnimBlock[vinAnimScriptIdx].field_18 = 0;
                 else
                     gBigAnimBlock[vinAnimScriptIdx].field_18 = 2048;
             }
             break;
         case 4:
             if (currentTargetMask & (unsigned __int16)word_9A88AA[0])
                 gBigAnimBlock[actor_id].field_160 = 0;
             if (currentTargetMask & (unsigned __int16)word_9A88AE)
                 gBigAnimBlock[actor_id].field_160 = 2048;
             break;
         default:
             goto LABEL_20;
         }
     }
     else if ((gBigAnimBlock[actor_id].additionalEffect != 21 || gBigAnimBlock[actor_id].commandID != 13)
         && actorID_0 != byte_BFB198)
     {
         v11 = gBigAnimBlock[byte_BFB198].restingPosition.zCoordinate
             - gBigAnimBlock[actor_id].restingPosition.zCoordinate;
         gBigAnimBlock[actor_id].field_160 = (unsigned __int64)sub_662573(
             gBigAnimBlock[byte_BFB198].restingPosition.xCoordinate
             - (signed int)gBigAnimBlock[actor_id].restingPosition.xCoordinate)
             + 2048;
         v12 = gBigAnimBlock[byte_BFB198].restingPosition.zCoordinate
             - gBigAnimBlock[actor_id].restingPosition.zCoordinate;
         *(_WORD*)&smallModelDataArray[byte_BFB198].field_2C = sub_662573(
             gBigAnimBlock[byte_BFB198].restingPosition.xCoordinate
             - (signed int)gBigAnimBlock[actor_id].restingPosition.xCoordinate);
     }
     break;
 case 0xFDu:
     gBigAnimBlock[actor_id].restingPosition.xCoordinate = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     gBigAnimBlock[actor_id].restingPosition.yCoordinate = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     gBigAnimBlock[actor_id].restingPosition.zCoordinate = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     break;
 case 0xFEu:
     if (!gBigAnimBlock[actor_id].waitFrames)
     {
         scriptContextPtr->currentOpCode = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition];
         if (scriptContextPtr->currentOpCode == 192)
         {
             gBigAnimBlock[actor_id].currentScriptPosition = 0;
             dword_BE0E28[16 * actor_id] = 0;
             smallModelDataArray[actor_id].field_36 = 0;
             *(_DWORD*)&gBigAnimBlock[actor_id].field_74 = 0;
             gBigAnimBlock[actor_id].waitFrames = 0;
             gBigAnimBlock[actor_id].executingAnimation = *scriptContextPtr->scriptPtr;
             gBigAnimBlock[actor_id].animScriptIndex = actorIdleAnimScripts[actor_id];
             scriptContextPtr->scriptPtr = (byte*)ptrToScriptTable[gBigAnimBlock[actor_id].animScriptIndex];
             resetScriptState(actor_id);
         }
     }
     break;
 default:                              // /run for playing a model animations
     gBigAnimBlock[actor_id].executingAnimation = scriptContextPtr->currentOpCode;
     *(_DWORD*)&gBigAnimBlock[actor_id].field_74 = 0;
     smallModelDataArray[actor_id].field_36 = 0;
     dword_BE0E28[16 * actor_id] = 0;
     scriptContextPtr->isScriptActive = 0;
     break;
                }
            }
        }*/
