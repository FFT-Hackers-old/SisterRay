#include "battle.h"
#include "../impl.h"
#include "../gzip.h"
#include "../party/party_utils.h"
#include <string>
#include <zlib.h>
#include <cstdio>
#include <memory>
#include "../EncodedString.h"

void srLoadBattleFormation(i32 formationIndex, i32(*modelAppearCallback)(void)) {
    char v4; 
    int enemyIndex;
    void *v16; 
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
        auto formationIDstr = std::to_string(formationIndex);
        auto formation = gContext.formations.get_element(assembleGDataKey(formationIndex));
        *formationSetupPtr = formation.formationSetup;
        *formationCameraPtr = formation.formationCamera;
        *formationActorDataPtr = formation.formationActorDataArray;

        auto& uniqueEnemyIds = formation.FormationEnemyIDs;
        for (auto enemyIndex = 0; enemyIndex < 3; enemyIndex++) {
            auto currentEnemyData = gContext.enemies.get_element(uniqueEnemyIds.uniqueIDs[enemyIndex]);
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
                enemyDataPtr[enemyIndex].enemyHP = enemyDataPtr[enemyIndex].enemyHP * 2;
                enemyDataPtr[enemyIndex].enemyStrength = (enemyDataPtr[enemyIndex].enemyStrength * 3) / 2;                
                enemyDataPtr[enemyIndex].enemyMagic = (enemyDataPtr[enemyIndex].enemyMagic * 3) / 2;
                enemyDataPtr[enemyIndex].enemyDefense = (enemyDataPtr[enemyIndex].enemyDefense * 3) / 2;
                enemyDataPtr[enemyIndex].enemyMDefense = (enemyDataPtr[enemyIndex].enemyMDefense * 3) / 2;
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
            characterScriptIndex = gUnkPartyDataArray[actorIndex].characterID;
            if (characterScriptIndex != -0xFF && linkedScriptArray[characterScriptIndex] != 0xFF) //this handles script links
                characterScriptIndex = linkedScriptArray[characterScriptIndex];
            auto& charAIData = gContext.characters.get_element(getCharacterName(characterScriptIndex)).characterAI;
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
            auto& enemyAIData = gContext.enemies.get_element(uniqueID).enemyAI;
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
            modelDataCpys[actorID].idleAnim = gAiActorVariables[actorID].idleAnimID;
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

/*Rewrite this function to expect an ABSOLUTE instead of relative id when executing enemy attacks*/
i32 enqueueScriptAction(i16 actorID, i16 commandIndex, i16 relAttackIndex) {
    u32* dword_C3F338 = (u32*)(0xC3F338);
    u16* word_9AB0AE = (u16*)(0x9AB0AE);

    /*Temporarily make the index type relative*/
    switch (commandIndex) { //convert to a relative/absolute ID offset in the player spells table
        case 3: {
            relAttackIndex -= 56;
            break;
        }
        case 13: {
            relAttackIndex -= 72;
            break;
        }
        default:{
        }
    }

    srLogWrite("enqueueing entry with for actor %i, command_id %i, attack_id %i from AI script", actorID, commandIndex, relAttackIndex);
    gAiActorVariables[actorID].lastTargets = *word_9AB0AE;
    BattleQueueEntry queueEntry = { *(u8*)dword_C3F338, 0, actorID, commandIndex, relAttackIndex, *word_9AB0AE };
    
    auto var = enqueueBattleAction((u8 *)&queueEntry);
    return var;
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

void dispatchAutoActions(i32 partyIndex, i32 actionType) {
    AutoActionType dispatchType;
    switch (actionType) {
        case 0: {
            dispatchType = FINAL_ATTACK;
            break;
        }
        case 1: {
            dispatchType = SNEAK_ATTACK;
            gActorTimerBlock[partyIndex].unkActorFlags |= 4;
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
    auto& autoActions = gContext.party.get_element(getPartyKey(partyIndex)).actorAutoActions;
    for (auto& action : autoActions) {
        if (action.dispatchType == AUTOACT_NO_ACTION)
            continue;
        if (action.counterCount == 0 || action.counterCount == 0xFF)
            continue;
        if (dispatchType == action.dispatchType) {
            //Add counter chance based code here
            auto finalAction = getActionToDispatch(action);
            auto targetMask = setTargetMask(partyIndex, action);
            auto priority = 1;
            if (action.dispatchType == FINAL_ATTACK)
                priority = 0;
            enqueueAction(partyIndex, priority, action.commandIndex, finalAction, targetMask);
            action.counterCount--;
        }
    }

}

u16 getActionToDispatch(const SrAutoAction& action) {
    if (action.actionIndex != 0xFFFF) //else return a random action based on the command type
        return action.actionIndex;
}

u16 setTargetMask(u8 partyIndex, const SrAutoAction& action) {
    u16 targetMask = 0;
    u8 targetingData = 3; //default targeting mask
    /*In the case of magic */
    if (action.commandIndex == CMD_MAGIC || action.commandIndex == CMD_SUMMON) {
        auto actionData = getSpellSlot(partyIndex, action.commandIndex, action.actionIndex);
        targetingData = actionData->targetData;
    }
    else {
        targetingData = getCommandSlot(partyIndex, action.commandIndex)->targetingData;
    }
    if (action.dispatchType != SNEAK_ATTACK) {
        if (targetingData & 2) {
            targetMask = gAiActorVariables[partyIndex].prevAttackerMask;
        }
        else {
            targetMask = 1 << partyIndex;
        }
    }
    return targetMask;
}
