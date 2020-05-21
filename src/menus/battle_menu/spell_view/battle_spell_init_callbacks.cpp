#include "battle_spell_init_callbacks.h"
#include "../../common_widgets.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initBattleMagicViewWidget(const MenuInitEvent* event) {
    auto mainWidget = event->menu->menuWidget;
    auto battleSummonView = createActionViewWidget(BATTLE_SPELL_WIDGET_NAME.c_str(), 0, 340, battleSpellNameViewUpdater, BATTLE_MENU_NAME.c_str(), BATTLE_MAGIC_STATE);
    addChildWidget(mainWidget, battleSummonView, BATTLE_SPELL_WIDGET_NAME);
}
