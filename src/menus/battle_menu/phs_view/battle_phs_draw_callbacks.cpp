#include "battle_phs_draw_callbacks.h"

using namespace BattleMenuWidgetNames;

void drawBattlePHSViewWidget(const MenuDrawEvent* event) {

    auto menuWidget = event->menu->menuWidget;
    if (event->menuState != BATTLE_PHS_STATE) {
        disableWidget(getChild(menuWidget, BATTLE_PHS_WIDGET_NAME));
        return;
    }

    enableWidget(getChild(menuWidget, BATTLE_PHS_WIDGET_NAME));
    std::vector<std::string> names = { PARTY_1_PHS_GRID, PARTY_2_PHS_GRID, PARTY_3_PHS_GRID };
    for (u32 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID) {
            enableWidget(getChild(getChild(menuWidget, BATTLE_PHS_WIDGET_NAME), names[partyIdx]));
            setActiveCursorIndex(event->menu, BATTLE_PHS_STATE, *BATTLE_ACTIVE_ACTOR_ID);
            continue;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_PHS_WIDGET_NAME), names[partyIdx]));
    }
}
