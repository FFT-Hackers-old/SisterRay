#include "battle_change_menu.h"
#include "../../../impl.h"

using namespace BattleChangeWidgetNames;

void battleChangeUpdateHandler(i32 updateStateMask) {
    Menu* menuObject = gContext.menuWidgets.get_element("BATTLE_ESKILL");

    sub_6C98A6();
    auto menuWidget = menuObject->menuWidget;
    setMenuState(menuObject, *BATTLE_ACTIVE_ACTOR_ID); //Here each party member is treated as a "menu state" for the view, which we set to always match the greater battle menu
    BattleSpellInputEvent event = { menuObject, menuObject->currentState };

    std::vector<std::string> names = { PARTY_1_SPELL_GRID, PARTY_2_SPELL_GRID, PARTY_3_SPELL_GRID };
    for (i32 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID) {
            enableWidget(getChild(getChild(menuWidget, BATTLE_CHANGE_MENU_NAME), names[partyIdx]));
            continue;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_CHANGE_MENU_NAME), names[partyIdx]));
    }

    gContext.eventBus.dispatch(DRAW_BATTLE_ESKILL, &event);
    drawWidget(menuWidget);

    drawCursor(getStateCursor(menuObject, event.menuState), 0.1f); /*Here we just draw the cursor for each state, as they are independent party states*/
}

void battleChangeInputHandler() {
    if (*ACCEPTING_BATTLE_INPUT)
        return;

    if (*BATTLE_MENU_STATE != 4)
        return;
    Menu* menuObject = gContext.menuWidgets.get_element("BATTLE_ESKILL");
    dispatchMenuInput(0, menuObject, BATTLE_ESKILL_VIEW);
}
