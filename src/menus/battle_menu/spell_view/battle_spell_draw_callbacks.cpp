#include "battle_spell_draw_callbacks.h"
#include "../../common_widgets.h"

using namespace BattleMenuWidgetNames;

void drawBattleSpellViewWidget(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    /*This should be a draw callback, probably*/
    if (event->menuState != BATTLE_MAGIC_STATE) {
        disableWidget(getChild(menuWidget, BATTLE_SPELL_WIDGET_NAME));
        return;
    }

    enableWidget(getChild(menuWidget, BATTLE_SPELL_WIDGET_NAME));
    updateActionsActor(getChild(menuWidget, BATTLE_SPELL_WIDGET_NAME), *BATTLE_ACTIVE_ACTOR_ID, event->menu, BATTLE_MAGIC_STATE);
}
