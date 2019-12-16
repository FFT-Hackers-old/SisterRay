#include "battle_summon_draw_callbacks.h"

using namespace BattleMenuWidgetNames;

void drawBattleSummonViewWidget(const MenuDrawEvent* event) {

    auto menuWidget = event->menu->menuWidget;
    if (event->menuState != BATTLE_SUMMON_STATE) {
        disableWidget(getChild(menuWidget, BATTLE_SUMMON_WIDGET_NAME));
        return;
    }

    enableWidget(getChild(menuWidget, BATTLE_SUMMON_WIDGET_NAME));
    std::vector<std::string> names = { PARTY_1_SUMMON_GRID, PARTY_2_SUMMON_GRID, PARTY_3_SUMMON_GRID };
    for (u32 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID) {
            enableWidget(getChild(getChild(menuWidget, BATTLE_SUMMON_WIDGET_NAME), names[partyIdx]));
            setActiveCursorIndex(event->menu, BATTLE_SUMMON_STATE, *BATTLE_ACTIVE_ACTOR_ID);
            continue;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_SUMMON_WIDGET_NAME), names[partyIdx]));
    }
}
