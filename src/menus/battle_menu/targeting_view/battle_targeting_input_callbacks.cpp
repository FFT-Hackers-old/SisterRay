#include "battle_input_targeting_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"
#include "../../../battle/engine/battle_engine_interface.h"

using namespace BattleMenuWidgetNames;

void handleSelectTargets(const MenuInputEvent* event) {
    if (!ACCEPTING_BATTLE_INPUT) {
        if (byte_BFB2EC || word_DC38D0) {
                playMenuSound(3);
                ACCEPTING_BATTLE_INPUT = 1;
                return;
        }
        if (*W_COMMAND_ENABLED) {
            if (*W_COMMAND_ENABLED == 2) {
                executeWCommand();
                return;
            }
            auto menuChoice = getStateCursor(event->menu, getMenuState(event->menu), *BATTLE_ACTIVE_ACTOR_ID)->context;
            auto flatIndex = menuChoice.baseRowIndex + menuChoice.relativeRowIndex;
            if (*ISSUED_COMMAND_ID == CMD_W_ITEM) {
                gContext.battleInventory->decrementInventoryEntry(flatIndex, 1);
            }
            *W_COMMAND_ENABLED = *W_COMMAND_ENABLED + 1;
            *W_FIRST_ACTION_USED = *ISSUED_ACTION_ID;
            *W_FIRST_ACTION_INDEX = *ISSUED_ACTION_MENU_INDEX;
            *W_FIRST_TARGETING_CURSOR_TYPE = *ISSUED_ACTION_TARGET_TYPE;
            *W_FIRST_VALID_TARGET_MASK = *ISSUED_ACTION_VALID_TARGET_MASK;
            *W_FIRST_TARGET_INDEX = *ISSUED_ACTION_TARGET_INDEX;
            *W_FIRST_STARTING_ROW = *ISSUED_ACTION_STARTING_ROW;
            setMenuState(event->menu, *PREVIOUS_BATTLE_MENU_STATE);
             word_DC38E0 = 1;
            *ACCEPTING_BATTLE_INPUT = 1;
            /*if (byte_DC207D)
                setViewState3(21);
            }*/
            return;
        }

        if ((*ISSUED_COMMAND_ID == CMD_ITEM || *ISSUED_COMMAND_ID == CMD_W_ITEM || *ISSUED_COMMAND_ID = CMD_THROW)){
            gContext.battleInventory->decrementInventoryEntry(flatIndex, 1);
            dispatchBattleCommand();
        }
    }
}

void handleExitSelectTargets(const MenuInputEvent* event) {
    if (*W_COMMAND_ENABLED == 2 && (*ISSUED_COMMAND_ID == CMD_W_ITEM)) {
        const BattleInventoryEntry& inventoryEntry = gContext.battleInventory->get_resource(*W_FIRST_ACTION_INDEX);
        if (inventoryEntry.item_id == 0xFFFF) {
            gContext.battleInventory->addResourceAtIndex(*W_FIRST_ACTION_INDEX, *W_FIRST_ACTION_USED);
        }
        gContext.battleInventory->incrementInventoryEntry(*W_FIRST_ACTION_INDEX, 1);
    }
    playMenuSound(4);
    setMenuState(event->menu, *PREVIOUS_BATTLE_MENU_STATE);
    word_DC38E0 = 1;
    *ACCEPTING_BATTLE_INPUT = 1;
    if (byte_DC207D)
        setViewState3(21);
}
}
