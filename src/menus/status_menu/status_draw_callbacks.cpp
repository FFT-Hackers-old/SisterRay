#include "status_draw_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../../gamedata/stat_names.h"
#include "../common_widgets.h"

using namespace StatusWidgetNames;

void handleStatusMenuChangeCharacter(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    auto topWidget = getChild(menuWidget, CHAR_DATA_WIDGET_NAME);
    updatePortraitPartyIndex(getChild(topWidget, PORTRAIT_WIDGET_NAME), *STAT_MENU_ACTIVE_ACTOR));
    updateHPBarPartyIndex(getChild(topWidget, HPBAR_WIDGET_NAME), *STAT_MENU_ACTIVE_ACTOR));

    enableWidget(getChild(menuWidget, STATUS_COMMAND_WIDGET_NAME));
    updateCommandsActor(getChild(menuWidget, STATUS_COMMAND_WIDGET_NAME), *STAT_MENU_ACTIVE_ACTOR);
    resizeCommandBox(*STAT_MENU_ACTIVE_ACTOR, getChild(menuWidget, STATUS_COMMAND_WIDGET_NAME));
}

