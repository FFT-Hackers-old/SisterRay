#include "stagger.h"
#include "../battle/battle_engine_api.h"
#include "../events/event_bus_interface.h"
#include "../impl.h"
#include "../gamedata/summons.h"
#include "../gamedata/battle_stats.h"
#include "../battle/battle_actors.h"
#include "../gamedata/damage_callback_utils.h"
#include "../battle/effects/effects_api.h"
#include "../battle/battle_actor_interface.h"

#define STAGGER_MOD_NAME "STGMODNAME"

void loadStagger() {
    registerStat(StaggerStatNames::STAGGER.c_str(), "Stagger", 1024, 0);
    registerStat(StaggerStatNames::MAX_STAGGER.c_str(), "Stagger", 1024, 255);
    registerStat(StaggerStatNames::STAGGER_INFLT_RES.c_str(), "Stagger Res", 100, 0);
    registerStat(StaggerStatNames::STAGGER_INFLT_PEN.c_str(), "Stagger Pen", 100, 0);
    SrStatusBehavior behavior = { false, false, 0, false, false };
    registerStatus(
        StaggerStatNames::STAGGER_STATUS.c_str(),
        "Staggered",
        StaggerStatNames::STAGGER_INFLT_PEN.c_str(),
        StaggerStatNames::STAGGER_INFLT_RES.c_str(),
        behavior
    );
    srAddListener(INIT_PLAYER_PARTY_MEMBER, (SrEventCallback)initializePartyStaggerStats, STAGGER_MOD_NAME);
    srAddListener(INIT_ENEMY, (SrEventCallback)initializeEnemyStaggerStats, STAGGER_MOD_NAME);
    srAddListener(INIT_ATTACK, (SrEventCallback)initializeAttackStaggerStats, STAGGER_MOD_NAME);
    srAddListener(ON_FINALIZE_IMPACT_EVENTS, (SrEventCallback)setStaggerToInflict, STAGGER_MOD_NAME);
    srAddListener(TRIGGER_DAMAGE_DISPLAY, (SrEventCallback)triggerStaggerDamage, STAGGER_MOD_NAME);
    srAddListener(POST_DAMAGE_FORMULA, (SrEventCallback)applyStaggerDamageMedifiers, STAGGER_MOD_NAME);
    srAddListener(INIT_BATTLE_ACTORS, (SrEventCallback)staggerInitActors, STAGGER_MOD_NAME);
    srAddListener(ON_VTIMER_TICK, (SrEventCallback)decayStaggerGuage, STAGGER_MOD_NAME);
    srAddListener(UPDATE_ACTOR_TIMERS, (SrEventCallback)handleStaggeredActorATB, STAGGER_MOD_NAME);
    initializeStaggerMenuElement();
}


// Should probably be data driven
void initializeEnemyStaggerStats(InitEnemyEvent* enemyEvent) {
    enemyEvent->enemyState->enemyStats[StaggerStatNames::STAGGER].baseValue = 0;
    enemyEvent->enemyState->enemyStats[StaggerStatNames::STAGGER].statValue = 0;

    enemyEvent->enemyState->enemyStats[StaggerStatNames::MAX_STAGGER].baseValue = 512;
    enemyEvent->enemyState->enemyStats[StaggerStatNames::MAX_STAGGER].statValue = 512;

    enemyEvent->enemyState->enemyStats[StaggerStatNames::STAGGER_INFLT_PEN].baseValue = 0;
    enemyEvent->enemyState->enemyStats[StaggerStatNames::STAGGER_INFLT_RES].statValue = 0;
}

void initializeAttackStaggerStats(InitAttackEvent* attackEvent) {
    attackEvent->attack->stats[StaggerStatNames::STAGGER_DAMAGE].statValue = 16;
}


void initializePartyStaggerStats(InitPartyMemberEvent* partyEvent) {
    partyEvent->partyState->srPartyMember->stats[StaggerStatNames::STAGGER].baseValue = 0;
    partyEvent->partyState->srPartyMember->stats[StaggerStatNames::STAGGER].statValue = 0;

    partyEvent->partyState->srPartyMember->stats[StaggerStatNames::MAX_STAGGER].baseValue = 512;
    partyEvent->partyState->srPartyMember->stats[StaggerStatNames::MAX_STAGGER].statValue = 512;

    partyEvent->partyState->srPartyMember->stats[StaggerStatNames::STAGGER_INFLT_PEN].baseValue = 512;
    partyEvent->partyState->srPartyMember->stats[StaggerStatNames::STAGGER_INFLT_RES].statValue = 512;
}


void initializeSummonStaggerStats(InitSummonEvent* summonEvent) {

}

void initializeStaggerMenuElement() {
    srAddListener(INIT_BATTLE_MENU, (SrEventCallback)initStaggerBarWidget, STAGGER_MOD_NAME);
    srAddListener(DRAW_BATTLE_MENU, (SrEventCallback)drawStaggerBarWidget, STAGGER_MOD_NAME);

}

void initStaggerBarWidget(const MenuInitEvent* event) {
    auto mainWidget = event->menu->menuWidget;

    std::vector<std::string> names = { StaggerWidgetNames::ENEMY_BAR_1, StaggerWidgetNames::ENEMY_BAR_2, StaggerWidgetNames::ENEMY_BAR_3,
        StaggerWidgetNames::ENEMY_BAR_4, StaggerWidgetNames::ENEMY_BAR_5, StaggerWidgetNames::ENEMY_BAR_6 };

    for (auto idx = 0; idx < 6; idx++) {
        auto enemyStaggerWidget = allocateStaggerBarWidget(names[idx].c_str(), 0, 0);
        addChildWidget(mainWidget, (Widget*)enemyStaggerWidget, names[idx]);
    }
}

Widget* allocateStaggerBarWidget(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto atbDataWidget = createWidget(name);
    moveWidget(atbDataWidget, xCoordinate, yCoordinate);

    DrawGameAssetParams barBorder = BarBorder(xCoordinate, yCoordinate, 0.9f, 40, 4);
    addChildWidget(atbDataWidget, (Widget*)createGameAssetWidget(barBorder, std::string("BORDER")), std::string("BORDER"));

    DrawBarParams atbBarParams = { xCoordinate + 6, yCoordinate, 0, 4, 0, 0.9f };
    addChildWidget(atbDataWidget, (Widget*)createBarWidget(atbBarParams, std::string("BAR")), std::string("BAR"));
    return atbDataWidget;
}

void drawStaggerBarWidget(const MenuDrawEvent* event) {
    auto mainWidget = event->menu->menuWidget;

    std::vector<std::string> names = { StaggerWidgetNames::ENEMY_BAR_1, StaggerWidgetNames::ENEMY_BAR_2, StaggerWidgetNames::ENEMY_BAR_3,
        StaggerWidgetNames::ENEMY_BAR_4, StaggerWidgetNames::ENEMY_BAR_5, StaggerWidgetNames::ENEMY_BAR_6 };

    for (auto idx = 4; idx < 10; idx++) {
        auto enemyStaggerWidget = srGetChild(mainWidget, names[idx - 4].c_str());
        auto& screenSpacePosition = getActorScreenSpacePosition(idx);
        if (screenSpacePosition.actorPosition.x == 0xFFFF || screenSpacePosition.actorPosition.y == 0xFFFF) {
            disableWidget(enemyStaggerWidget);
            continue;
        }
        if (!gContext.battleActors.isActorInBattle(idx)) {
            disableWidget(enemyStaggerWidget);
            continue;
        }
        enableWidget(enemyStaggerWidget);
        moveWidget(enemyStaggerWidget, screenSpacePosition.actorPosition.x - 20, screenSpacePosition.actorPosition.y + 40);
        auto& currentStagger = gContext.battleActors.getActiveBattleActor(idx).battleStats->at(StaggerStatNames::STAGGER);
        auto& incrementCtx = currentStagger.incrementCtx;
        if (srActorHasStatus(gContext.battleActors.getActiveBattleActor(idx), StaggerStatNames::STAGGER_STATUS)) {
            updateBarColor((BarWidget*)srGetChild(enemyStaggerWidget, "BAR"), 0xFFDF0000);
        }
        else {
            updateBarColor((BarWidget*)srGetChild(enemyStaggerWidget, "BAR"), 0x2A6CF500);
        }
        if (incrementCtx.barDisplayValue != 1024) {
            if (incrementCtx.incrementTargetValue > incrementCtx.barDisplayValue) {
                incrementCtx.barDisplayValue += 5;
                if (incrementCtx.barDisplayValue > incrementCtx.incrementTargetValue) {
                    incrementCtx.barDisplayValue == incrementCtx.incrementTargetValue;
                }
            }
            if (incrementCtx.incrementTargetValue < incrementCtx.barDisplayValue) {
                incrementCtx.barDisplayValue -= 5;
                if (incrementCtx.barDisplayValue < incrementCtx.incrementTargetValue) {
                    incrementCtx.barDisplayValue == incrementCtx.incrementTargetValue;
                }
            }
        }
        updateBarLength((BarWidget*)srGetChild(enemyStaggerWidget, "BAR"), (incrementCtx.barDisplayValue / 16));

    }
}

// This should probably be an extra event type
void setStaggerToInflict(SrActionImpactSetupEvent* srEvent) {
    auto& targetState = srEvent->damageCtx->srDamageContext->targetState;
    auto& attackerState = srEvent->damageCtx->srDamageContext->attackerState;
    auto& attackStagger = srEvent->damageCtx->srDamageContext->attackStats[StaggerStatNames::STAGGER_DAMAGE].statValue;
    u16 hitStaggerDamage = (attackStagger / 16) * (1 + (attackerState.battleStats->at(StatNames::STAGGER_POWER).activeValue / 100)) * 300;
    hitStaggerDamage = hitStaggerDamage - (hitStaggerDamage * (targetState.battleStats->at(StatNames::STAGGER_RES).activeValue / 100.0f));
    if (srActorHasStatus(targetState, StaggerStatNames::STAGGER_STATUS)) {
        hitStaggerDamage /= 3;
    };
    srEvent->damageCtx->srDamageContext->miscComputation["CUMULATIVE_STAGGER"] += hitStaggerDamage;
    u16 staggerAfterInflict = srEvent->damageCtx->srDamageContext->miscComputation["CUMULATIVE_STAGGER"] + targetState.battleStats->at(StaggerStatNames::STAGGER).activeValue;
    if (!srActorHasStatus(targetState, StaggerStatNames::STAGGER_STATUS)) {
        if ((staggerAfterInflict >= 1024) && (targetState.battleStats->at(StaggerStatNames::STAGGER).activeValue < 1024)) {
            srEvent->impactEvent->impactSoundID = 0x98;
            srInflictStatus(targetState, StaggerStatNames::STAGGER_STATUS);
        }
    }
    srEvent->srExtendedEvent->staggerDamage = hitStaggerDamage;

}

void triggerStaggerDamage(TriggerDamageDisplayEvent* srEvent) {
    auto& stagger = srEvent->targetState->battleStats->at(StaggerStatNames::STAGGER);
    stagger.incrementCtx.incrementTargetValue = stagger.activeValue + srEvent->extendedImpactEvent->staggerDamage;
    if (stagger.incrementCtx.incrementTargetValue > 1024) {
        stagger.incrementCtx.incrementTargetValue = 1024;
    }
    if ((stagger.incrementCtx.incrementTargetValue == 1024) && (!srActorHasStatus(*srEvent->targetState, StaggerStatNames::STAGGER_STATUS))) {
        auto reactionCtx = (TargetReactionEffectCtx*)getEffect100QueueTop();
        triggerScreenFlash(0x8A, 0x67, 0xA1);
    }
    stagger.statValue = stagger.incrementCtx.incrementTargetValue;
    stagger.activeValue = stagger.statValue;
}

void applyStaggerDamageMedifiers(DamageCalculationEvent* srEvent) {
    auto& targetState = srEvent->srDamageContext->targetState;
    auto& attackerState = srEvent->srDamageContext->attackerState;
    if (srActorHasStatus(targetState, StaggerStatNames::STAGGER_STATUS)) {
        srEvent->damageContext->currentDamage *= (1 + (attackerState.battleStats->at(StatNames::STAGGER_POWER).activeValue / 100.0f));
    };
}

void staggerInitActors(InitBattleActorEvent* initActorEvent) {
    initActorEvent->ownerState->battleStats->at(StaggerStatNames::STAGGER).activeValue = 0;
}

void handleStaggeredActorATB(UpdateActorTimersEvent* updateTimerEvent) {
    if (srActorHasStatus(*updateTimerEvent->srActor, StaggerStatNames::STAGGER_STATUS)) {
        updateTimerEvent->srActor->actorTimers->turnTimerIncrement = 0.1f * updateTimerEvent->srActor->actorTimers->turnTimerIncrement;
        return;
    }
}

void decayStaggerGuage(UpdateActorTimersEvent* srTimerEvent) {
    auto& stagger = *getBattleActorStat(srTimerEvent->srActor, StaggerStatNames::STAGGER.c_str());
    u16 decrement = 20;
    bool isStaggered = srActorHasStatus(*srTimerEvent->srActor, StaggerStatNames::STAGGER_STATUS);
    if (isStaggered) {
        decrement *= 3;
    }
    auto decayedTarget = stagger.activeValue - decrement;
    if (stagger.activeValue > 0) {
        if (decayedTarget < 0) {
            decayedTarget = 0;
            if (isStaggered) {
                srRemoveStatus(*srTimerEvent->srActor, StaggerStatNames::STAGGER_STATUS);
            }
        }
        stagger.statValue = decayedTarget;
        stagger.incrementCtx.incrementTargetValue = decayedTarget;
        stagger.activeValue = stagger.statValue;
    }
}
