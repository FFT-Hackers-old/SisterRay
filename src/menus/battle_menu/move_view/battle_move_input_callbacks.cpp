#include "battle_move_input_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"
#include "../../../battle/battle_engine_interface.h"

using namespace BattleMenuWidgetNames;

/*Spell selection handler*/
void handleSelectMove(const MenuInputEvent* srMenuEvent) {
    /*if (!checkHandlingInput())
        return;*/

    if (srMenuEvent->menuState != BATTLE_MOVE_STATE) {
        return;
    }

    auto moveChoiceCursor = getStateCursor(srMenuEvent->menu, srMenuEvent->menuState, *BATTLE_ACTIVE_ACTOR_ID)->context;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (moveChoiceCursor.maxColumnBound * (moveChoiceCursor.relativeRowIndex + moveChoiceCursor.baseRowIndex)) + moveChoiceCursor.relativeColumnIndex;
    switch (flatIndex) {
    case 2: {
        srLogWrite("In Open PHS Handler");
        /*if (!checkHandlingInput()) {
            srLogWrite("Aborting PHS Open due to input freeze");
            return;
        }*/

        if (srMenuEvent->menuState != BATTLE_MOVE_STATE) {
            srLogWrite("Aborting PHS Open due to menuState mismatch");
            return;
        }

        srLogWrite("Transitioning to PHS state");
        playMenuSound(1);
        *PREVIOUS_BATTLE_MENU_STATE = BATTLE_MOVE_STATE;
        setChosenCommandID(gContext.commands.getResourceIndex("CMD_PHS"));
        setMenuState(srMenuEvent->menu, BATTLE_PHS_STATE);
        break;
    }
    }
    /*if (enabledCommands[flatIndex].commandFlags & 2 || enabledCommands[flatIndex].commandID == 255) {
        playMenuSound(3);
        return;
    }*/
}


void goToCommand(const MenuInputEvent* srMenuEvent) {
    srLogWrite("In Open Move View Handler");
    if (srMenuEvent->menuState != BATTLE_MOVE_STATE) {
        srLogWrite("Aborting Move Open due to menuState mismatch");
        return;
    }

    srLogWrite("Transitioning to Move View");
    playMenuSound(1);
    *PREVIOUS_BATTLE_MENU_STATE = BATTLE_MOVE_STATE;
    setMenuState(srMenuEvent->menu, BATTLE_CMD_STATE);
}

// Call this function prior to transitioning into the Portrait display view
typedef void (*SRPFN_GAMELOADPORTRAIT)(u32, u32);
#define gameLoadPortrait ((SRPFN_GAMELOADPORTRAIT)0x6C1ABA)

typedef void(SRPFN_GAMEFREEEFFECT)(void*);
#define gameFreeEffect   ((SRPFN_GAMEFREEEFFECT)0x671082)
void loadPortraits() {

}
