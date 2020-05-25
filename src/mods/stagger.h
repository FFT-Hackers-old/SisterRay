#ifndef STAGGER_H
#define STAGGER_H

#include <SisterRay/SisterRay.h>
#include <string>

namespace StaggerStatNames {
    const std::string STAGGER = "STAGGER";
    const std::string MAX_STAGGER = "MAX_STAGGER";
    const std::string STAGGER_DAMAGE = "STAGGER_DMG";
    const std::string STAGGER_INFLT_PEN = "STAGGER_PEN";
    const std::string STAGGER_INFLT_RES = "STAG_INFLT_RES";
    const std::string STAGGER_STATUS = "STS_STAGGER";
}

namespace StaggerWidgetNames {
    const std::string ENEMY_BAR_1 = "1_STG_ENEMY";
    const std::string ENEMY_BAR_2 = "2_STG_ENEMY";
    const std::string ENEMY_BAR_3 = "3_STG_ENEMY";
    const std::string ENEMY_BAR_4 = "4_STG_ENEMY";
    const std::string ENEMY_BAR_5 = "5_STG_ENEMY";
    const std::string ENEMY_BAR_6 = "6_STG_ENEMY";
}


void loadStagger();
void initializeEnemyStaggerStats(InitEnemyEvent* enemyEvent);
void initializePartyStaggerStats(InitPartyMemberEvent* partyEvent);
void initializeStaggerMenuElement();
void decayStaggerGuage(UpdateActorTimersEvent* srTimerEvent);
void staggerInitActors(InitBattleActorEvent* initActorEvent);
void handleStaggeredActorATB(UpdateActorTimersEvent* updateTimerEvent);

void initializeAttackStaggerStats(InitAttackEvent* enemyEvent);
void initStaggerBarWidget(const MenuInitEvent* event);
void drawStaggerBarWidget(const MenuDrawEvent* event);
Widget* allocateStaggerBarWidget(const char* name, i32 xCoordinate, i32 yCoordinate);
void setStaggerToInflict(SrActionImpactSetupEvent* srEvent);
void triggerStaggerDamage(TriggerDamageDisplayEvent* srEvent);
void applyStaggerDamageMedifiers(DamageCalculationEvent* srEvent);
#endif // !STAGGER_H
