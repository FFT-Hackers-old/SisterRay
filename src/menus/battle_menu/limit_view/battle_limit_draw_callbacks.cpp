#include "battle_limit_draw_callbacks.h"
#include "../../common_widgets.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void drawBattleLimitViewWidget(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    /*This should be a draw callback, probably*/
    if (event->menuState != BATTLE_LIMIT_STATE) {
        disableWidget(getChild(menuWidget, BATTLE_LIMIT_WIDGET_NAME));
        return;
    }

    enableWidget(getChild(menuWidget, BATTLE_LIMIT_WIDGET_NAME));
    updateActionsActor(getChild(menuWidget, BATTLE_LIMIT_WIDGET_NAME), *BATTLE_ACTIVE_ACTOR_ID, event->menu, BATTLE_LIMIT_STATE);
    if (gContext.battleActors.isActorSummon(*BATTLE_ACTIVE_ACTOR_ID)) {
        auto summonIdx = gContext.battleActors.getActiveSummonIdx();
        auto& limits = gContext.summons.getResource(summonIdx).limitBreaks;
        auto& cursor = getStateCursor(event->menu, BATTLE_LIMIT_STATE, *BATTLE_ACTIVE_ACTOR_ID)->context;
        cursor.maxRowBound = ceil((limits.size() / 2.0f));
        cursor.maxColumnBound = (limits.size() == 1) ? 1 : 2;
        cursor.viewColumnBound = (limits.size() == 1) ? 1 : 2;
    }
    else {
        auto& limits = gContext.party.getActivePartyCharacter(*BATTLE_ACTIVE_ACTOR_ID).limitBreaks;
        auto& cursor = getStateCursor(event->menu, BATTLE_LIMIT_STATE, *BATTLE_ACTIVE_ACTOR_ID)->context;
        cursor.maxRowBound = ceil((limits.size() / 2.0f));
        cursor.maxColumnBound = (limits.size() == 1) ? 1 : 2;
        cursor.viewColumnBound = (limits.size() == 1) ? 1 : 2;
    }
}
