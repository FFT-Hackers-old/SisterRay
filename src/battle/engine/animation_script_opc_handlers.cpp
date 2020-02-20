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
#include "../battle_models/model_setup_routines.h"


OpCodeControlSequence nopCode(AnimScriptEvent* srEvent) {
    return RUN_NEXT;
}

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
    u32* off_C0600C = (u32*)0xC0600C;
    u32* off_C06008 = (u32*)0xC06008;
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
        *off_C06008 = 0;
        *off_C0600C = scriptCtx->field_15;
        break;
    }
    case 7: {
        *off_C06008 = 0;
        *off_C0600C = scriptCtx->field_15 + 8;
        break;
    }
    case 8: {
        *off_C06008 = 1;
        *off_C0600C = scriptCtx->field_15;
        break;
    }
    case 9: {
        *off_C06008 = 1;
        *off_C0600C = scriptCtx->field_15 + 8;
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
    auto unkPaletteSwapArg = *off_C0600C;
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
    u32* off_C06008 = (u32*)0xC06008;
    auto scriptCtx = srEvent->scriptContext;
    auto scriptPtr = srEvent->scriptPtr;
    auto actorID = srEvent->actorID;
    *off_C06008 = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID));
    scriptCtx->field_14 = srPushEffect60(opCode91EffectHead);
    auto& effectCtx = *getEffectContext60(scriptCtx->field_14);
    effectCtx.wordArray[1] = *(u16*)off_C06008;
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
    u32* off_C06008 = (u32*)0xC06008;
    u32* off_C0600C = (u32*)0xC0600C;
    u32* off_C06010  = (u32*)0xC06010 ;
    auto scriptCtx = srEvent->scriptContext;
    auto scriptPtr = srEvent->scriptPtr;
    auto actorID = srEvent->actorID;

    *off_C06008 = readOpCodeArg16(scriptPtr, scriptCtx, getBattleModelState(actorID));
    *off_C0600C = readOpCodeArg16(scriptPtr, scriptCtx, getBattleModelState(actorID));
    *off_C06010  = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID));;

    scriptCtx->field_14 = srPushEffect10(opCode94EffectHead);
    auto& effectCtx = *getEffectContext60(scriptCtx->field_14);
    effectCtx.wordArray[3] = actorID;
    effectCtx.wordArray[4] = *(u16*)off_C06008;
    effectCtx.wordArray[5] = *(u16*)off_C0600C;
    effectCtx.wordArray[6] = (*off_C0600C - *off_C06008) / *off_C06010 ;
    effectCtx.wordArray[1] = *(u16*)off_C06010 ;
    return RUN_NEXT;
}

OpCodeControlSequence OpCode95(AnimScriptEvent* srEvent) {
    auto& actorModelState = *getBattleModelState(srEvent->actorID);
    if (getBattleType() != 1 && getBattleType() == 2 && srEvent->actorID < 4) {
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
    auto actorData = getGamePartyMember(actorID);
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
    u32* off_C06008 = (u32*)0xC06008;
    u32* off_C0600C = (u32*)0xC0600C;
    auto scriptCtx = srEvent->scriptContext;
    auto scriptPtr = srEvent->scriptPtr;
    auto actorID = srEvent->actorID;

    *off_C06008 = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID));
    *off_C0600C = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID));
    sub_42567E(actorID, *(u8*)off_C06008, *(u8*)off_C0600C);
    return RUN_NEXT;
}

//Displays strings with a delay
OpCodeControlSequence OpCode98(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    u32* off_C0600C = (u32*)0xC0600C;
    u8* byte_DC0E11 = (u8*)0xDC0E11;
    auto scriptCtx = srEvent->scriptContext;
    auto scriptPtr = srEvent->scriptPtr;
    auto actorID = srEvent->actorID;

    *off_C06008 = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID)); //number of frames to wait
    scriptCtx->field_14 = srPushEffect100((PFNSREFFECTCALLBACK)srDisplayActionStringEffect);;
    auto& effectCtx = *getEffectContext100(scriptCtx->field_14);
    effectCtx.wordArray[1] = ((u16)(*byte_DC0E11 >> 2)) + 4; //number of frames to display string
    effectCtx.wordArray[2] = *(u16*)off_C06008; //delay before display begins
    return RUN_NEXT;
}

#define moveCallback426A26  ((PFNSREFFECTCALLBACK)0x426A26)
OpCodeControlSequence OpCode99(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    u32* off_C0600C = (u32*)0xC0600C;
    u8* byte_BE10B4 = (u8*)0xBE10B4;
    u8* isTargetingAll = (u8*)0xBF2E1C;
    u16* currentTargetMask = (u16*)0xBFD0F8;
    auto scriptCtx = srEvent->scriptContext;
    auto scriptPtr = srEvent->scriptPtr;
    auto actorID = srEvent->actorID;

    *byte_BE10B4 = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID));
    scriptCtx->opCodeArgs[4] = readOpCodeArg16(scriptPtr, scriptCtx, getBattleModelState(actorID));
    *off_C06008 = readOpCodeArg16(scriptPtr, scriptCtx, getBattleModelState(actorID));
    scriptCtx->opCodeArgs[3] = readOpCodeArg8(scriptPtr, scriptCtx, getBattleModelState(actorID));
    if (*isTargetingAll) {
        srComputeEnemyPartyCenter(*currentTargetMask, (R3PointWord*)&scriptCtx->field_16);
        srDispatchMoveEffects(
            actorID,
            scriptPtr,
            scriptCtx->opCodeArgs[4],
            moveCallback426A26,
            scriptCtx->opCodeArgs[3],
            (R3PointWord*)&(scriptCtx->field_16));
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
            (R3PointWord*)&(getBattleModelState(targetID))->restingPosition);
    }
    return RUN_NEXT;
}

OpCodeControlSequence OpCode9A(AnimScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    auto& ownerModelState = *srEvent->battleModelState;
    auto& targetModelState = *getBattleModelState(getAnimatingActionTargetIdx());
    auto& targetModelState74 = *getBattleModelState74(getAnimatingActionTargetIdx());

    scriptCtx.opCodeArgs[4] = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[1] = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.field_8 = GAME_ANGLE_MAX - (targetModelState74.field_2C + 2048);
    scriptCtx.opCodeArgs[0] = (targetModelState.field_6 * targetModelState.field_12 / GAME_ANGLE_MAX);

    i16 xDecrement = (i16)((scriptCtx.opCodeArgs[0] * srCalculateXVectorComponent(scriptCtx.field_8)) / GAME_ANGLE_MAX);
    ownerModelState.restingPosition.x = targetModelState.restingPosition.x - xDecrement;
    i16 zDecrement = (i16)((scriptCtx.opCodeArgs[0] * srCalculateZVectorComponent(scriptCtx.field_8)) / GAME_ANGLE_MAX);
    ownerModelState.restingPosition.z = targetModelState.restingPosition.z - zDecrement;
    ownerModelState.restingPosition.y = scriptCtx.opCodeArgs[1];
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
    u32* off_C06008 = (u32*)0xC06008;
    auto scriptCtx = srEvent->scriptContext;
    auto actorID = srEvent->actorID;
    u16* currentTargetMask = (u16*)0xBFD0F8;

    *off_C06008 = readOpCodeArg8(srEvent->scriptPtr, scriptCtx, getBattleModelState(srEvent->actorID));
    switch (*off_C06008) {
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
    u32* off_C06008 = (u32*)0xC06008;
    auto& targetModelState74 = *getBattleModelState74(*G_TARGET_INDEX);
    auto& targetModelState = *getBattleModelState(*G_TARGET_INDEX);
    targetModelState74.field_C |= 1;
    *off_C06008 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    targetModelState.field_170 = (&(srEvent->battleModelState->field_174) + 52 * (*off_C06008)); // this is some kind of array
    targetModelState.restingYRotation = 0;
    targetModelState.restingXRotation = 0;
    targetModelState.restingPosition.z = 0;
    targetModelState.restingPosition.y = 0;
    targetModelState.restingPosition.x = 0;
    targetModelState.restingZRotation = 2048;
    targetModelState.field_25 |= 0x10u;
    targetModelState.field_1AE4 = srEvent->actorID;
    targetModelState.field_1AE8 = *off_C06008;
    return RUN_NEXT;
}

typedef void(*PFNSRSUB5BEB27)(u16, u16);
#define gamePushPeriodicDamage    ((PFNSRSUB5BEB27)0x5BEB27)
OpCodeControlSequence OpCodeA1(AnimScriptEvent* srEvent) {
    u8* byte_BF23BC = (u8*)0xBF23BC;
    u32* off_C06008 = (u32*)0xC06008;
    u32* off_C0600C = (u32*)0xC0600C;
    *byte_BF23BC = 0;
    *off_C06008 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C0600C = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState); //period
    gamePushPeriodicDamage(*off_C06008, *off_C0600C);
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
    attackerModelState.restingPosition.x = actorPosition.x;
    attackerModelState.restingPosition.y = actorPosition.y;
    attackerModelState.restingPosition.z = actorPosition.z;
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
    u32* off_C06008 = (u32*)0xC06008;
    u32* off_C0600C = (u32*)0xC0600C;
    *off_C06008 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C0600C = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    gameMoveActorToRestingPosition(srEvent->actorID, *off_C0600C, *off_C06008);
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeA9(AnimScriptEvent* srEvent) {
    u8* byte_C05F80 = (u8*)0xC05F80;
    u32* off_C06008 = (u32*)0xC06008;
    auto& scriptCtx = *(srEvent->scriptContext);
    auto& actorModelState = *(srEvent->battleModelState);
    auto& actorModelState74 = *getBattleModelState74(srEvent->actorID);
    auto& actorRotationData = *getBattleModelRotationData(srEvent->actorID);
    actorModelState.currentScriptPosition++;
    actorModelState.runningAnimIdx = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    actorModelState.currentScriptPosition--;
    actorModelState.field_74 = 0;
    actorModelState74.field_36 = 0;
    actorRotationData.field_0 = 0;
    *byte_C05F80 += 3; //Unsure why this code is needed, may modify
    *off_C06008 = *byte_C05F80 & 0xF;
    for (u32 frame = 0; frame < *off_C06008; ++frame){
        ++actorModelState74.field_36;
        srPlayModelAnimation(srEvent->actorID);
    }
    scriptCtx.isScriptActive = 0;
    return PLAY_ANIM;
}

OpCodeControlSequence OpCodeAA(AnimScriptEvent* srEvent) {
    u8* byte_BF2A2C = (u8*)0xBF2A2C;
    *byte_BF2A2C = 0;
    return RUN_NEXT;
}

//Moves the actor during cover to the position of the attacker
OpCodeControlSequence OpCodeAB(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    u32* off_C0600C = (u32*)0xC0600C;
    u8* G_ATTACKER_IDX = (u8*)0xBFCB68;
    u8* G_CURRENT_ACTION_ACTOR = (u8*)0xBE1170;
    R3PointWord* actorPositionArray = (R3PointWord*)(0xBFD0A0);
    auto& actorBattleState = *srEvent->battleModelState;
    auto& attackerModelState = *getBattleModelState(*G_ATTACKER_IDX);
    auto& scriptCtx = *(srEvent->scriptContext);

    *off_C06008 = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    actorBattleState.restingPosition.x = actorPositionArray[*G_ATTACKER_IDX].x;
    actorBattleState.restingPosition.y = actorPositionArray[*G_ATTACKER_IDX].y;
    actorBattleState.restingPosition.z = actorPositionArray[*G_ATTACKER_IDX].z;
    actorBattleState.restingYRotation = attackerModelState.initialYRotation;
    scriptCtx.opCodeArgs[4] = *(u16*)off_C06008;
    scriptCtx.opCodeArgs[1] = 0;
    scriptCtx.field_8 = GAME_ANGLE_MAX - (getBattleModelState(*G_CURRENT_ACTION_ACTOR)->restingYRotation + 2048);
    scriptCtx.opCodeArgs[0] = (actorBattleState.field_6 * actorBattleState.field_12 >> 12) + (attackerModelState.field_6 * scriptCtx.opCodeArgs[4] >> 12);

    i64 xDelta = (scriptCtx.opCodeArgs[0]*(srCalculateXVectorComponent((i16)scriptCtx.field_8))) / GAME_ANGLE_MAX;
    attackerModelState.restingPosition.x = actorBattleState.restingPosition.x - xDelta;
    i64 zDelta = (scriptCtx.opCodeArgs[0]*(srCalculateZVectorComponent((i16)scriptCtx.field_8))) / GAME_ANGLE_MAX;
    attackerModelState.restingPosition.z = zDelta + actorBattleState.restingPosition.z;
    attackerModelState.restingPosition.y = scriptCtx.opCodeArgs[1];
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
        const auto& vincentData = getGamePartyMember(srEvent->actorID);
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
    targetModelState.restingPosition.x = actorPositionArray[*G_TARGET_IDX].x;
    targetModelState.restingPosition.y = actorPositionArray[*G_TARGET_IDX].y;
    targetModelState.restingPosition.z = actorPositionArray[*G_TARGET_IDX].z;
    targetModelState.restingZRotation = 0;
    targetModelState.restingYRotation = 0;
    targetModelState.restingXRotation = 0;
    targetModelState.field_25 &= 0xEFu;
    return RUN_NEXT;
}

//Used by Carry Armor
OpCodeControlSequence OpCodeAF(AnimScriptEvent* srEvent) {
    u8* G_TARGET_INDEX = (u8*)0xBFB198;
    u32* off_C06008 = (u32*)0xC06008;
    auto& targetModelState74 = *getBattleModelState74(*G_TARGET_INDEX);
    auto& targetModelState = *getBattleModelState(*G_TARGET_INDEX);
    targetModelState74.field_C |= 1;
    *off_C06008 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    targetModelState.field_170 = (&(srEvent->battleModelState->field_174) + 52 * (*off_C06008)); // this is some kind of array
    targetModelState.restingYRotation = 0;
    targetModelState.restingXRotation = 0;
    targetModelState.restingZRotation = 0;
    targetModelState.restingPosition.z = 0;
    targetModelState.restingPosition.y = 0;
    targetModelState.restingPosition.x = 0;
    targetModelState.field_25 |= 0x10u;
    targetModelState.field_1AE4 = srEvent->actorID;
    targetModelState.field_1AE8 = *off_C06008;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeB0(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    auto& battleModelState = *getBattleModelState(srEvent->actorID);
    *off_C06008 = battleModelState.restingYRotation + 2048;
    battleModelState.restingPosition.x -= (258 * srCalculateXVectorComponent(*off_C06008)) / GAME_ANGLE_MAX;
    battleModelState.restingPosition.z -= (258 * srCalculateZVectorComponent(*off_C06008)) / GAME_ANGLE_MAX;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeB1(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    auto& battleModelState = *getBattleModelState(srEvent->actorID);
    *off_C06008 = battleModelState.restingYRotation;
    battleModelState.restingPosition.x -= (258 * srCalculateXVectorComponent(*off_C06008)) / GAME_ANGLE_MAX;
    battleModelState.restingPosition.z -= (258 * srCalculateZVectorComponent(*off_C06008)) / GAME_ANGLE_MAX;
    return RUN_NEXT;
}


#define sub_42517B ((PFNSREFFECTCALLBACK)0x42517B)
OpCodeControlSequence OpCodeB6(AnimScriptEvent* srEvent) {
    auto& ownerModelState = *srEvent->battleModelState;
    auto& ownerModelState74 = *getBattleModelState74(srEvent->actorID);
    auto& ownerRotationData = *getBattleModelRotationData(srEvent->actorID);
    auto& scriptCtx = *srEvent->scriptContext;

    srPushEffect60(sub_42517B);
    ownerModelState.runningAnimIdx = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    ownerModelState.field_74 = 0;
    ownerRotationData.field_0 = 0;
    if (ownerModelState74.field_36) {
        for (u16 frame = 0; frame < ownerModelState74.field_36; ++frame)
            srPlayModelAnimation(srEvent->actorID);
    }
    scriptCtx.isScriptActive = 0;
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

typedef i16(*SRPFNSUB425890)(u8, u8);
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
    *byte_BF211C = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
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


typedef void(*SRPFNSUB5BE9F0)(u16, u16, u16);
#define pushDelayedDamageDisplayEffect    ((SRPFNSUB5BE9F0)0x5BE9F0)
OpCodeControlSequence OpCodeBE(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    u8* byte_BF23BC = (u8*)0xBF23BC;
    u8* byte_BFCDE0 = (u8*)0xBFCDE0;
    *byte_BF23BC = 1;                    // multi-hit damage queue
    *off_C06008 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    bool isNotTifa = UNK_ACTOR_STRUCT_ARRAY[getActionActorIdx()].characterID != ACT_IDX_TIFA;
    if (isNotTifa || *off_C06008 != 8 || !(UNK_ACTOR_STRUCT_ARRAY[*byte_BFCDE0].field_8 & 0x800)) {
        pushDelayedDamageDisplayEffect(getAnimatingActionTargetMask(), *off_C06008, 1);
    }
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeBF(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    u32* off_C0600C = (u32*)0xC0600C;
    u8* byte_BFCDF0 = (u8*)0xBFCDF0;
    u16* word_BF2E08 = (u16*)0xBF2E08;

    *off_C06008 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C0600C = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    byte_BFCDF0[*off_C06008] = *(u8*)off_C0600C;
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
    } while (position != (u8)0xC9);
    return RUN_NEXT;
}

typedef void(*PFNSRSUB425D25)();
#define gamePushDisplayDamage ((PFNSRSUB425D25)0x425D29)

OpCodeControlSequence OpCodeC2(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    u32* off_C0600C = (u32*)0xC0600C;
    u8* byte_BFCDE0 = (u8*)0xBFCDE0;

    *off_C06008 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C0600C = srPushEffect100(gamePushDisplayDamage);
    auto& effectCtx = *getEffectContext100(*off_C0600C);
    effectCtx.wordArray[3] = *byte_BFCDE0;
    auto& displayEvent = *getFirstTargetDisplayEvent(*byte_BFCDE0);
    effectCtx.wordArray[4] = displayEvent.dealtDamage;
    effectCtx.wordArray[6] = displayEvent.actionFlags;
    effectCtx.wordArray[2] = displayEvent.impactEventQueueIdx;
    effectCtx.byteArray[23] = displayEvent.damageEventQueueIdx;
    effectCtx.wordArray[1] = *(u16*)off_C06008;
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
    u32* off_C06008 = (u32*)0xC06008;
    i32* off_C0600C = (i32*)0xC0600C;
    u32* off_C06010  = (u32*)0xC06010 ;
    u32* off_C06014  = (u32*)0xC06014 ;
    u32* off_C06018 = (u32*)0xC06018;
    u8* byte_9ADF08 = (u8*)0x9ADF08;
    u16* word_9A88BC = (u16*)0x9A88BC;

    if (!*byte_9ADF08) {
        sub_430D32(0x185u, -1, 64); //dunno what this does
        *byte_9ADF08 = 1;
    }
    *off_C0600C = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C06010  = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C06014  = srPushEffect10(sub_426941);
    auto& effectCtx = *getEffectContext10(*off_C06014 );
    effectCtx.wordArray[3] = srEvent->actorID;
    effectCtx.wordArray[4] = 0;
    effectCtx.wordArray[1] = *(u16*)off_C06010 ;
    switch (getBattleType()) {
    case 0:
    case 1:
    case 8:
        *off_C06018 = *off_C0600C;
        break;
    case 2:
        *off_C06018 = -*off_C0600C;
        break;
    case 3:
    case 5:
    case 6:
    case 7:
        if (srEvent->battleModelState->initialYRotation)
            *off_C06018 = -*off_C0600C;
        else
            *off_C06018 = *off_C0600C;
        break;
    case 4:
        if (*word_9A88BC){
            srEvent->battleModelState->restingYRotation = 0;
            *off_C06018 = *off_C0600C;
        }
        else {
            srEvent->battleModelState->restingYRotation = 2048;
            *off_C06018 = -*off_C0600C;
        }
        break;
    default:
        break;
    }
    effectCtx.wordArray[5] = *(u16*)off_C06018; 
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
    u32* off_C06008 = (u32*)0xC06008;
    u32* off_C0600C = (u32*)0xC0600C;
    u32* off_C06010  = (u32*)0xC06010 ;
    u32* off_C06014  = (u32*)0xC06014 ;
    *off_C06008 = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C0600C = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C06010  = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C06014  = srPushEffect10(sub_426941);
    auto& effectCtx = *getEffectContext10(*off_C06014 );
    effectCtx.wordArray[3] = srEvent->actorID;
    effectCtx.wordArray[4] = *(u16*)off_C06008;
    effectCtx.wordArray[5] = *(u16*)off_C0600C;
    effectCtx.wordArray[1] = *(u16*)off_C06010 ;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeCA(AnimScriptEvent* srEvent) {
    if (isEffectLoading()){
        u8 position = 0;
        srEvent->battleModelState->currentScriptPosition = 0;
        do {
            position = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
        } while (position != 0xC9);
    }
    return RUN_NEXT;
}

#define targetRelativeXYPlaneMoveEffect ((PFNSREFFECTCALLBACK)0x426A26)
OpCodeControlSequence OpCodeCC(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    auto& scriptCtx = *srEvent->scriptContext;
    scriptCtx.opCodeArgs[4] = 0;
    *off_C06008 = 0;
    scriptCtx.opCodeArgs[3] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    srComputeEnemyPartyCenter(getAnimatingActionTargetMask(), (R3PointWord*)&scriptCtx.field_16);
    scriptCtx.field_16 *= 2;
    scriptCtx.field_1A *= 2;
    srDispatchMoveEffects(
        srEvent->actorID,
        srEvent->scriptPtr,
        scriptCtx.opCodeArgs[4],
        targetRelativeXYPlaneMoveEffect,
        scriptCtx.opCodeArgs[3],
        (R3PointWord*)&(scriptCtx.field_16)
    );
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeCE(AnimScriptEvent* srEvent) {
    if (srEvent->actorID >= 4) {
        u8 position = 0;
        do {
            position = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
            srLogWrite("executing opcode 0xCE, postion: %x", position);
        } while (position != (u8)0xCD);
    }
    return RUN_NEXT;
}

#define targetRelative3DMoveEffect    ((PFNSREFFECTCALLBACK)0x42739D)
OpCodeControlSequence OpCodeCF(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    u32* off_C0600C = (u32*)0xC0600C;
    u32* off_C06010  = (u32*)0xC06010 ;
    u32* off_C06014  = (u32*)0xC06014 ;
    auto& scriptCtx = *srEvent->scriptContext;

    scriptCtx.opCodeArgs[4] = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C0600C = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C06008 = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C06010  = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C06014  = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[3] = *(u16*)off_C06014 ;
    srDispatchMoveEffects(
        srEvent->actorID,
        srEvent->scriptPtr,
        scriptCtx.opCodeArgs[4],
        targetRelative3DMoveEffect,
        scriptCtx.opCodeArgs[3],
        (R3PointWord*)&(getBattleModelState(getAnimatingActionTargetIdx())->restingPosition.x)
    );
    return RUN_NEXT;
}


#define targetRelativeMoveEffect    ((PFNSREFFECTCALLBACK)0x426F58)
OpCodeControlSequence OpCodeD0(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;

    auto& scriptCtx = *srEvent->scriptContext;
    scriptCtx.opCodeArgs[4] = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C06008 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    switch (*off_C06008) {
    case 0:
    case 1:
    case 2:
    case 3:
        scriptCtx.opCodeArgs[3] = 5;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        scriptCtx.opCodeArgs[3] = 8;
        break;
    default:
        break;
    }
    R3PointWord* moveBasePoint = nullptr;
    if (getTargetAllActive()) {
        srComputeEnemyPartyCenter(getAnimatingActionTargetMask(), (R3PointWord*)&(scriptCtx.field_16));
        moveBasePoint = (R3PointWord*)&(scriptCtx.field_16);
    }
    else {
        moveBasePoint = (R3PointWord*)(getBattleModelState(getAnimatingActionTargetIdx())->restingPosition.x);
    }
    srDispatchMoveEffects(
        srEvent->actorID,
        srEvent->scriptPtr,
        scriptCtx.opCodeArgs[4],
        targetRelativeMoveEffect,
        scriptCtx.opCodeArgs[3],
        moveBasePoint
    );
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeD1(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    auto& scriptCtx = *srEvent->scriptContext;

    scriptCtx.opCodeArgs[4] = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C06008 = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[3] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    R3PointWord* moveBasePoint = nullptr;
    if (getTargetAllActive()) {
        srComputeEnemyPartyCenter(getAnimatingActionTargetMask(), (R3PointWord*)&(scriptCtx.field_16));
        moveBasePoint = (R3PointWord*)&(scriptCtx.field_16);
    }
    else {
        auto pos = getBattleModelState(getAnimatingActionTargetIdx())->restingPosition;
        moveBasePoint = (R3PointWord*)&(getBattleModelState(getAnimatingActionTargetIdx())->restingPosition.x); 
    }
    srDispatchMoveEffects(
        srEvent->actorID,
        srEvent->scriptPtr,
        scriptCtx.opCodeArgs[4],
        targetRelativeXYPlaneMoveEffect,
        scriptCtx.opCodeArgs[3],
        moveBasePoint
    );
    return RUN_NEXT;
}


#define selfRelativeYPlaneMoveEffect   ((PFNSREFFECTCALLBACK)0x4267F1)
OpCodeControlSequence OpCodeD4(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    auto& scriptCtx = *srEvent->scriptContext;
    scriptCtx.opCodeArgs[4] = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[3] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);;
    *off_C06008 = scriptCtx.opCodeArgs[4] / scriptCtx.opCodeArgs[3];
    scriptCtx.field_14 = srPushEffect10(selfRelativeYPlaneMoveEffect);
    auto& effectCtx = *getEffectContext10(scriptCtx.field_14);
    effectCtx.wordArray[3] = srEvent->actorID;
    effectCtx.wordArray[4] = *(u16*)off_C06008;
    effectCtx.wordArray[1] = scriptCtx.opCodeArgs[3];
    return RUN_NEXT;
}


#define selfRelative3DMoveEffect   ((PFNSREFFECTCALLBACK)0x4270DE)
OpCodeControlSequence OpCodeD5(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    u32* off_C0600C = (u32*)0xC0600C;
    u32* off_C06010  = (u32*)0xC06010 ;
    u32* off_C06014  = (u32*)0xC06014 ;
    auto& scriptCtx = *srEvent->scriptContext;

    scriptCtx.opCodeArgs[4] = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C0600C = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C06008 = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C06010  = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C06014  = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[3] = *(u16*)off_C06014 ;
    srDispatchMoveEffects(
        srEvent->actorID,
        srEvent->scriptPtr,
        scriptCtx.opCodeArgs[4],
        selfRelative3DMoveEffect,
        scriptCtx.opCodeArgs[3],
        (R3PointWord*)(getBattleModelState(getAnimatingActionTargetIdx())->restingPosition.x)
    );
    return RUN_NEXT;
}


#define sub_4277B1   ((PFNSREFFECTCALLBACK)0x4277B1)
OpCodeControlSequence OpCodeD6(AnimScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    scriptCtx.opCodeArgs[0] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[1] = srPushEffect60(sub_4277B1);
    auto& effectCtx = *getEffectContext60(scriptCtx.opCodeArgs[1]);
    effectCtx.wordArray[2] = srEvent->actorID;
    effectCtx.wordArray[1] = scriptCtx.opCodeArgs[0];
    return RUN_NEXT;
}


#define sub_4276B6   ((PFNSREFFECTCALLBACK)0x4276B6)
OpCodeControlSequence OpCodeD7(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    u32* off_C0600C = (u32*)0xC0600C;
    auto& scriptCtx = *srEvent->scriptContext;

    *off_C06008 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C0600C = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[0] = srPushEffect60(sub_4276B6);
    auto& effectCtx = *getEffectContext60(scriptCtx.opCodeArgs[0]);
    effectCtx.wordArray[1] = *(u16*)off_C06008;
    effectCtx.wordArray[2] = *(u16*)off_C0600C;
    effectCtx.wordArray[3] = srEvent->actorID;
    return RUN_NEXT;
}

#define pushPlaySoundEffect   ((PFNSREFFECTCALLBACK)0x427737)
OpCodeControlSequence OpCodeD8(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    u32* off_C0600C = (u32*)0xC0600C;
    auto& scriptCtx = *srEvent->scriptContext;

    *off_C06008 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C0600C = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[0] = srPushEffect60(pushPlaySoundEffect);
    auto& effectCtx = *getEffectContext60(scriptCtx.opCodeArgs[0]);
    effectCtx.wordArray[1] = *(u16*)off_C06008;
    effectCtx.wordArray[2] = *(u16*)off_C0600C;
    return RUN_NEXT;
}

//This opcode is used to extend the script to run player registered opcodes
OpCodeControlSequence OpCodeDA(AnimScriptEvent* srEvent) {
    auto extendedCode = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    if (extendedCode <= 0x71) {
        srLogWrite("ERROR: Extended opcodes must have indexes greater than 0xFF");
        return RUN_NEXT;
    }
    auto opcode = gContext.animScriptOpcodes.getResource(extendedCode);
    return opcode(srEvent);
}

OpCodeControlSequence OpCodeDB(AnimScriptEvent* srEvent) {
    u32** dword_9AEAF0 = (u32**)0x9AEAF0; 
    u32* dword_D8E490 = (u32*)0xD8E490;
    u8* byte_BFCDE0 = (u8*)0xBFCDE0;
    auto& scriptCtx = *srEvent->scriptContext;

    scriptCtx.opCodeArgs[2] = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[0] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[1] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *dword_9AEAF0 = dword_D8E490;
    gameLoadMachineGunEffect();
    auto& effectCtx = *getEffectContext60(srPushEffect60(opCode96EffectHead));
    effectCtx.wordArray[1] = *byte_BFCDE0;
    effectCtx.wordArray[2] = srEvent->actorID;
    *(u32*)&(effectCtx.wordArray[13]) = 0xFFFFFFFF;
    effectCtx.wordArray[12] = scriptCtx.opCodeArgs[2];
    effectCtx.wordArray[3] = scriptCtx.opCodeArgs[0];
    effectCtx.wordArray[4] = scriptCtx.opCodeArgs[1];
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeDC(AnimScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    auto& ownerModelState = *srEvent->battleModelState;

    scriptCtx.opCodeArgs[0] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    auto readArg = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    switch (scriptCtx.opCodeArgs[0]) {
    case 0: {
        ownerModelState.initialZRotation = (ownerModelState.field_6 * readArg) / GAME_ANGLE_MAX;
        break;
    }
    case 1: {
        ownerModelState.field_1C = (ownerModelState.field_6 * readArg) / GAME_ANGLE_MAX;
        break;
    }
    default: {
        break;
    }
    }
    return RUN_NEXT;
}


OpCodeControlSequence OpCodeDD(AnimScriptEvent* srEvent) {
    u32** dword_9AEAF0 = (u32**)0x9AEAF0;
    u32* dword_D8E490 = (u32*)0xD8E490;
    u8* byte_BFCDE0 = (u8*)0xBFCDE0;
    auto& scriptCtx = *srEvent->scriptContext;

    scriptCtx.opCodeArgs[0] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[1] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *dword_9AEAF0 = dword_D8E490;
    gameLoadMachineGunEffect();
    auto& effectCtx = *getEffectContext60(srPushEffect60(opCode96EffectHead));
    effectCtx.wordArray[1] = *byte_BFCDE0;
    effectCtx.wordArray[2] = srEvent->actorID;
    *(void**)&(effectCtx.wordArray[13]) = (void*)&(srEvent->battleModelState->bData0x12[7]);
    effectCtx.wordArray[12] = srEvent->battleModelState->field_1C;
    effectCtx.wordArray[3] = scriptCtx.opCodeArgs[0];
    return RUN_NEXT;
}


OpCodeControlSequence OpCodeDE(AnimScriptEvent* srEvent) {
    u32** dword_9AEAF0 = (u32**)0x9AEAF0;
    u32* dword_D8E490 = (u32*)0xD8E490;
    u8* byte_BFCDE0 = (u8*)0xBFCDE0;
    auto& scriptCtx = *srEvent->scriptContext;

    scriptCtx.opCodeArgs[0] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[1] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *dword_9AEAF0 = dword_D8E490;
    gameLoadMachineGunEffect();
    auto& effectCtx = *getEffectContext60(srPushEffect60(opCode96EffectHead));
    effectCtx.wordArray[1] = *byte_BFCDE0;
    effectCtx.wordArray[2] = srEvent->actorID;
    *(void**)&(effectCtx.wordArray[13]) = (void*)&(srEvent->battleModelState->bData0x12[6]);
    effectCtx.wordArray[12] = srEvent->battleModelState->initialZRotation;
    effectCtx.wordArray[3] = scriptCtx.opCodeArgs[0];
    effectCtx.wordArray[4] = scriptCtx.opCodeArgs[1];
    return RUN_NEXT;
}

typedef i64(*PFNSRSUB662573)(i32, i32);
#define calculateYAngleComponent        ((PFNSRSUB662573)0x662573)
OpCodeControlSequence OpCodeDF(AnimScriptEvent* srEvent) {
    auto& scriptCtx = *srEvent->scriptContext;
    auto& ownerModelState = *srEvent->battleModelState;
    R3PointWord* basePoint = (R3PointWord*)&scriptCtx.field_16;
    srComputeEnemyPartyCenter(getAnimatingActionTargetMask(), basePoint);
    auto zDelta = basePoint->z - ownerModelState.restingPosition.z; 
    ownerModelState.restingYRotation = calculateYAngleComponent(basePoint->x - ownerModelState.restingPosition.x, zDelta) + 2048;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeE0(AnimScriptEvent* srEvent) {
    auto effectCtx = srCreateEffect(gamePushSpellAura, EFFECT60);
    effectCtx->wordArray[0] = 1;
    effectCtx->wordArray[1] = 36;
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

OpCodeControlSequence OpCodeE3(AnimScriptEvent* srEvent) {
    auto& actorModelState = *getBattleModelState(getActionActorIdx());
    R3PointWord* actorPositionArray = (R3PointWord*)(0xBFD0A0);
    actorPositionArray[getActionActorIdx()].x = actorModelState.restingPosition.x;
    actorPositionArray[getActionActorIdx()].z = actorModelState.restingPosition.z;
    return RUN_NEXT;
}


OpCodeControlSequence OpCodeE4(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    auto& ownerModelState = *getBattleModelState(srEvent->actorID);
    *off_C06008 = ownerModelState.restingYRotation + 2048;
    ownerModelState.restingPosition.x -= (516 * srCalculateXVectorComponent(*off_C06008)) / GAME_ANGLE_MAX;
    ownerModelState.restingPosition.z -= (516* srCalculateZVectorComponent(*off_C06008)) / GAME_ANGLE_MAX;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeE5(AnimScriptEvent* srEvent) {
    auto& ownerModelState = *srEvent->battleModelState;
    u8* byte_BFCDE0 = (u8*)0xBFCDE0;
    switch (getBattleType()) {
    case 1: {
        if (srEvent->actorID >= 4)
            ownerModelState.initialYRotation = 2048;
        break;
    }
    case 2: {
        if (srEvent->actorID < 4)
            ownerModelState.initialYRotation = 2048;
        break;
    }
    case 3:
    case 5:
    case 6: {
        if (srEvent->actorID >= 4) {
            if (srEvent->actorID == getActionActorIdx()) {
                if (getBattleModelState(*byte_BFCDE0)->initialYRotation)
                    ownerModelState.initialYRotation = 0;
                else
                    ownerModelState.initialYRotation = 2048;
            }
            else if (getBattleModelState(getActionActorIdx())->initialYRotation) {
                ownerModelState.initialYRotation = 0;
            }
            else {
                ownerModelState.initialYRotation = 2048;
            }
        }
        break;
    }
    case 4: {
        if (srEvent->actorID < 4 && (srEvent->actorID != *byte_BFCDE0 || srEvent->actorID != getActionActorIdx()) && !getTargetAllActive()) {
            if (srEvent->actorID == getActionActorIdx()) {
                if (getBattleModelState(*byte_BFCDE0)->initialYRotation)
                    ownerModelState.initialYRotation = 0;
                else
                    ownerModelState.initialYRotation = 2048;
            }
            else if (getBattleModelState(getActionActorIdx())->initialYRotation) {
                ownerModelState.initialYRotation = 0;
            }
            else {
                ownerModelState.initialYRotation = 2048;
            }
        }
        break;
    }
    default:
        break;
    }
    ownerModelState.restingYRotation = ownerModelState.initialYRotation;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeE6(AnimScriptEvent* srEvent) {
    auto effectCtx = srCreateEffect(gamePushSpellAura, EFFECT60);
    effectCtx->wordArray[0] = 0;
    effectCtx->wordArray[1] = 13;
    return RUN_NEXT;
}


OpCodeControlSequence OpCodeE7(AnimScriptEvent* srEvent) {
    u32* off_C06010  = (u32*)0xC06010 ;
    auto& modelState = *srEvent->battleModelState;
    auto& scriptCtx = *srEvent->scriptContext;

    *off_C06010  = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    u8 flagsLocal = modelState.field_25 | 4;
    if (*off_C06010 )
        flagsLocal = modelState.field_25 & 0xFB;
    modelState.field_25 = flagsLocal;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeE8(AnimScriptEvent* srEvent) {
    auto& modelState = *srEvent->battleModelState;
    auto& modelState74 = *getBattleModelState74(srEvent->actorID);
    //srLoadActionSpellEffects(srEvent->actorID, modelState.commandID, modelState74.actionIdx);
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeE9(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    auto& ownerModelState = *srEvent->battleModelState;
    auto& ownerModelState74 = *getBattleModelState74(srEvent->actorID);
    auto& scriptCtx = *srEvent->scriptContext;

    scriptCtx.opCodeArgs[4] = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[3] = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    *off_C06008 = ownerModelState74.field_28;
    srDispatchMoveEffects(
        srEvent->actorID,
        srEvent->scriptPtr,
        scriptCtx.opCodeArgs[4],
        targetRelativeXYPlaneMoveEffect,
        scriptCtx.opCodeArgs[3],
        (R3PointWord*)&(getBattleModelState(getAnimatingActionTargetIdx())->restingPosition.x)
    );
    return RUN_NEXT;
}


OpCodeControlSequence OpCodeEA(AnimScriptEvent* srEvent) {
    u8* byte_DC0E11 = (u8*)0xDC0E11;
    auto& scriptCtx = *srEvent->scriptContext;
    scriptCtx.field_14 = srPushEffect100((PFNSREFFECTCALLBACK)srDisplayActionStringEffect);
    auto& effectCtx = *getEffectContext100(scriptCtx.field_14);
    effectCtx.wordArray[1] = (*byte_DC0E11 >> 2) + 4;
    effectCtx.wordArray[2] = 0;
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
    srLogWrite("IN OPCODE EB: Dispatching actions for actor: %d, command %d, actionId: %d",
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
    srLogWrite("IN OPCODE EC: Dispatching actions for actor: %d, command %d, actionId: %d",
        srEvent->actorID,
        modelState.commandID,
        modelState74.actionIdx
    );
    srDispatchActionSpellEffects(srEvent->actorID, modelState.commandID, modelState74.actionIdx);
    *byte_9ADF04 = 0;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeED(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    auto& ownerModelState = *getBattleModelState(srEvent->actorID);
    *off_C06008 = ownerModelState.restingYRotation;
    ownerModelState.restingPosition.x -= (516 * srCalculateXVectorComponent(*off_C06008)) / GAME_ANGLE_MAX;
    ownerModelState.restingPosition.z -= (516 * srCalculateZVectorComponent(*off_C06008)) / GAME_ANGLE_MAX;
    return RUN_NEXT;
}

//0xFF is the same
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

OpCodeControlSequence OpCodeF1(AnimScriptEvent* srEvent) {
    auto& ownerModelState = *srEvent->battleModelState;
    auto& ownerModelState74 = *getBattleModelState74(srEvent->actorID);
    ownerModelState74.field_C &= 0xFFFBu;
    ownerModelState.field_26 = 1;
    ownerModelState74.field_C &= 0xF7;
    --ownerModelState.currentScriptPosition;
    return BREAK;
}

OpCodeControlSequence OpCodeF3(AnimScriptEvent* srEvent) {
    auto& ownerModelState = *srEvent->battleModelState;
    if (!ownerModelState.waitFrames) {
        return RUN_NEXT;
    }
    --ownerModelState.waitFrames;
    --ownerModelState.currentScriptPosition;
    srEvent->scriptContext->isScriptActive = 0;
    return BREAK;
}

OpCodeControlSequence OpCodeF4(AnimScriptEvent* srEvent) {
    auto& ownerModelState = *srEvent->battleModelState;
    ownerModelState.waitFrames = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    return RUN_NEXT;
}


OpCodeControlSequence OpCodeF5(AnimScriptEvent* srEvent) {
    u32* off_C06014  = (u32*)0xC06014 ;
    EnemyPosition* enemyPositionArray = (EnemyPosition*)0xBF2054;
    *off_C06014  = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    enemyPositionArray[(srEvent->actorID - 4)].modelArrayIdx = (*(u16*)off_C06014 ) + 3;
    gameInitEnemiesFromAB(srEvent->actorID);
    srEvent->battleModelState->modelEffectFlags |= 1u;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeF6(AnimScriptEvent* srEvent) {
    auto& ownerModelState74 = *getBattleModelState74(srEvent->actorID);
    u8* canDieDuringEffect = (u8*)0xBFC398; //controls whether the actor can die during the effect or not
    u16* killedActorsMask = (u16*)0xBFCB20; //mask of actors killed by the last consumed damage event
    if (*canDieDuringEffect) {
        if (ownerModelState74.field_C & 0x10) {
            gamePushDeathEffects(srEvent->actorID, 1); //dispatches 
        }
    }
    else if ((1 << srEvent->actorID) & *killedActorsMask){
        gamePushDeathEffects(srEvent->actorID, 1);
    }
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeF7(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    u8* byte_BF23BC = (u8*)0xBF23BC;
    *byte_BF23BC = 0;
    *off_C06008 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    pushDelayedDamageDisplayEffect(getAnimatingActionTargetMask(), *off_C06008, 1);
    return RUN_NEXT;
}

#define sub_425AAD      ((PFNSREFFECTCALLBACK)0x425AAD)
OpCodeControlSequence OpCodeF8(AnimScriptEvent* srEvent) {
    u32* off_C06008 = (u32*)0xC06008;
    u32* off_C06010  = (u32*)0xC06010 ;
    auto& scriptCtx = *srEvent->scriptContext;
    EnemyPosition* enemyPositionArray = (EnemyPosition*)0xBF2054;

    *off_C06008 = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    scriptCtx.opCodeArgs[0] = srPushEffect60(sub_425AAD);
    if (srEvent->actorID >= 4)
        *off_C06010  = enemyPositionArray[srEvent->actorID - 4].modelArrayIdx;
    else
        *off_C06010  = srEvent->actorID;

    u32* battleModelABHeader = (u32*)getActiveModelFromArray(*off_C06010 )->animScriptStruct;
    auto& effectCtx = *getEffectContext60(scriptCtx.opCodeArgs[0]);
    *(u32*)&effectCtx.wordArray[7] = battleModelABHeader[*off_C06008 + 9]; //Should have a dword view
    effectCtx.wordArray[2] = srEvent->actorID;
    effectCtx.byteArray[22] = 0;
    effectCtx.wordArray[1] = 0;
    effectCtx.wordArray[0] = ++(getBattleModelState74(srEvent->actorID)->innateStatusMask);
    return RUN_NEXT;
}

//resets actor orientation
OpCodeControlSequence OpCodeF9(AnimScriptEvent* srEvent) {
    srEvent->battleModelState->restingYRotation = srEvent->battleModelState->initialYRotation;
    return RUN_NEXT;
}

//return actor to default position
OpCodeControlSequence OpCodeFA(AnimScriptEvent* srEvent) {
    R3PointWord* actorPositionArray = (R3PointWord*)(0xBFD0A0);
    auto& ownerModelState = *srEvent->battleModelState;

    ownerModelState.restingPosition.x = actorPositionArray[srEvent->actorID].x;
    ownerModelState.restingPosition.y = actorPositionArray[srEvent->actorID].y;
    ownerModelState.restingPosition.z = actorPositionArray[srEvent->actorID].z;
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeFC(AnimScriptEvent* srEvent) {
    auto& ownerModelState = *srEvent->battleModelState;
    auto& targetModelState = *getBattleModelState(getAnimatingActionTargetIdx());
    auto& targetModelState74 = *getBattleModelState74(getAnimatingActionTargetIdx());
    u16* word_9A88AA = (u16*)0x9A88AA;
    if (getTargetAllActive()){
        switch (getBattleType()) {
        case 3:
        case 5:
        case 6:
        case 7: {
            for (u8 enemyIdx = 4; enemyIdx < 10; ++enemyIdx) {
                if (ownerModelState.initialYRotation)
                    getBattleModelState(enemyIdx)->initialYRotation = 0;
                else
                    getBattleModelState(enemyIdx)->initialYRotation = 2048;
            }
            break;
        }
        case 4: {
            if (getAnimatingActionTargetMask() & word_9A88AA[0])
                ownerModelState.restingYRotation = 0;
            if (getAnimatingActionTargetMask() & word_9A88AA[2])
                ownerModelState.restingYRotation = 2048;
            break;
        }
        default:
            return RUN_NEXT;
        }
    }
    else if ((ownerModelState.animationEffect != 21 || ownerModelState.commandID != 13) && getActionActorIdx() != getAnimatingActionTargetIdx()) {
        i32 zDelta = targetModelState.restingPosition.z - ownerModelState.restingPosition.z;
        i32 xDelta = targetModelState.restingPosition.x - ownerModelState.restingPosition.x;
        srLogWrite("computed orientation in FC: %x", calculateYAngleComponent(xDelta, zDelta));
        ownerModelState.restingYRotation = calculateYAngleComponent(xDelta, zDelta) + (GAME_ANGLE_MAX / 2);
        targetModelState74.field_2C = calculateYAngleComponent(xDelta, zDelta);
        srLogWrite("target orientation set to %x", targetModelState74.field_2C);
    }
    return RUN_NEXT;
}

//move actor to position specified in arguments
OpCodeControlSequence OpCodeFD(AnimScriptEvent* srEvent) {
    R3PointWord* actorPositionArray = (R3PointWord*)(0xBFD0A0);
    auto& ownerModelState = *srEvent->battleModelState;

    ownerModelState.restingPosition.x = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    ownerModelState.restingPosition.y = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    ownerModelState.restingPosition.z = readOpCodeArg16(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    return RUN_NEXT;
}

OpCodeControlSequence OpCodeFE(AnimScriptEvent* srEvent) {
    auto& ownerModelState = *srEvent->battleModelState;
    auto& ownerModelState74 = *getBattleModelState74(srEvent->actorID);
    auto& ownerRotationData = *getBattleModelRotationData(srEvent->actorID);
    auto& scriptCtx = *srEvent->scriptContext;
    if (!ownerModelState.waitFrames) {
        scriptCtx.currentOpCode = srEvent->scriptPtr[ownerModelState.currentScriptPosition];
        if (scriptCtx.currentOpCode == (u8)0xC0) {
            ownerModelState.currentScriptPosition = 0;
            ownerModelState.field_74 = 0;
            ownerModelState74.field_36 = 0;
            ownerRotationData.field_0 = 0;
            ownerModelState.waitFrames = 0;
            ownerModelState.runningAnimIdx = *scriptCtx.scriptPtr;
            ownerModelState.animScriptIndex = getActorIdleAnimScript(srEvent->actorID);
            scriptCtx.scriptPtr = srEvent->animationScriptTable[ownerModelState.animScriptIndex];
            ownerModelState.isScriptExecuting = 1;
            ownerModelState.currentScriptPosition = 0;
            ownerModelState.waitFrames = 0;
        }
    }
    return RUN_NEXT;
}

/*
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
 case 0xCBu:
     *newAxisPosition = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     v35 = readWordArg(actor_id, scriptContextPtr->scriptPtr);
     *off_C0600C = v35;
     *off_C06010  = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C06000 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C06004 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C06014  = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C06018 = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     *off_C0601C = *off_C06004 & 0xFF | 32 * (*off_C06000 & 0xFF) | ((*off_C06010  & 0xFF) << 10);
     if (*newAxisPosition == 255)
         sub_5BF388(actor_id, 0, *off_C0600C, *off_C0601C, *off_C06014 , *off_C06018);
     else
         sub_5BF388(actor_id, *newAxisPosition, *off_C0600C, *off_C0601C, *off_C06014 , *off_C06018);
     goto LABEL_20;

 case 0xDAu:
     gBigAnimBlock[actor_id].additionalEffect = scriptContextPtr->scriptPtr[gBigAnimBlock[actor_id].currentScriptPosition++];
     gBigAnimBlock[actor_id].commandID = 2;
     goto LABEL_20;
*/
