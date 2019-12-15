#include "battle_eskill_draw_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void drawBattleESkillViewWidget(const MenuDrawEvent* event) {

    auto menuWidget = event->menu->menuWidget;
    /*This should be a draw callback, probably*/
    if (event->menuState != BATTLE_ESKILL_STATE)
        disableWidget(getChild(menuWidget, BATTLE_ESKILL_WIDGET_NAME));
        return;

    enableWidget(getChild(menuWidget, BATTLE_ESKILL_WIDGET_NAME));
    std::vector<std::string> names = { PARTY_1_ESKILL_GRID, PARTY_2_ESKILL_GRID, PARTY_3_ESKILL_GRID };
    for (u32 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID) {
            enableWidget(getChild(getChild(menuWidget, BATTLE_ESKILL_WIDGET_NAME), names[partyIdx]));
            setActiveCursorIndex(event->menu, BATTLE_ESKILL_STATE, *BATTLE_ACTIVE_ACTOR_ID);
            continue;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_ESKILL_WIDGET_NAME), names[partyIdx]));
    }
}
