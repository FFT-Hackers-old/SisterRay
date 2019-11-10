#include "battle_summon_menu.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void battleSummonUpdateHandler(i32 updateStateMask) {
    Menu* menuObject = gContext.menuWidgets.get_element(BATTLE_SUMMON_WIDGET_NAME);

    sub_6C98A6();
    auto menuWidget = menuObject->menuWidget;

    BattleSpellInputEvent event = { menuObject, menuObject->currentState };
    gContext.eventBus.dispatch(DRAW_BATTLE_SUMMON, &event);
    drawWidget(menuWidget);

    drawCursor(getStateCursor(menuObject, *BATTLE_ACTIVE_ACTOR_ID), 0.1f); /*Here we just draw the cursor for each state, as they are independent party states*/
}

void battleSummonInputHandler() {
    if (*ACCEPTING_BATTLE_INPUT)
        return;

    if (*BATTLE_MENU_STATE != SUMMON_BATTLE_STATE)
        return;

    Menu* menuObject = gContext.menuWidgets.get_element(BATTLE_SUMMON_WIDGET_NAME);
    dispatchMenuInput(0, menuObject, BATTLE_SUMMON_VIEW);
}

