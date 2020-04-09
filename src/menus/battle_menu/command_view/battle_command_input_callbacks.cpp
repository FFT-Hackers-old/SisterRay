#include "battle_command_input_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"
#include "../../../battle/engine/battle_engine_interface.h"

using namespace BattleMenuWidgetNames;

/*Spell selection handler*/
void handleSelectCommand(const MenuInputEvent* event) {
    if (!checkHandlingInput())
        return;

    if (event->menuState != BATTLE_CMD_STATE) {
        return;
    }

    auto commandChoiceCursor = getStateCursor(event->menu, event->menuState, *BATTLE_ACTIVE_ACTOR_ID)->context;
    auto& enabledCommands = getGamePartyMember(*BATTLE_ACTIVE_ACTOR_ID)->enabledCommandArray;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (commandChoiceCursor.maxColumnBound * (commandChoiceCursor.relativeRowIndex + commandChoiceCursor.baseRowIndex)) + commandChoiceCursor.relativeColumnIndex;
    if (enabledCommands[flatIndex].commandFlags & 2 || enabledCommands[flatIndex].commandID == 255) {
        playMenuSound(3);
        return;
    }

    playMenuSound(1);
    auto& command = enabledCommands[flatIndex];
    setChosenCommandID(command.commandID);
    setChosenActionValidTargetMask(*word_9A889A);
    *PREVIOUS_BATTLE_MENU_STATE = BATTLE_CMD_STATE;
    srLogWrite("Transitioning to targeting state");
    *W_COMMAND_ENABLED = 0;
    runSelectCallbacks(command, event->menu);
}
