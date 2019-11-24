#include "battle_item_draw_callbacks.h"

using namespace BattleMenuWidgetNames;

void drawBattleItemViewWidget(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    if (event->menuState != BATTLE_ITEM_STATE) {
        disableWidget(getChild(menuWidget, BATTLE_ITEM_WIDGET_NAME));
        return;
    }

    /*This should be a draw callback, probably*/
    enableWidget(getChild(menuWidget, BATTLE_ITEM_WIDGET_NAME));
    std::vector<std::string> names = { PARTY_1_ITEM_GRID, PARTY_2_ITEM_GRID, PARTY_3_ITEM_GRID };
    for (i32 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID) {
            enableWidget(getChild(getChild(menuWidget, BATTLE_ITEM_WIDGET_NAME), names[partyIdx]));
            setActiveCursorIndex(event->menu, BATTLE_ITEM_STATE, *BATTLE_ACTIVE_ACTOR_ID);
            continue;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_ITEM_WIDGET_NAME), names[partyIdx]));
    }
}
