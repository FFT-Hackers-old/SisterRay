#include "battle_command_menu.h"
#include "../../../impl.h"

using namespace BattleCommandWidgetNames;

void battleCommandUpdateHandler(i32 updateStateMask) {
    Menu* menuObject = gContext.menuWidgets.get_element("BATTLE_SPELL");

    sub_6C98A6();
    auto menuWidget = menuObject->menuWidget;
    setMenuState(menuObject, *BATTLE_ACTIVE_ACTOR_ID); //Here each party member is treated as a "menu state" for the view, which we set to always match the greater battle menu

    std::vector<std::string> names = { PARTY_1_CMD_GRID, PARTY_2_CMD_GRID, PARTY_3_CMD_GRID };
    for (i32 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID) {
            enableWidget(getChild(getChild(menuWidget, BATTLE_COMMAND_MENU_NAME), names[partyIdx]));
            continue;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_COMMAND_MENU_NAME), names[partyIdx]));
    }

    auto commandChoiceCursor = getStateCursor(menuObject, *BATTLE_ACTIVE_ACTOR_ID);
    commandChoiceCursor->context.maxColumnBound = PARTY_STRUCT_ARRAY[*BATTLE_ACTIVE_ACTOR_ID].commandColumns;
    commandChoiceCursor->context.viewColumnBound = PARTY_STRUCT_ARRAY[*BATTLE_ACTIVE_ACTOR_ID].commandColumns;
    resizeBox(getChild(menuWidget, CMD_GRID_BOX), 135, 350, 88 * commandChoiceCursor->context.maxColumnBound, 130);


    BattleCommandInputEvent event = { menuObject, menuObject->currentState };
    gContext.eventBus.dispatch(DRAW_BATTLE_COMMAND, &event);
    drawWidget(menuWidget);
    drawCursor(getStateCursor(menuObject, event.menuState), 0.1f); /*Here we just draw the cursor for each state, as they are independent party states*/
}

void battleCommandInputHandler() {
    if (*ACCEPTING_BATTLE_INPUT)
        return;

    if (*BATTLE_MENU_STATE != 6)
        return;

    Menu* menuObject = gContext.menuWidgets.get_element("BATTLE_COMMAND");
    dispatchMenuInput(0, menuObject, BATTLE_COMMAND_VIEW);
}
