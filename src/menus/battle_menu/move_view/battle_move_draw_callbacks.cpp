#include "battle_move_draw_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void drawBattleMoveViewWidget(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;

    if (event->menuState != BATTLE_MOVE_STATE) {
        disableWidget(getChild(menuWidget, BATTLE_MOVE_WIDGET_NAME));
        return;
    }
    // updateCommandsActive(*BATTLE_ACTIVE_ACTOR_ID, 0);

    enableWidget(getChild(menuWidget, BATTLE_MOVE_WIDGET_NAME));
    std::vector<std::string> names = { PARTY_1_MOVE_GRID, PARTY_2_MOVE_GRID, PARTY_3_MOVE_GRID };
    for (i32 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID) {
            enableWidget(getChild(getChild(menuWidget, BATTLE_MOVE_WIDGET_NAME), names[partyIdx]));
            setActiveCursorIndex(event->menu, BATTLE_MOVE_STATE, *BATTLE_ACTIVE_ACTOR_ID);
            continue;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_MOVE_WIDGET_NAME), names[partyIdx]));
    }
}
