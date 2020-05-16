#ifndef STAGGER_H
#define STAGGER_H

#include <SisterRay/SisterRay.h>
#include <string>

namespace StaggerStatNames {
    const std::string STAGGER = "STAGGER";
    const std::string MAX_STAGGER = "MAX_STAGGER";
}

namespace StaggerWidgetNames {
    const std::string ENEMY_BAR_1 = "1_STG_ENEMY";
    const std::string ENEMY_BAR_2 = "2_STG_ENEMY";
    const std::string ENEMY_BAR_3 = "3_STG_ENEMY";
    const std::string ENEMY_BAR_4 = "4_STG_ENEMY";
    const std::string ENEMY_BAR_5 = "5_STG_ENEMY";
    const std::string ENEMY_BAR_6 = "6_STG_ENEMY";
}

void initializeEnemyStaggerStats(InitEnemyEvent* enemyEvent);
void initializePartyStaggerStats(InitPartyMemberEvent* partyEvent);
void initStaggerBarWidget(const MenuInitEvent* event);
Widget* allocateStaggerBarWidget(const char* name, i32 xCoordinate, i32 yCoordinate);
#endif // !STAGGER_H
