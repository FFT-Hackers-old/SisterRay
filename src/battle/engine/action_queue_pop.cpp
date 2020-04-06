#include "action_queue_pop.h"
#include "../../impl.h"
#include "../battle.h"
#include "../../party/party_utils.h"
#include <memory>
#include "../../impl.h"
#include "../../gamedata/damage_callback_utils.h"

/*Re-implemented to modularize adding commands to the game*/
void srHandlePoppedAction(BattleQueueEntry* poppedAction) {
    u32* dword_9AEA84 = (u32*)0x9AEA84;
    u32* dword_9AEA60 = (u32*)0x9AEA60;
    u32* dword_9AEA6C = (u32*)0x9AEA6C;
    u16* word_9AAD10 = (u16*)0x9AAD10;

    if (*dword_9AEA84 > 2)
        return;

    auto issuingActorID = poppedAction->queueAction.attackerActorID;
    SrDamageContext srDamageContext = SrDamageContext();
    ActionContextEvent actionEvent = { gDamageContextPtr, &srDamageContext, poppedAction, issuingActorID, AI_BATTLE_CONTEXT };

    if (!(*dword_9AEA84)) {
        *dword_9AEA6C = 0;
        *dword_9AEA60 = 0;
        *word_9AAD10 = 0;
        preActionHandles(actionEvent);
        *dword_9AEA84 = 1;
    }

    runSetupCallbacks(actionEvent);
    *dword_9AEA84 = 2;

    if (!(*dword_9AEA60)) {
        postActionHandles(actionEvent);
        *dword_9AEA84 = 0;
        *dword_9AEA6C = 1;
    }
}


//String display needs to be reworked, it is reliant on the kernel
typedef void(*PFNSRSUB5C7D59)(u8, u32, u8, u32*);
#define pushDisplayString   ((PFNSRSUB5C7D59)0x5C7D59)

typedef void(*PFNSRSUB5C8B80)();
#define incrementRandom     ((PFNSRSUB5C8B80)0x5C8B80)

void preActionHandles(ActionContextEvent& actionEvent) {
    auto poppedAction = actionEvent.poppedAction;
    auto issuingActorID = actionEvent.issuingActorID;
    u16* gDisplayTextIdx = (u16*)0x9AAD16;
    u16* word_9AAD18 = (u16*)0x9AAD18;
    incrementRandom();
    memset((void*)gDamageContextPtr, 0, sizeof(DamageCalcStruct));
    srLogWrite("HANDLING BATTLE ACTION: actor: %d, command %d", poppedAction->queueAction.attackerActorID, poppedAction->queueAction.actionCommandIndex);
    gContext.eventBus.dispatch(ACTION_PRE_COMMAND, &actionEvent);

    //Display Lucky 7's text
    if (G_ACTOR_TIMER_ARRAY[issuingActorID].unkActorFlags & 0x80) {
        G_ACTOR_TIMER_ARRAY[issuingActorID].unkActorFlags &= 0x7Fu;
        if (G_ACTOR_TIMER_ARRAY[issuingActorID].currentHP == 7777) {
            *gDisplayTextIdx = 81;
            *word_9AAD18 = 47;
        }
    }
    //Display caught by surprise string
    /*if (gActorTimerBlock[issuingActorID].unkActorFlags & 4) {
        u32 strArgs = issuingActorID;
        pushDisplayString(issuingActorID, 122, 1, &strArgs);
        gActorTimerBlock[issuingActorID].unkActorFlags &= 0xFBu;
    }*/
}

typedef void(*PFNSUBSR436CF2)();
#define setDamageEventFlags   ((PFNSUBSR436CF2)0x436CF2)

void postActionHandles(ActionContextEvent& actionEvent) {
    auto poppedAction = actionEvent.poppedAction;
    auto issuingActorID = actionEvent.issuingActorID;
    u32* dword_9AD1B4 = (u32*)0x9AD1B4;
    setDamageEventFlags();
    gContext.eventBus.dispatch(ACTION_POST_COMMAND, &actionEvent);
    srLogWrite("Running post action callbacks");

    if (*dword_9AD1B4) {
        --(*dword_9AD1B4);
    }
    else if (poppedAction->entryPriority <= 2) {
        ++G_ACTOR_TIMER_ARRAY[issuingActorID].field_2A;
    }
}

void registerActionPopCallbacks() {
    const auto& modName = std::string("srFF7Base");
    gContext.eventBus.addListener(ACTION_PRE_COMMAND, (SrEventCallback)&runPreActionScripts, modName);
    gContext.eventBus.addListener(ACTION_PRE_COMMAND, (SrEventCallback)&handleToad, modName);
    gContext.eventBus.addListener(ACTION_PRE_COMMAND, (SrEventCallback)&initDamageContext, modName);
    gContext.eventBus.addListener(ACTION_PRE_COMMAND, (SrEventCallback)&setAllFlag, modName);
    gContext.eventBus.addListener(ACTION_PRE_COMMAND, (SrEventCallback)&prepareMimedAction, modName);
    gContext.eventBus.addListener(ACTION_PRE_COMMAND, (SrEventCallback)&setEnemyCommandData, modName);
    gContext.eventBus.addListener(ACTION_PRE_COMMAND, (SrEventCallback)&setPlayerCommandData, modName);

    gContext.eventBus.addListener(ACTION_POST_COMMAND, (SrEventCallback)&setMimeData, modName);
    gContext.eventBus.addListener(ACTION_POST_COMMAND, (SrEventCallback)&handleAddedCut, modName);
    gContext.eventBus.addListener(ACTION_POST_COMMAND, (SrEventCallback)&handleSense, modName);
    gContext.eventBus.addListener(ACTION_POST_COMMAND, (SrEventCallback)&handleCounters, modName);
    gContext.eventBus.addListener(ACTION_POST_COMMAND, (SrEventCallback)&handleLuckySevens, modName);
}
