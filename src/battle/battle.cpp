#include "battle.h"
#include "../impl.h"
#include "../gzip.h"
#include "../party/party_actions.h"
#include <string>
#include <zlib.h>
#include <cstdio>
#include <memory>
#include "../EncodedString.h"
#include <algorithm>
#include "engine/damage_events.h"
#include "engine/action_queue.h"
#include "engine/battle_engine_interface.h"
#include "../inventories/inventory_functions.h"
#include "string_display_event.h"
#include "../gamedata/summons.h"


#define initEventList         ((PFNSR_VOIDSUB)0x4350CB)
#define modifyPartyStateFlags ((PFNSR_VOIDSUB)0x5D0CA0)
#define modifyEnemyStateFlags ((PFNSR_VOIDSUB)0x432C60)
#define runPartyInitScripts   ((PFNSR_VOIDSUB)0x5CFE10)
#define runEnemyInitAIScripts ((PFNSR_VOIDSUB)0x5D0BDF)
#define runFormationAIScripts ((PFNSR_VOIDSUB)0x5C8931)
#define initActorATBs         ((PFNSR_VOIDSUB)0x4383F5)
#define sub_433020            ((PFNSR_VOIDSUB)0x433020)

typedef void(*PFNSR_UNKSUB433020)(int);
#define sub_5C8C42            ((PFNSR_UNKSUB433020)0x5C8C42)
void setupBattle(u32 formationID) {
    u32* dword_9ACB84 = (u32*)0x9ACB84;
    u32* dword_9AB070 = (u32*)0x9AB070;
    u32* dword_9AB9F4 = (u32*)0x9AB9F4;
    u16* word_9A8B08 = (u16*)0x9A8B08;
    u32* dword_9A89D0 = (u32*)0x9A89D0;
    u8* G_BATTLE_TYPE = (u8*)0x9A8762;

    auto randInt = rand();
    sub_5C8C42(randInt);
    for (u8 characterID = 0; characterID < 9; ++characterID) {
        //recalculateBaseStats(characterIndex);
        bool characterActive = false;
        for (auto idx = 0; idx < 3; idx++) {
            if (G_SAVE_MAP->activeParty[idx] == characterID) {
                gContext.party.recalculatePartyMember(idx);
                characterActive = true;
            }
        }
        if (characterActive)
            continue;
        gContext.party.recalculateCharacter(characterID);
        //srRecalculateDerivedStats(characterIndex);
    }
    gContext.party.initializePlayableSummons();
    updateMiscPartyStats();
    //auto battleString = getBattleString(126);
    *dword_9ACB84 = 0x10;
    *dword_9AB070 = 0x21;
    initAnimEventQueue();
    initializeActionQueue();
    /*for (partyIndexc = 0; partyIndexc < 2; ++partyIndexc)
        gMimeActions[partyIndexc].attackerID = -1;*/
    initEventList();
    *dword_9AB9F4 = -1;
    setDisplayStringIdx(0xFFFF);
    auto aiContext = AI_BATTLE_CONTEXT;
    for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
        aiContext->actorAIStates[actorIdx].index = -1;
        aiContext->actorAIStates[actorIdx].sizeScale = 16;
    }
    sub_433020();

    u8* byte_DC0E10 = (u8*)0xDC0E10;
    setBattleSpeed(*byte_DC0E10);
    initializePlayerActors();
    setupBattleInventory();
    srLoadBattleFormation(formationID, 0);
    *dword_9A89D0 = 0;
    initializeEnemyActors();

    u16* word_9AAD02 = (u16*)0x9AAD02;
    u16* word_DC0E12 = (u16*)0xDC0E12;
    //setBattleSpeed(*byte_DC0E10);
    *word_9AAD02 = (*word_DC0E12 & 0xC0) >> 6;
    for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
        if (aiContext->actorAIStates[actorIdx].index != -1)
            aiContext->activeActorMask |= 1 << actorIdx;
    }
    aiContext->battleFormationIdx = formationID;
    aiContext->battleType = *G_BATTLE_TYPE;
    modifyPartyStateFlags();
    modifyEnemyStateFlags();
    runPartyInitScripts();
    runEnemyInitAIScripts();
    runFormationAIScripts();
    modifyEnemyStateFlags();
    initActorATBs();
    //initTimerHPCopies();
    *word_9A8B08 |= 1u;
    for (u8 partyIdx = 0; partyIdx < 3; ++partyIdx)
        dispatchAutoActions(partyIdx, 1);           // sneakAttackActions

    if (aiContext->unkBattleFlags & 8) {
        setBattleSpeed(128);
        word_9AAD02 = 0;
        /*for (u8 partyIdx = 0; partyIdx < 3; ++partyIdx)
            sub_5D0097(partyIdx)*/
    }
    else {
        //enqueue7777Hits();
    }

    u32* dword_C06644 = (u32*)0xC06644;
    u32* dword_C06648 = (u32*)0xC06648;
    u32* dword_C06640 = (u32*)0xC06640;
    u32* dword_99CE08 = (u32*)0x99CE08;
    u32* dword_9ACB64 = (u32*)0x9ACB64;
    if (aiContext->unkBattleFlags & 4) {
        if (!(*dword_99CE08 & 4)) {
            *dword_99CE08 |= 4u;
            *dword_C06640 = -1;
        }

        *dword_C06644 = aiContext->unkLimitDivisor;
        auto limitDivisor = aiContext->unkLimitDivisor;
        auto v7 = 1 << (6 * aiContext->unkLimitDivisor);
        for (u8 enemyIdx = 0; enemyIdx < 6; ++enemyIdx) {
            if (v7 & *dword_C06640) {
                *dword_C06640 &= ~v7;
            }
            else if (aiContext->actorAIStates[enemyIdx + 4].index != -1) {
                aiContext->actorAIStates[enemyIdx + 4].currentHP = dword_C06648[6 * limitDivisor + enemyIdx];
                if (!aiContext->actorAIStates[enemyIdx + 4].currentHP) {
                    aiContext->actorAIStates[enemyIdx + 4].statusMask |= 1;
                    aiContext->actorAIStates[enemyIdx + 4].initalStatusMasks | 1;
                    aiContext->actorAIStates[enemyIdx + 4].stateFlags &= 0xE7;
                }
            }
            v7 *= 2;
        }
        modifyEnemyStateFlags();
    }
    *dword_9ACB64 = 1;
}

void srLoadBattleFormation(i32 formationIndex, i32(*modelAppearCallback)(void)) {
    int enemyIndex;
    FormationEnemies* formationEnemiesPtr = getInBattleFormationEnemyModels();
    FormationSetup* formationSetupPtr = getInBattleFormationSetup();
    FormationCamera* formationCameraPtr = getInBattleFormationCamera();
    FormationActorDataArray* formationActorDataPtr = getInBattleFormationActorDataArray();
    EnemyData* enemyDataPtr = (EnemyData*)(0x9A8E9C);
    FormationEnemyIDs* sceneAIDataPtr = (FormationEnemyIDs*)(0x9A9CFC);
    u32* formationAIDataPtr = (u32*)(0x9A9AFC);
    u32* dword_C069BC = (u32*)(0xC069BC);
    u32* dword_9A89D0 = (u32*)(0x9A89D0);
    u16* battleFlags = (u16*)(0x9A88A6);
    u16* gFormationType = (u16*)(0x9AAD20);
    u16* gEscapeFlag = (u16*)(0x9AAD0A);
    u16* word_9AAD0E = (u16*)(0x9AAD0E);
    u32* dword_9ACB68 = (u32*)(0x9ACB68);
    u8* battleTypeArray = (u8*)(0x7B76F0);
    // this code coordinates, along with a2, additional setup for "next battle"
    if (*dword_C069BC) {
        if (*dword_C069BC != 1) {
            if (*dword_C069BC != 2) {
                *dword_C069BC = 0;
                *dword_9A89D0 = 1;
                return;
            }
            goto LABEL_11;
        }
    }
    else {
        *dword_C069BC = 1;
    }

    if (!modelAppearCallback) { 
        *dword_C069BC = 2;

    LABEL_11:
        auto formation = gContext.formations.getResource(formationIndex);
        *formationSetupPtr = formation.formationSetup;
        *formationCameraPtr = formation.formationCamera;
        *formationActorDataPtr = formation.formationActorDataArray;

        auto& uniqueEnemyIds = formation.FormationEnemyIDs;
        for (auto enemyIndex = 0; enemyIndex < 3; enemyIndex++) {
            auto currentEnemyData = gContext.enemies.getResource(uniqueEnemyIds.uniqueIDs[enemyIndex]);
            formationEnemiesPtr->EnemyIds[enemyIndex] = currentEnemyData.modelID;
            enemyDataPtr[enemyIndex] = currentEnemyData.enemyData;
            /*While the original game copies AI data here, we parse AI scripts and store them with our registries
              Ergo, we elect to store a battle accessible SR struct which contains unique enemy IDs here*/
            *sceneAIDataPtr = formation.FormationEnemyIDs;
            *formationAIDataPtr = formationIndex;
        }

        //Set pre-emptive in some circumstances
        if ((*battleFlags & 4) && (formationSetupPtr->battleFlags & 0x10) && !(formationSetupPtr->battleType))
            formationSetupPtr->battleType = 1;
        *gFormationType = battleTypeArray[formationSetupPtr->battleType];

        if (*battleFlags & 0x40) {                   // Boost stats in battle arena
            formationSetupPtr->battleFlags |= 4u;
            formationSetupPtr->battleLocation = 37;
            formationSetupPtr->initCameraID = (getRandomNumber() & 3) + 96;
            formationSetupPtr->escapeCounter = 1;
            for (enemyIndex = 0; enemyIndex < 3; ++enemyIndex) { // boost all stats for battle arena, need to cap at 255
                enemyDataPtr[enemyIndex].maxHP = enemyDataPtr[enemyIndex].maxHP * 2;
                enemyDataPtr[enemyIndex].strength = (enemyDataPtr[enemyIndex].strength * 3) / 2;                
                enemyDataPtr[enemyIndex].magic = (enemyDataPtr[enemyIndex].magic * 3) / 2;
                enemyDataPtr[enemyIndex].defense = (enemyDataPtr[enemyIndex].defense * 3) / 2;
                enemyDataPtr[enemyIndex].mDefense = (enemyDataPtr[enemyIndex].mDefense * 3) / 2;
            }
        }
        else if (*battleFlags & 8) {
            formationSetupPtr->battleFlags &= 0xFFFBu;
        }

        if (!(formationSetupPtr->battleFlags & 4))
            *battleFlags |= 8u;

        *gEscapeFlag = formationSetupPtr->escapeCounter;
        if (*gFormationType == 1 || *gFormationType == 3)
            *gEscapeFlag = 1;
        *word_9AAD0E = *gEscapeFlag;
        *dword_C069BC = 0;
        *dword_9A89D0 = 1;
        return;
    }

    modelAppearCallback();
    if (*dword_9ACB68) {
        *dword_9ACB68 = 0;
        *dword_C069BC = 2;
    }
}

void srExecuteAIScript(i32 actorIndex, i32 scriptType, i32 a3) {
    scriptAnimDataCpy modelDataCpys[10];
    const u8* scriptPtr = nullptr;
    u8 characterScriptIndex = 0xFF; 
    FormationActorDataArray* formationActorDataPtr = getInBattleFormationActorDataArray();
    FormationEnemyIDs* sceneAIDataPtr = (FormationEnemyIDs*)(0x9A9CFC);
    u8* linkedScriptArray = (u8*)(0x8FEE38);
    u16* unknownPtr = (u16*)(0x9AAD14);

    *unknownPtr |= 1 << scriptType;

    switch (actorIndex) {
        case 0:
        case 1:
        case 2: {
            characterScriptIndex = G_BATTLE_PARTY10_ARRAY[actorIndex].characterID;
            if (characterScriptIndex != -0xFF && linkedScriptArray[characterScriptIndex] != 0xFF) //this handles script links
                characterScriptIndex = linkedScriptArray[characterScriptIndex];
            auto& charAIData = gContext.characters.getElement(getCharacterName(characterScriptIndex)).characterAI;
            scriptPtr = getScriptPtr(charAIData, scriptType);
            break;
        }
        case 3: {
            break;
        }
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9: {
            auto formationEnemyID = formationActorDataPtr->formationDatas[actorIndex - 4].enemyID; //fetch the formation relative ID, it's modified from the absolute ID
            auto uniqueID = sceneAIDataPtr->uniqueIDs[formationEnemyID];
            auto& enemyAIData = gContext.enemies.getResource(uniqueID).enemyAI;
            scriptPtr = getScriptPtr(enemyAIData, scriptType);
            break;
        }
        default: {
        }
    }

    if (scriptPtr) {
        //Here we copy the animation index, size scale, and damaged animation of each actor so we can react if they are changed by scripts
        for (auto actorID = 0; actorID < 10; ++actorID) {
            modelDataCpys[actorID].sizeScale = gAiActorVariables[actorID].sizeScale;
            modelDataCpys[actorID].idleAnim = gAiActorVariables[actorID].idleAnimScript;
            modelDataCpys[actorID].damagedAnim = gAiActorVariables[actorID].damageAnimID;
        }

        sub_5D9550(actorIndex, a3, 0); //Not sure what this function does yet
        runAIScript(actorIndex, (i32)scriptPtr, characterScriptIndex);

        /*If the size scale or animation has changed, we call some functions*/
        for (auto actorID = 0; actorID < 10; ++actorID) {
            if (modelDataCpys[actorID].sizeScale != gAiActorVariables[actorID].sizeScale)
                createAnimationEvent(actorID, 4, gAiActorVariables[actorID].sizeScale, 16);

            if (modelDataCpys[actorID].damagedAnim != gAiActorVariables[actorID].damageAnimID)
                sub_437185(actorID, modelDataCpys[actorID].damagedAnim, gAiActorVariables[actorID].damageAnimID, 0);
        }
    }
}

i32 srExecuteFormationScripts() {
    int result; 
    const u8* scriptPtr;
    u16 scriptType; 
    u32* formationAIDataPtr = (u32*)(0x9A9AFC);
    u16* word_9AAD14 = (u16*)(0x9AAD14);

    sub_5D9550(-1, 0, 0);
    auto formationIndex = *formationAIDataPtr;
    for (scriptType = 0; scriptType < 8; ++scriptType) {
        if ((1 << scriptType) & (i16)(*word_9AAD14)) {
            *word_9AAD14 &= ~(i16)(1 << scriptType);
            auto& formationAI = getFormation(formationIndex).formationAI;
            scriptPtr = getScriptPtr(formationAI, scriptType);
            if (scriptPtr)
                result = runAIScript(3, (i32)scriptPtr, -1);
        }
        result = scriptType + 1;
    }
    return result;
}

/*void* transformEnemyCommand() {
    gDamageContextPtr->absAttackIndex = gDamageContextPtr->relAttackIndex;
    if (gDamageContextPtr->commandIndex == CMD_ENEMY_ACTION) {
        u16 baseOffsets[5] = { 0, 56, 72, 96, 256 };
        u16 commandIndexes[4] = { 2, 3, 13, 20 };
        for (auto baseIndexOffset = 0; baseIndexOffset < 4; ++baseIndexOffset) {
            if (gDamageContextPtr->absAttackIndex < baseOffsets[1 + baseIndexOffset]) {
                gDamageContextPtr->commandIndexCopy = commandIndexes[baseIndexOffset];
                gDamageContextPtr->sceneAbilityIndex = gDamageContextPtr->absAttackIndex - baseOffsets[baseIndexOffset];
            }
        }
    }
    return (void*)gDamageContextPtr;
}*/

void dispatchAutoActions(u8 characterIdx, i32 actionType) {
    AutoActionType dispatchType;
    switch (actionType) {
        case 0: {
            dispatchType = FINAL_ATTACK;
            break;
        }
        case 1: {
            dispatchType = SNEAK_ATTACK;
            G_ACTOR_TIMER_ARRAY[characterIdx].unkActorFlags |= 4;
            break;
        }
        case 2: {
            dispatchType = COUNTER_ACTION;
            break;
        }
        default: {
            dispatchType = COUNTER_ACTION;
        }
    }
    auto& autoActions = getSrPartyMember(characterIdx).srPartyMember->actorAutoActions;
    for (auto& action : autoActions) {
        if (action.dispatchType == AUTOACT_NO_ACTION)
            continue;
        if (action.counterCount == 0 || action.counterCount == 0xFF)
            continue;
        if (dispatchType == action.dispatchType) {
            //Add counter chance based code here
            auto finalAction = getActionToDispatch(action);
            auto targetMask = setTargetMask(characterIdx, action);
            u8 priority = 1;
            if (action.dispatchType == FINAL_ATTACK)
                priority = 0;
            enqueueAction(characterIdx, priority, action.commandIndex, finalAction, targetMask);
            action.counterCount--;
        }
    }

}

u16 getActionToDispatch(const SrAutoAction& action) {
    if (action.actionIndex != 0xFFFF) //else return a random action based on the command type
        return action.actionIndex;

    return 0;
}

u16 setTargetMask(u8 characterIdx, const SrAutoAction& action) {
    u16 targetMask = 0;
    u8 targetingData = 3; //default targeting mask
    /*In the case of magic */
    if (action.commandIndex == CMD_MAGIC || action.commandIndex == CMD_SUMMON) {
        auto actionData = getSpellSlot(characterIdx, action.commandIndex, action.actionIndex);
        targetingData = actionData->targetData;
    }
    else {
        targetingData = getCommandSlot(characterIdx, action.commandIndex)->targetingData;
    }
    if (action.dispatchType != SNEAK_ATTACK) {
        if (targetingData & 2) {
            targetMask = gAiActorVariables[characterIdx].prevAttackerMask;
        }
        else {
            targetMask = 1 << characterIdx;
        }
    }
    return targetMask;
}
