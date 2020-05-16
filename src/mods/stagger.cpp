#include "stagger.h"
#include "../battle/battle_engine_api.h"
#include "../events/event_bus_interface.h"
#include "../impl.h"
#include "../gamedata/summons.h"

void loadStagger() {
    registerStat(StaggerStatNames::STAGGER.c_str(), "Stagger", 1024, 0);
    registerStat(StaggerStatNames::MAX_STAGGER.c_str(), "Stagger", 1024, 255);
    srAddListener(INIT_PLAYER_PARTY_MEMBER, (SrEventCallback)initializePartyStaggerStats, "STAGGER_MOD");
    srAddListener(INIT_ENEMY, (SrEventCallback)initializeEnemyStaggerStats, "STAGGER_MOD");
}


// Should probably be data driven
void initializeEnemyStaggerStats(InitEnemyEvent* enemyEvent) {
    enemyEvent->enemyState->enemyStats[StaggerStatNames::STAGGER].baseValue = 0;
    enemyEvent->enemyState->enemyStats[StaggerStatNames::STAGGER].statValue = 0;

    enemyEvent->enemyState->enemyStats[StaggerStatNames::MAX_STAGGER].baseValue = 512;
    enemyEvent->enemyState->enemyStats[StaggerStatNames::MAX_STAGGER].statValue = 512;
}


void initializePartyStaggerStats(InitPartyMemberEvent* partyEvent) {
    partyEvent->partyState->srPartyMember->playerStats[StaggerStatNames::STAGGER].baseValue = 0;
    partyEvent->partyState->srPartyMember->playerStats[StaggerStatNames::STAGGER].statValue = 0;

    partyEvent->partyState->srPartyMember->playerStats[StaggerStatNames::MAX_STAGGER].baseValue = 512;
    partyEvent->partyState->srPartyMember->playerStats[StaggerStatNames::MAX_STAGGER].statValue = 512;
}


void initializeSummonStaggerStats(InitSummonEvent* summonEvent) {

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

    DrawGameAssetParams barBorder = BarBorder(xCoordinate, yCoordinate, 0.6f, 40, 4);
    addChildWidget(atbDataWidget, (Widget*)createGameAssetWidget(barBorder, std::string("BORDER")), std::string("BORDER"));

    DrawBarParams atbBarParams = { xCoordinate + 6, yCoordinate, 0, 4, 0, 0.6f };
    addChildWidget(atbDataWidget, (Widget*)createBarWidget(atbBarParams, std::string("BAR")), std::string("BAR"));
    return atbDataWidget;
}

void drawStaggerBarWidget(const MenuDrawEvent* event) {
    auto mainWidget = event->menu->menuWidget;

    std::vector<std::string> names = { StaggerWidgetNames::ENEMY_BAR_1, StaggerWidgetNames::ENEMY_BAR_2, StaggerWidgetNames::ENEMY_BAR_3,
        StaggerWidgetNames::ENEMY_BAR_4, StaggerWidgetNames::ENEMY_BAR_5, StaggerWidgetNames::ENEMY_BAR_6 };

    for (auto idx = 0; idx < 6; idx++) {
        auto enemyStaggerWidget = srGetChild(mainWidget, names[idx].c_str());
    }
}

void setStaggerToInflict() {

}

void triggerStaggerDamage(TriggerDamageDisplayEvent* srEvent) {
    auto& stagger = srEvent->targetState->battleStats->at(StaggerStatNames::STAGGER);
}
