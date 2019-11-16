#include "battle_targeting_draw_callbacks.h"

using namespace BattleMenuWidgetNames;

void drawTargetingCursors(const BattleDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    /*This should be a draw callback, probably*/
    if (event->menuState != BATTLE_TARGETING_STATE) {
        return;
    }
    gameDrawTargetingCursors(0xDC1768, *BATTLE_TARGETED_GLOBAL);
}

