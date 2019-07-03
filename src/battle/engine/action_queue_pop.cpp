#include "action_queue_pop.h"
#include "../../impl.h"
#include "../battle.h"
#include "../../party/party_utils.h"
#include <memory>

/*Re-implemented to modularize adding commands to the game*/
void mainActionRoutine(BattleQueueEntry* poppedAction) {
    u32* dword_9AEA84 = (u32*)0x9AEA84;
    u32* dword_9AEA60 = (u32*)0x9AEA60;
    u32* dword_9AEA6C = (u32*)0x9AEA6C;

    if (*dword_9AEA84 > 2)
        return;

    auto issuingActorID = poppedAction->queueAction.attackerActorID;

    if (!(*dword_9AEA84)) {
        *dword_9AEA6C = 0;
        *dword_9AEA60 = 0;
        word_9AAD10 = 0;
        preActionHandles(poppedAction, issuingActorID);
        dword_9AEA84 = 1;
    }

    runSetupCallbacks(gDamageContextPtr->commandIndex);
    *dword_9AEA84 = 2;

    /*while (1) {
        callbackIdx = commandTypeExecutionArray[handlerToRunIdx++];
        if (callbackIdx == 31)
            break;
        if (callbackIdx == 20) {
            dword_9AEA78 = 0;
            commandTypeHandlers[20]();
            if (!dword_9AEA78)
            {
                *dword_9AEA60 = 1;
                break;
            }
            *dword_9AEA60 = 0;
            *dword_9AEA84 = 2;
        }
        else {
            commandTypeHandlers[callbackIdx]();
            dword_9AEA84 = 2;
        }
    }*/

    if (!dword_9AEA60) {
        postActionHandles(issuingActorID);
        *dword_9AEA84 = 0;
        *dword_9AEA6C = 1;
    }
}


//String display needs to be reworked, it is reliant on the kernel
typedef void(*PFNSRSUB5C7D59)(u8, u32, u8, u32*);
#define pushDisplayString   ((PFNSRSUB5C7D59)0x5C7D59)

typedef void(*PFNSRSUB5C8B80)();
#define incrementRandom     ((PFNSRSUB5C8B80)0x5C8B80)

void preActionHandles(BattleQueueEntry* poppedAction, u8 issuingActorID) {
    u16* gDisplayTextIdx = (u16*)0x9AAD16;
    u16* word_9AAD18 = (u16*)0x9AAD18;
    incrementRandom();
    memset((void*)gDamageContextPtr, 0, sizeof(DamageCalcStruct));

    runPreActionScripts(poppedAction, issuingActorID);
    handleToad(poppedAction, issuingActorID);
    initDamageContext(poppedAction, issuingActorID);
    setAllFlag(poppedAction, issuingActorID);
    prepareMimedAction(poppedAction, issuingActorID);
    setCommandData(poppedAction, issuingActorID);

    //Display Lucky 7's text
    if (gActorTimerBlock[issuingActorID].unkActorFlags & 0x80) {
        gActorTimerBlock[issuingActorID].unkActorFlags &= 0x7Fu;
        if (gActorTimerBlock[issuingActorID].currentHP == 7777) {
            *gDisplayTextIdx = 81;
            *word_9AAD18 = 47;
        }
    }

    FormationActorDataArray* formationData = getFormationActorData();
    if (issuingActorID >= 4)
        gDamageContextPtr->enemySceneIndex = formationData->formationDatas[issuingActorID - 4].enemyID;

    //Display caught by surprise string
    if (gActorTimerBlock[issuingActorID].unkActorFlags & 4) {
        u32 strArgs = issuingActorID;
        pushDisplayString(issuingActorID, 122, 1, &strArgs);
        gActorTimerBlock[issuingActorID].unkActorFlags &= 0xFBu;
    }
}

void postActionHandles(BattleQueueEntry* poppedAction, u8 issuingActorID) {
    sub_436CF2();
    setMimeData(poppedAction, issuingActorID);
    handleAddedCut(poppedAction, issuingActorID);

    if (gDamageContextPtr->commandIndexCopy == CMD_SENSE) {
        gDamageContextPtr->wasDamagedMask = 0;
        gDamageContextPtr->actionCounterable = 0;
    }
    handleCounters(poppedAction, issuingActorID);
    handleLuckySevens(poppedAction, issuingActorID)

    if (*dword_9AD1B4) {
        --(*dword_9AD1B4);
    }
    else if (poppedAction->entryPriority <= 2) {
        ++gActorTimerBlock[issuingActorID].field_2A;
    }
}
