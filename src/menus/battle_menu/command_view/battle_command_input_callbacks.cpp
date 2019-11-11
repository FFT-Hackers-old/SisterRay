#include "battle_spell_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"

using namespace BattleSpellWidgetNames;

/*Spell selection handler*/
void handleSelectCommand(const BattleSpellInputEvent* event) {
    u8* ISSUED_COMMAND_ID = (u8*)0xDC3C70;
    u16* VALID_TARGET_MASK = (u16*)0xDC3C60;
    u16* word_9A889A = (u16*)0x9A889A;
    u8* W_COMMAND_ENABLED = (u8*)0xDC3C88;
    auto commandChoiceCursor = getStateCursor(event->menu, event->menuState, *BATTLE_ACTIVE_ACTOR_ID)->context;
    auto& enabledCommands = PARTY_STRUCT_ARRAY[*BATTLE_ACTIVE_ACTOR_ID].enabledCommandArray;

    if (*ACCEPTING_BATTLE_INPUT)
        return;

    if (event->menuState != BATTLE_CMD_STATE) {
        return;
    }

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (commandChoiceCursor.maxColumnBound * (commandChoiceCursor.relativeRowIndex + commandChoiceCursor.baseRowIndex)) + commandChoiceCursor.relativeColumnIndex;
    if (enabledCommands[flatIndex].commandFlags & 2 || enabledCommands[flatIndex].magicIndex == 255) {
        playMenuSound(3);
    }
    else {
        auto& command = enabledCommands[flatIndex];
        *ISSUED_COMMAND_ID = command.commandID;
        *VALID_TARGET_MASK = word_9A889A;
        *W_COMMAND_ENABLED = 0;
        runSelectCallbacks(command, event->menu);
    }
}
