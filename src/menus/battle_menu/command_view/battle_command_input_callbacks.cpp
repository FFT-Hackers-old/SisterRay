#include "battle_command_input_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"
#include "../../../battle/battle_engine_interface.h"

using namespace BattleMenuWidgetNames;

/*Spell selection handler*/
void handleSelectCommand(const MenuInputEvent* srMenuEvent) {
    if (!checkHandlingInput())
        return;

    if (srMenuEvent->menuState != BATTLE_CMD_STATE) {
        return;
    }

    auto commandChoiceCursor = getStateCursor(srMenuEvent->menu, srMenuEvent->menuState, *BATTLE_ACTIVE_ACTOR_ID)->context;
    auto& enabledCommands = getGamePartyMember(*BATTLE_ACTIVE_ACTOR_ID)->enabledCommandArray;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (commandChoiceCursor.maxColumnBound * (commandChoiceCursor.relativeColumnIndex + commandChoiceCursor.baseColumnIndex)) + commandChoiceCursor.relativeRowIndex;
    if (enabledCommands[flatIndex].commandFlags & 2 || enabledCommands[flatIndex].commandID == 255) {
        playMenuSound(3);
        return;
    }

    playMenuSound(1);
    auto& command = enabledCommands[flatIndex];
    setChosenCommandID(command.commandID);
    setChosenActionValidTargetMask(*word_9A889A);
    *PREVIOUS_BATTLE_MENU_STATE = BATTLE_CMD_STATE;
    *W_COMMAND_ENABLED = 0;
    runSelectCallbacks(command, srMenuEvent->menu);
}

void openMove(const MenuInputEvent* srMenuEvent) {
    if (srMenuEvent->menuState != BATTLE_CMD_STATE) {
        return;
    }

    auto commandChoiceCursor = getStateCursor(srMenuEvent->menu, srMenuEvent->menuState, *BATTLE_ACTIVE_ACTOR_ID)->context;
    if (commandChoiceCursor.relativeColumnIndex != 0)
        return;

    playMenuSound(1);
    *PREVIOUS_BATTLE_MENU_STATE = BATTLE_CMD_STATE;
    setMenuState(srMenuEvent->menu, BATTLE_MOVE_STATE);
}


void toggleLimit(const MenuInputEvent* srMenuEvent) {
    if (srMenuEvent->menuState != BATTLE_CMD_STATE) {
        return;
    }

    auto partyState = gContext.party.getActivePartyMember(*BATTLE_ACTIVE_ACTOR_ID);
    if (partyState.gamePartyMember->limitGuage != 0xFF00) {
        return;
    }

    toggleBack(*BATTLE_ACTIVE_ACTOR_ID, 0);
    playMenuSound(1);
}
