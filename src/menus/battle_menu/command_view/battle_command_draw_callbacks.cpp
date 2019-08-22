#include "battle_summon_draw_callbacks.h"

using namespace BattleMenuWidgetNames;

void drawBattleSCommandViewWidget(const BattleSpellDrawEvent* event) {

    if (event->menuState != BATTLE_CMD_STATE)
        return;

    auto menuWidget = event->menu->menuWidget;
    /*This should be a draw callback, probably*/
    std::vector<std::string> names = { PARTY_1_CMD_GRID, PARTY_2_CMD_GRID, PARTY_3_CMD_GRID };
    for (i32 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID) {
            enableWidget(getChild(getChild(menuWidget, BATTLE_COMMAND_WIDGET_NAME), names[partyIdx]));
            continue;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_COMMAND_WIDGET_NAME), names[partyIdx]));
    }
}
