#include "battle_item_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"
#include "../../../battle/engine/battle_engine_interface.h"

using namespace BattleMenuWidgetNames;

/*Spell selection handler*/
void handleSelectItem(const MenuInputEvent* event) {
    if (!checkHandlingInput())
        return;

    if (event->menuState != BATTLE_ITEM_STATE)
        return;

    auto itemCursorChoice = getStateCursor(event->menu, event->menuState, *BATTLE_ACTIVE_ACTOR_ID)->context;
    u16* restoreTypeGlobal = (u16*)(0xDC2088);
    if (*byte_9AC111) {
        *byte_9AC111 = 0;
    }

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (itemCursorChoice.maxColumnBound * (itemCursorChoice.relativeRowIndex + itemCursorChoice.baseRowIndex)) + itemCursorChoice.relativeColumnIndex;
    auto itemID = gContext.battleInventory->get_resource(flatIndex).item_id;
    bool didSucceed = didItemUseSucceed(itemID);
    srLogWrite("ITEM USE SUCCEED BOOL: %d", didSucceed);
    if (didSucceed) {
        playMenuSound(1);
        setChosenActionID(itemID);
        setChosenActionMenuIndex(flatIndex);
        setTargetingFromFlags(gContext.battleInventory->get_resource(flatIndex).targetFlags, false);
        setMenuState(event->menu, BATTLE_TARGETING_STATE);
        *PREVIOUS_BATTLE_MENU_STATE = BATTLE_ITEM_STATE;
        auto restoreType = gContext.items.get_resource(itemID).resource_conditions;
        /*restoreTypeGlobal = restoreType;
        if (*restoreTypeGlobal != 0xFFFF)
            initHandlerCursorState(-1, -1, 21);*/
        // Decrement the item when it is chosen to solve the W-Item bug

        if (*W_COMMAND_ENABLED == 2) {
            gContext.battleInventory->decrementInventoryEntry(flatIndex, 1);
        }
    }
    else {
        playMenuSound(3);
    }
}


void handleExitItem(const MenuInputEvent* event) {
    if (event->menuState != BATTLE_ITEM_STATE)
        return;

    if (*W_COMMAND_ENABLED == 2 && (*ISSUED_COMMAND_ID == CMD_W_ITEM)) {
        const BattleInventoryEntry& inventoryEntry = gContext.battleInventory->get_resource(*W_FIRST_ACTION_INDEX);
        if (inventoryEntry.item_id == 0xFFFF) {
            gContext.battleInventory->addResourceAtIndex(*W_FIRST_ACTION_INDEX, *W_FIRST_ACTION_USED);
        }
        gContext.battleInventory->incrementInventoryEntry(*W_FIRST_ACTION_INDEX, 1);
    }

    playMenuSound(4);
    *ACCEPTING_BATTLE_INPUT = 1;
    setMenuState(event->menu, BATTLE_CMD_STATE);
}
