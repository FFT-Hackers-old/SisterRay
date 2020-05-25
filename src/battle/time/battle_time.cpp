#include "battle_time.h"
#include "../battle_actors.h"
#include "../../events/event_bus_interface.h"
#include "../../impl.h"
#include "../../gamedata/stat_names.h"
#include "../../gamedata/status_names.h"
#include "../../gamedata/damage_callback_utils.h"
#include "../battle_engine_interface.h"


static u32* G_VTIMER_INCREMENTS = (u32*)0x9AE928;
static u32* G_CTIMER_INCREMENTS = (u32*)0x9AE978;
static u32* G_TIMER_INCREMENTS = (u32*)0x9AE8D0;
static u32* G_V_TIMERS = (u32*)0x9AE900;
static u32* G_C_TIMERS = (u32*)0x9AE8D8;
static u32* G_G_TIMERS = (u32*)0x9AE8A8;


typedef void(*PFNSRSUB_5C89E2)(u32, u32, u32);
static PFNSRSUB_5C89E2 actorDeathHandler = (PFNSRSUB_5C89E2)0x5C89E2;
static PFNSR_VOIDSUB runFormationAIScripts = (PFNSR_VOIDSUB)0x5C8931;
void srDispathTimeEvents() {
    u32* timerSetChoice = (u32*)0x9AE8A0;
    *timerSetChoice ^= 1u;
    auto& aiContext = *AI_BATTLE_CONTEXT;
    u32* actorVTimerIncrements = &(G_VTIMER_INCREMENTS[10 * (*timerSetChoice)]);
    u32* actorCTimerIncrements = &(G_CTIMER_INCREMENTS[10 * (*timerSetChoice)]);
    u32 globalTimerDecrement = G_TIMER_INCREMENTS[*timerSetChoice] >> 13;
    G_TIMER_INCREMENTS[*timerSetChoice] &= 0x1FFFu;
    for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
        auto& battleActor = getActiveBattleActor(actorIdx);
        auto timerField6 = battleActor.actorTimers->field_6;
        u16 actorVTimer = actorVTimerIncrements[actorIdx];
        u16 actorCTimer = actorCTimerIncrements[actorIdx];

        G_V_TIMERS[actorIdx] += actorVTimer;
        bool didVTimerTick = G_V_TIMERS[actorIdx] >> 13;
        UpdateActorTimersEvent updateTimersEvent{ &battleActor };
        if (didVTimerTick) {
            srLogWrite("DISPATCHING V-Timer Tick Events for actor %i", actorIdx);
            dispatchEvent(ON_VTIMER_TICK, &updateTimersEvent);
        }
        G_V_TIMERS[actorIdx] &= 0x1FFFu;

        G_C_TIMERS[actorIdx] += actorCTimer;
        bool didCTimerTick = G_C_TIMERS[actorIdx] >> 12;
        if (didCTimerTick) {
            srLogWrite("DISPATCHING C-Timer Tick Events for actor %i", actorIdx);
            dispatchEvent(ON_CTIMER_TICK, &updateTimersEvent);
        }
        G_C_TIMERS[actorIdx] &= 0xFFFu;

        G_G_TIMERS[actorIdx] += timerField6 * actorVTimer;
        auto v8 = G_G_TIMERS[actorIdx] >> 15;
        G_G_TIMERS[actorIdx] &= 0x7FFFu;
        actorVTimerIncrements[actorIdx] = 0;
        actorCTimerIncrements[actorIdx] = 0;

        if (!(srActorHasStatus(battleActor, StatusNames::DEATH))) {
            auto currentHP = v8 + battleActor.actorBattleVars->currentHP;
            if (currentHP >= 0) {
                if (currentHP > battleActor.actorBattleVars->maxHP)
                    currentHP = battleActor.actorBattleVars->maxHP;
            }
            else {
                srInflictStatus(battleActor, StatusNames::DEATH);
                actorDeathHandler(actorIdx, actorIdx, 1);
                runFormationAIScripts();
                currentHP = 0;
            }
            battleActor.actorBattleVars->currentHP = currentHP;
        }
        for (u8 gameEventType = 0; gameEventType < 16; ++gameEventType) {
            u32 eventTimerDecrement;
            if (battleActor.actorTimers->eventTimers[gameEventType]) {
                if (gameEventType) {
                    if (gameEventType > 1 && gameEventType <= 3)
                        eventTimerDecrement = didCTimerTick;
                    else
                        eventTimerDecrement = didVTimerTick;
                }
                else {
                    eventTimerDecrement = globalTimerDecrement;
                }
                u16 eventTimerValue = battleActor.actorTimers->eventTimers[gameEventType] - eventTimerDecrement;
                if (eventTimerValue < 0)
                    eventTimerValue = 0;
                if (!eventTimerValue)
                    gameCreateEvent(2, actorIdx, gameEventType, gameEventType);
                battleActor.actorTimers->eventTimers[gameEventType] = eventTimerValue;
            }
        }
    }
}

static PFNSR_VOIDSUB gameSyncTimers = (PFNSR_VOIDSUB)0x43553F;
static PFNSR_INTSUB consumeEvents = (PFNSR_INTSUB)0x4351BD;
static PFNSR_VOIDSUB fillActorResources = (PFNSR_VOIDSUB)0x4355F0;
typedef u32(*PFNSR_4324B6)();
static PFNSR_4324B6 isWaitModeActive = (PFNSR_4324B6)0x4324B6;
void srHandleFillATB() {
    u16* word_9A8B08 = (u16*)0x9A8B08;
    u16* word_9A88A4 = (u16*)0x9A88A4;
    u16* G_NOT_DEFEATED_ACTORS = (u16*)0x9A8896;
    u16* G_DEAD_ACTORS_MASK = (u16*)0x9AAD22;
    consumeEvents(0);
    *word_9A8B08 |= 1u;
    if (!isWaitModeActive()) {
        listenForEscape();
        gameSyncTimers();
        for (u8 actorIdx = 0; actorIdx < 10; ++actorIdx) {
            auto& actorTimers = *gContext.battleActors.getActiveBattleActor(actorIdx).actorTimers;
            srSetActorTimers(getActiveBattleActor(actorIdx));
            if (!((*word_9A88A4 | 7) & (*G_NOT_DEFEATED_ACTORS | 0x3F0) & (1 << actorIdx))) {
                continue;
            }

            UpdateActorTimersEvent updateTimersEvent{ &gContext.battleActors.getActiveBattleActor(actorIdx) };
            dispatchEvent(ON_ACTOR_ATB_FILL, &updateTimersEvent);
            auto atbValue = actorTimers.charATBValue;
            auto turnTimerIncrement = actorTimers.turnTimerIncrement;
            if (atbValue == 0xFFFF) {
                continue;
            }

            if (actorTimers.limitFastATBMask & 1)
                turnTimerIncrement *= 4;
            if ((1 << actorIdx) & *G_DEAD_ACTORS_MASK)
                turnTimerIncrement = 0;

            u16 newATBValue = ((turnTimerIncrement + atbValue) <= 0xFFFF) ? (turnTimerIncrement + atbValue) : 0xFFFF;
            actorTimers.charATBValue = newATBValue;
            if (newATBValue == 0xFFFF) {
                handleActorTurn(actorIdx, gContext.battleActors.getActiveBattleActor(actorIdx));
            }
        }
    }
    fillActorResources();
    srHandleFillLimit();
    for (u8 partyIdx = 0; partyIdx < 3; ++partyIdx) {
        auto& actorTimers = *gContext.battleActors.getActiveBattleActor(partyIdx).actorTimers;
        auto& partyState = getActivePartyMember(partyIdx);
        partyState.gamePartyMember->atbTimer = actorTimers.charATBValue;
    }
    // restrictionMaskRefs(); This references the old Battle Inventory, what ever it is is not working anyway
}

const static u16 escapeTimerTick = 0x2000;
void listenForEscape() {
    u16* G_ESCAPE_TIMER = (u16*)0x9AAD0C;
    u16* G_ESCAPE_FLAG = (u16*)0x9AAD0A;
    u16* word_9AAD08 = (u16*)0x9AAD08;
    u16* word_9AAD06 = (u16*)0x9AAD06;
    u32* dword_9A85D4 = (u32*)0x9A85D4;
    u16* word_9A8B08 = (u16*)0x9A8B08;
    u16* word_9AAD28 = (u16*)0x9AAD28;
    u16* word_9AAD0E = (u16*)0x9AAD0E;
    u16* G_FORMATION_TYPE = (u16*)(0x9AAD20);
    *word_9A8B08 &= 0xFFFEu;
    bool listenForEscape = *G_ESCAPE_FLAG && (*G_ESCAPE_FLAG == *word_9AAD0E);
    if (listenForEscape) {
        if ((*dword_9A85D4 & 0xC) == 12) {
            *word_9AAD06++;
        }
        *word_9AAD08++;
        *G_ESCAPE_TIMER += getBattleSpeed();
        if (*G_ESCAPE_TIMER > escapeTimerTick) {
            *G_ESCAPE_TIMER -= escapeTimerTick;
            bool canEscape = (*word_9AAD28 < 0x4000 || *G_FORMATION_TYPE == 1 || *G_FORMATION_TYPE == 3);
            if ((*word_9AAD08 < 2 * *word_9AAD06) && canEscape) {
                --* G_ESCAPE_FLAG; //dispatchEscape
            }
            *word_9AAD08 = 0;
            *word_9AAD06 = 0;
        }
    }
}

typedef u32(*SRPFN_43740B)(u32);
static SRPFN_43740B sub_43740B = (SRPFN_43740B)0x43740B;
typedef u16(*SRPFN_437420)(u32);
static SRPFN_437420 sub_437420 = (SRPFN_437420)0x437420;
void handleActorTurn(u8 actorIdx, ActorBattleState& actorState) {
    u16* word_9AAD04 = (u16*)0x9AAD04;
    u16* word_9AAD1C = (u16*)0x9AAD1C;
    u32* dword_9AE208 = (u32*)0x9AE208;
    UpdateActorTimersEvent updateTimersEvent{ &actorState };
    dispatchEvent(ON_ACTOR_ATB_FULL, &updateTimersEvent);
    if (actorIdx >= 3 || actorState.actorBattleVars->stateFlags & 0x10) {
        gameIssueChosenAction(actorIdx, 0xFFFF, 0, 0);
    }
    else if ((1 << actorIdx) & *word_9AAD1C) {
        auto v0 = sub_43740B(actorIdx);
        gameIssueChosenAction(actorIdx, v0, 0, 0);
    }
    else {
        sub_437420(actorIdx);
        *word_9AAD04 |= 1 << actorIdx;
    }
    actorState.actorTimers->limitFastATBMask &= 0xFFFEu;
    *dword_9AE208 |= 2u;

}


static PFNSR_INTSUB gameFlagActorHasLimit = (PFNSR_INTSUB)0x4375CE;
void srHandleFillLimit() {
    u32* dword_9AE17C = (u32*)0x9AE17C;
    for (u8 actorIdx = 0; actorIdx < 3; ++actorIdx) {
        auto& actorState = gContext.battleActors.getActiveBattleActor(actorIdx);
        auto& partyState = getActivePartyMember(actorIdx);
        auto currentLimitDisplay = partyState.gamePartyMember->limitGuage >> 8;
        auto limitBarCopy = actorState.party34->limitBarCopy;
        auto& aiContext = *AI_BATTLE_CONTEXT;
        if (aiContext.unkBattleFlags & 8) {
            limitBarCopy = 255 * (*dword_9AE17C / aiContext.unkLimitDivisor);
            limitBarCopy = (limitBarCopy < 255) ? limitBarCopy : 255;
        }
        if (currentLimitDisplay < limitBarCopy) {
            u16 newLimitDisplay = currentLimitDisplay + 4;
            if (newLimitDisplay > limitBarCopy)
                newLimitDisplay = limitBarCopy;
            if (newLimitDisplay == 255) {
                gameFlagActorHasLimit(actorIdx);
                gameCreateEvent(1, actorIdx, 1, 0);
            }
            partyState.gamePartyMember->limitGuage= newLimitDisplay << 8;
        }
    }
}

void srUpdateActorTimers(u8 actorIdx) {
    srSetActorTimers(getActiveBattleActor(actorIdx));
}

void srSetActorTimers(ActorBattleState& battleActor) {
    u16* word_9AAD1E = (u16*)0x9AAD1E;
    auto statusMask = (i32)battleActor.actorBattleVars->statusMask;
    auto& actorTimers = *battleActor.actorTimers;

    if (statusMask < 0) {
        switch (*word_9AAD1E) {
        case 1:
            statusMask |= (1 << STS_PARALYSIS_IDX);
            break;
        case 0:
        case 3:
            statusMask |= (1 << STS_BLIND_IDX);
            break;
        }
    }

    auto srStatus = battleActor.activeStatuses;
    auto vTimerIncrement = 0;
    auto cTimerIncrement = 0;
    if (!(srActorHasStatus(battleActor, StatusNames::STOP) || srActorHasStatus(battleActor, StatusNames::DEATH))) {
        if (srActorHasStatus(battleActor, StatusNames::SLOW)) {
            vTimerIncrement = getBattleSpeed();
            cTimerIncrement = 34;
        }
        else if (srActorHasStatus(battleActor, StatusNames::HASTE)) {
            vTimerIncrement = 3 * getBattleSpeed();
            cTimerIncrement = 136;
        }
        else {
            vTimerIncrement = 2 * getBattleSpeed();;
            cTimerIncrement = 68;
        }
    }

    auto& stats = *battleActor.battleStats;;
    auto agilityFactor = 1 + (stats[StatNames::AGILITY].activeValue / 128.0f);
    auto turnTimer = (agilityFactor * vTimerIncrement) / 2;

    if (srActorHasStatus(battleActor, StatusNames::STOP) || srActorHasStatus(battleActor, StatusNames::PARALYSIS) || srActorHasStatus(battleActor, StatusNames::SLEEP))
        turnTimer = 0;

    actorTimers.vTimerIncrement = vTimerIncrement;
    actorTimers.turnTimerIncrement = turnTimer;
    actorTimers.cTimerIncrement = cTimerIncrement;

    UpdateActorTimersEvent updateTimersEvent{ &battleActor };
    dispatchEvent(UPDATE_ACTOR_TIMERS, &updateTimersEvent);
}
