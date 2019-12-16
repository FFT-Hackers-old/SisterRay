#include "battle_targeting_draw_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

typedef void(*pfnsub6E6291)();
#define gameHandleTargetingInput  ((pfnsub6E6291)0x6E6291)

void drawTargetingCursors(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    if (event->menuState != BATTLE_TARGETING_STATE) {
        return;
    }
    srLogWrite("NOW IN TARGETING STATE");
    gameDrawTargetingCursors((void*)0xDC1768, *BATTLE_TARGETED_GLOBAL);
    gameHandleTargetingInput();
}

