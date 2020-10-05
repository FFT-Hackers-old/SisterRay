#include "battle_summon_draw_callbacks.h"
#include "../../common_widgets.h"

using namespace BattleMenuWidgetNames;

void drawBattleSummonViewWidget(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    if (event->menuState != BATTLE_SUMMON_STATE) {
        disableWidget(getChild(menuWidget, BATTLE_SUMMON_WIDGET_NAME));
        return;
    }
    enableWidget(getChild(menuWidget, BATTLE_SUMMON_WIDGET_NAME));
    updateActionsActor(getChild(menuWidget, BATTLE_SUMMON_WIDGET_NAME), *BATTLE_ACTIVE_ACTOR_ID, event->menu, BATTLE_SUMMON_STATE);
}
