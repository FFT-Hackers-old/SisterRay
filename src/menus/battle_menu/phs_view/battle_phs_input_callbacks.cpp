#include "battle_phs_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"
#include "../../../battle/battle_engine_interface.h"

using namespace BattleMenuWidgetNames;

/*Spell selection handler*/
void handleSelectPHS(const MenuInputEvent* event) {
    if (!checkHandlingInput())
        return;

    if (event->menuState != BATTLE_PHS_STATE)
        return;

    auto phsChoice = getStateCursor(event->menu, event->menuState, *BATTLE_ACTIVE_ACTOR_ID)->context;
    auto& inactiveParty = getActivePartyMember(*BATTLE_ACTIVE_ACTOR_ID).srPartyMember->partySwapBuffer;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (phsChoice.maxColumnBound * (phsChoice.relativeRowIndex + phsChoice.baseRowIndex)) + phsChoice.relativeColumnIndex;
    if (inactiveParty[flatIndex] == 0xFF) {
        playMenuSound(3);
        return;
    }
    playMenuSound(1);
    setChosenActionID(inactiveParty[flatIndex]);
    setChosenActionMenuIndex(flatIndex);
    dispatchChosenBattleAction();
    setMenuState(event->menu, BATTLE_INACTIVE);
    *PREVIOUS_BATTLE_MENU_STATE = BATTLE_PHS_STATE;
}


void handleExitPHS(const MenuInputEvent* event) {
    if (event->menuState != BATTLE_PHS_STATE)
        return;
    playMenuSound(4);
    *ACCEPTING_BATTLE_INPUT = 1;
    setMenuState(event->menu, BATTLE_CMD_STATE);
}
