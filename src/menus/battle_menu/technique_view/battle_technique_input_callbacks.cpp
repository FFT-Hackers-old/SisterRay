#include "battle_technique_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"
#include "../../../battle/battle_engine_interface.h"

using namespace BattleMenuWidgetNames;

/*Spell selection handler*/
void handleSelectTechnique(const MenuInputEvent* event) {
    if (!checkHandlingInput())
        return;
    if (event->menuState != BATTLE_TECHNIQUE_STATE)
        return;

    auto& enabledCommands = getActivePartyMember(*BATTLE_ACTIVE_ACTOR_ID).srPartyMember->actorTechniques;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto techniqueChoiceCursor = getStateCursor(event->menu, event->menuState, *BATTLE_ACTIVE_ACTOR_ID)->context;
    auto flatIndex = (techniqueChoiceCursor.maxColumnBound * (techniqueChoiceCursor.relativeColumnIndex + techniqueChoiceCursor.baseColumnIndex)) + techniqueChoiceCursor.relativeRowIndex;
    if (enabledCommands[flatIndex].commandFlags & 2 || enabledCommands[flatIndex].commandID == 255) {
        playMenuSound(3);
        return;
    }

    playMenuSound(1);
    auto& command = enabledCommands[flatIndex];
    setChosenCommandID(command.commandID);
    setChosenActionValidTargetMask(*word_9A889A);
    *PREVIOUS_BATTLE_MENU_STATE = BATTLE_TECHNIQUE_STATE;
    *W_COMMAND_ENABLED = 0;
    runSelectCallbacks(command, event->menu);
}


void handleExitTechnique(const MenuInputEvent* event) {
    if (event->menuState != BATTLE_TECHNIQUE_STATE)
        return;

    playMenuSound(4);
    *ACCEPTING_BATTLE_INPUT = 1;
    setMenuState(event->menu, BATTLE_CMD_STATE);
}
