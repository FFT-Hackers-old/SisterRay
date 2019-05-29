#include "battle_summon_menu.h"
#include "../../../impl.h"

using namespace BattleSummonWidgetNames;

void battleSummonUpdateHandler(i32 updateStateMask) {
    Menu* menuObject = gContext.menuWidgets.get_element("BATTLE_SUMMON");

    sub_6C98A6();
    auto menuWidget = menuObject->menuWidget;
    setMenuState(menuObject, *BATTLE_ACTIVE_ACTOR_ID); //Here each party member is treated as a "menu state" for the view, which we set to always match the greater battle menu

    /*This should be a draw callback, probably*/
    std::vector<std::string> names = { PARTY_1_SPELL_GRID, PARTY_2_SPELL_GRID, PARTY_3_SPELL_GRID };
    for (i32 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID) {
            enableWidget(getChild(getChild(menuWidget, BATTLE_SUMMON_MENU_NAME), names[partyIdx]));
            break;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_SUMMON_MENU_NAME), names[partyIdx]));
    }
    /*End lazy draw callback*/

    BattleSpellInputEvent event = { menuObject, menuObject->currentState };
    gContext.eventBus.dispatch(DRAW_BATTLE_SUMMON, &event);
    drawWidget(menuWidget);

    drawCursor(getStateCursor(menuObject, event.menuState), 0.1f); /*Here we just draw the cursor for each state, as they are independent party states*/

    if (!is_input_handling_enabled() && !(*BATTLE_PAUSED)) {
        dispatchMenuInput(updateStateMask, menuObject, BATTLE_SUMMON_VIEW);
    }
}
