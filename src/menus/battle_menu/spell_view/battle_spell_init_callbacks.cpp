#include "battle_spell_init_callbacks.h"
#include "../../common_widgets.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initBattleMagicViewWidget(const MenuInitEvent* event) {
    auto mainWidget = event->menu;
    auto battleSummonView = createActionViewWidget(BATTLE_SPELL_WIDGET_NAME, 0, 340, battleSpellNameViewUpdater, BATTLE_MENU_NAME.c_str(), BATTLE_MAGIC_STATE);
    addChildWidget(mainWidget, battleSummonView, BATTLE_SPELL_WIDGET_NAME);
}
