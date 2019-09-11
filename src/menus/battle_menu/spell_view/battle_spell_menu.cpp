#include "battle_spell_menu.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void battleSpellUpdateHandler(i32 updateStateMask) {
    Menu* menuObject = gContext.menuWidgets.get_element("BATTLE_SPELL");

    sub_6C98A6();
    auto menuWidget = menuObject->menuWidget;


    BattleSpellInputEvent event = { menuObject, menuObject->currentState };
    gContext.eventBus.dispatch(DRAW_BATTLE_SPELL, &event);
    drawWidget(menuWidget);
    drawCursor(getStateCursor(menuObject, event.menuState), 0.1f); /*Here we just draw the cursor for each state, as they are independent party states*/
}

void battleSpellInputHandler() {
    if (*ACCEPTING_BATTLE_INPUT)
        return;

    if (*BATTLE_MENU_STATE != 6)
        return;
    Menu* menuObject = gContext.menuWidgets.get_element("BATTLE_SPELL");
    dispatchMenuInput(0, menuObject, BATTLE_SPELL_VIEW);
}
