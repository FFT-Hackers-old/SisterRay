#include "battle_command_init_callbacks.h"
#include "../../common_widgets.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initBattleCommandViewWidget(const MenuInitEvent* event) {
    auto mainWidget = event->menu->menuWidget;
    auto battleCommandView = createCommandViewWidget(BATTLE_COMMAND_WIDGET_NAME.c_str(), 0x90, 0x154, &battleCommandNameViewUpdater, BATTLE_MENU_NAME.c_str(), BATTLE_CMD_STATE);
    addChildWidget(mainWidget, battleCommandView, BATTLE_COMMAND_WIDGET_NAME);
}

void battleCommandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    baseCommandNameViewUpdater(self, widget, flatIndex, BATTLE_ACTIVE_ACTOR_ID);
}
