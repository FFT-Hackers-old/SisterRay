#include "battle_spell_draw_callbacks.h"

using namespace BattleMenuWidgetNames;

void drawBattleSpellViewWidget(const BattleSpellDrawEvent* event) {

    auto menuWidget = event->menu->menuWidget;
    /*This should be a draw callback, probably*/
    if (event->menuState != BATTLE_MAGIC_STATE)
        disableWidget(getChild(menuWidget, BATTLE_SPELL_WIDGET_NAME))
        return;

    enableWidget(menuWidget, BATTLE_SPELL_WIDGET_NAME);
    std::vector<std::string> names = { PARTY_1_SPELL_GRID, PARTY_2_SPELL_GRID, PARTY_3_SPELL_GRID };
    for (i32 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID) {
            enableWidget(getChild(getChild(menuWidget, BATTLE_SPELL_WIDGET_NAME), names[partyIdx]));
            setActiveCursorIndex(event->menu, BATTLE_MAGIC_STATE, *BATTLE_ACTIVE_ACTOR_ID)
            continue;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_SPELL_WIDGET_NAME), names[partyIdx]));
    }
}
