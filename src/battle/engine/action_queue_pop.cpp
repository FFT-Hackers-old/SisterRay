#include "action_queue_pop.h"
#include "../../impl.h"
#include "../battle.h"
#include "../../party/party_utils.h"
#include <memory>

/*Re-implemented to modularize adding commands to the game*/
void srHandlePoppedAction(BattleQueueEntry* poppedAction) {
    u32* dword_9AEA84 = (u32*)0x9AEA84;
    u32* dword_9AEA60 = (u32*)0x9AEA60;
    u32* dword_9AEA6C = (u32*)0x9AEA6C;
    u16* word_9AAD10 = (u16*)0x9AAD10;

    if (*dword_9AEA84 > 2)
        return;

    auto issuingActorID = poppedAction->queueAction.attackerActorID;

    if (!(*dword_9AEA84)) {
        *dword_9AEA6C = 0;
        *dword_9AEA60 = 0;
        *word_9AAD10 = 0;
        preActionHandles(poppedAction, issuingActorID);
        *dword_9AEA84 = 1;
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
        postActionHandles(poppedAction, issuingActorID);
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

    ActionContextEvent actionEvent = { gDamageContextPtr, poppedAction, issuingActorID };
    gContext.eventBus.dispatch(ACTION_PRE_COMMAND, &actionEvent);

    //Display Lucky 7's text
    if (gActorTimerBlock[issuingActorID].unkActorFlags & 0x80) {
        gActorTimerBlock[issuingActorID].unkActorFlags &= 0x7Fu;
        if (gActorTimerBlock[issuingActorID].currentHP == 7777) {
            *gDisplayTextIdx = 81;
            *word_9AAD18 = 47;
        }
    }
    //Display caught by surprise string
    if (gActorTimerBlock[issuingActorID].unkActorFlags & 4) {
        u32 strArgs = issuingActorID;
        pushDisplayString(issuingActorID, 122, 1, &strArgs);
        gActorTimerBlock[issuingActorID].unkActorFlags &= 0xFBu;
    }
}

typedef void(*PFNSUBSR436CF2)();
#define setDamageEventFlags   ((PFNSUBSR436CF2)0x436CF2)

void postActionHandles(BattleQueueEntry* poppedAction, u8 issuingActorID) {
    u32* dword_9AD1B4 = (u32*)0x9AD1B4;
    setDamageEventFlags();

    ActionContextEvent actionEvent = { gDamageContextPtr, poppedAction, issuingActorID };
    gContext.eventBus.dispatch(ACTION_POST_COMMAND, &actionEvent);

    if (*dword_9AD1B4) {
        --(*dword_9AD1B4);
    }
    else if (poppedAction->entryPriority <= 2) {
        ++gActorTimerBlock[issuingActorID].field_2A;
    }
}

void registerActionPopCallbacks() {
    const auto& modName = std::string("srFF7Base");
    gContext.eventBus.addListener(ACTION_PRE_COMMAND, (SrEventCallback)&runPreActionScripts, modName);
    gContext.eventBus.addListener(ACTION_PRE_COMMAND, (SrEventCallback)&handleToad, modName);
    gContext.eventBus.addListener(ACTION_PRE_COMMAND, (SrEventCallback)&initDamageContext, modName);
    gContext.eventBus.addListener(ACTION_PRE_COMMAND, (SrEventCallback)&setAllFlag, modName);
    gContext.eventBus.addListener(ACTION_PRE_COMMAND, (SrEventCallback)&prepareMimedAction, modName);
    gContext.eventBus.addListener(ACTION_PRE_COMMAND, (SrEventCallback)&setCommandData, modName);

    gContext.eventBus.addListener(ACTION_POST_COMMAND, (SrEventCallback)&setMimeData, modName);
    gContext.eventBus.addListener(ACTION_POST_COMMAND, (SrEventCallback)&handleAddedCut, modName);
    gContext.eventBus.addListener(ACTION_POST_COMMAND, (SrEventCallback)&handleSense, modName);
    gContext.eventBus.addListener(ACTION_POST_COMMAND, (SrEventCallback)&handleCounters, modName);
    gContext.eventBus.addListener(ACTION_POST_COMMAND, (SrEventCallback)&handleLuckySevens, modName);
}
