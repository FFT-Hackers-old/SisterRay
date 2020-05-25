#include "status_draw_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../../gamedata/stat_names.h"
#include "../common_widgets.h"

using namespace StatusWidgetNames;

void handleStatusMenuChangeCharacter(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    auto topWidget = getChild(menuWidget, CHAR_DATA_WIDGET_NAME);
    updatePortraitPartyIndex(getChild(topWidget, PORTRAIT_WIDGET_NAME), *STAT_MENU_ACTIVE_ACTOR);
    updateHPBarPartyIndex(getChild(topWidget, HPBAR_WIDGET_NAME), *STAT_MENU_ACTIVE_ACTOR);

    enableWidget(getChild(getChild(menuWidget, PRIM_STATS_WIDGET_NAME), STATUS_COMMAND_WIDGET_NAME));
    updateCommandsActor(getChild(getChild(menuWidget, PRIM_STATS_WIDGET_NAME), STATUS_COMMAND_WIDGET_NAME), *STAT_MENU_ACTIVE_ACTOR);
    resizeCommandBox(*STAT_MENU_ACTIVE_ACTOR, getChild(getChild(menuWidget, PRIM_STATS_WIDGET_NAME), STATUS_COMMAND_WIDGET_NAME));
}


void handleActivateCombatStats(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    if (event->menuState == 1) {
        enableWidget(getChild(menuWidget, COMBAT_STATS_WIDGET));
        disableWidget(getChild(menuWidget, PRIM_STATS_WIDGET_NAME));
        return;
    }
    enableWidget(getChild(menuWidget, PRIM_STATS_WIDGET_NAME));
    disableWidget(getChild(menuWidget, COMBAT_STATS_WIDGET));
}
