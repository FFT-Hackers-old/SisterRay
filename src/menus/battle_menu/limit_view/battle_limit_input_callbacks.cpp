#include "battle_limit_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"
#include "../../../battle/battle_engine_interface.h"

using namespace BattleMenuWidgetNames;

/*Spell selection handler*/
void handleEnterLimit(const MenuInputEvent* event) {
    if (!checkHandlingInput())
        return;
    if (event->menuState != BATTLE_LIMIT_STATE)
        return;

    auto limitChoiceCursor = getStateCursor(event->menu, event->menuState, *BATTLE_ACTIVE_ACTOR_ID)->context;
    auto& limitBreaks = gContext.party.getActivePartyCharacter(*BATTLE_ACTIVE_ACTOR_ID).limitBreaks;
    u16* restoreTypeGlobal = (u16*)(0xDC2088);

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (limitChoiceCursor.maxColumnBound * (limitChoiceCursor.relativeRowIndex + limitChoiceCursor.baseRowIndex)) + limitChoiceCursor.relativeColumnIndex;
    if (limitBreaks[flatIndex] == 0xFFFF) {
        srLogWrite("Trying to dispatch LIMIT at flatindex %d action %d", flatIndex, limitBreaks[flatIndex]);
        playMenuSound(3);
        return;
    }
    playMenuSound(1);
    auto action = getCommandAction(CMD_LIMIT, limitBreaks[flatIndex]);
    setChosenActionID(limitBreaks[flatIndex]);
    setChosenActionMenuIndex(flatIndex);
    setTargetingFromFlags(action.attackData.targetingFlags, false);
    setMenuState(event->menu, BATTLE_TARGETING_STATE);
    *PREVIOUS_BATTLE_MENU_STATE = BATTLE_LIMIT_STATE;
    //auto restoreType = gContext.attacks.getElement(assemblekey(2, enabledSpells[flatIndex].magicIndex)).attackData.restoreTypes;
    //*restoreTypeGlobal = restoreType;
    // if (*restoreTypeGlobal != 0xFFFF)
    //    initHandlerCursorState(-1, -1, 21); //Open up the "restore view" if it's defined
}


void handleExitLimit(const MenuInputEvent* event) {
    if (event->menuState != BATTLE_LIMIT_STATE)
        return;

    playMenuSound(4);
    *ACCEPTING_BATTLE_INPUT = 1;
    setMenuState(event->menu, BATTLE_CMD_STATE);
}
