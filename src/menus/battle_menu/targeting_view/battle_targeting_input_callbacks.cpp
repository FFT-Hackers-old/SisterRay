#include "battle_targeting_input_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"
#include "../../../battle/engine/battle_engine_interface.h"

using namespace BattleMenuWidgetNames;

void handleSelectTargets(const MenuInputEvent* event) {
    if (event->menuState != BATTLE_TARGETING_STATE)
        return;

    u8* byte_BFB2EC = (u8*)0xBFB2EC;
    u16* word_DC38D0 = (u16*)0xDC38D0;
    u16* word_DC38E0 = (u16*)0xDC38E0;

    if (!*ACCEPTING_BATTLE_INPUT) {
        auto menuChoice = getStateCursor(event->menu, *PREVIOUS_BATTLE_MENU_STATE, *BATTLE_ACTIVE_ACTOR_ID);
        if (menuChoice != nullptr) {
            auto context = menuChoice->context;
            auto flatIndex = (context.maxColumnBound * (context.relativeRowIndex + context.baseRowIndex)) + context.relativeColumnIndex;
            if (*byte_BFB2EC || *word_DC38D0) {
                playMenuSound(3);
                *ACCEPTING_BATTLE_INPUT = 1;
                return;
            }
            if (*W_COMMAND_ENABLED) {
                if (*W_COMMAND_ENABLED == 2) {
                    dispatchFirstWAction();
                    dispatchChosenBattleAction();
                    setMenuState(event->menu, BATTLE_INACTIVE);
                    playMenuSound(1);
                    *ACCEPTING_BATTLE_INPUT = 1;
                    return;
                }
                if (*ISSUED_COMMAND_ID == CMD_W_ITEM) {
                    gContext.battleInventory->decrementInventoryEntry(flatIndex, 1);
                }
                *W_COMMAND_ENABLED = *W_COMMAND_ENABLED + 1;
                storeFirstWCommandIssued();
                setMenuState(event->menu, *PREVIOUS_BATTLE_MENU_STATE);
                *word_DC38E0 = 1;
                *ACCEPTING_BATTLE_INPUT = 1;
                return;
            }
            if ((*ISSUED_COMMAND_ID == CMD_ITEM || *ISSUED_COMMAND_ID == CMD_W_ITEM || *ISSUED_COMMAND_ID == CMD_THROW)) {
                gContext.battleInventory->decrementInventoryEntry(flatIndex, 1);
            }
        }
        playMenuSound(1);
        *ACCEPTING_BATTLE_INPUT = 1;
        setMenuState(event->menu, BATTLE_INACTIVE);
        dispatchChosenBattleAction();
    }
}

void handleExitSelectTargets(const MenuInputEvent* event) {
    if (event->menuState != BATTLE_TARGETING_STATE)
        return;

    u16* word_DC38E0 = (u16*)0xDC38E0;

    if (*W_COMMAND_ENABLED == 2 && (*ISSUED_COMMAND_ID == CMD_W_ITEM)) {
        auto indexChosen = getChosenActionMenuIndex();
        const BattleInventoryEntry& inventoryEntry = gContext.battleInventory->get_resource(indexChosen);
        if (inventoryEntry.item_id == 0xFFFF) {
            gContext.battleInventory->addResourceAtIndex(indexChosen, getChosenActionID());
        }
        gContext.battleInventory->incrementInventoryEntry(indexChosen, 1);
    }
    playMenuSound(4);
    setMenuState(event->menu, *PREVIOUS_BATTLE_MENU_STATE);
    *word_DC38E0 = 1;
    *ACCEPTING_BATTLE_INPUT = 0;
    srLogWrite("Exiting targeting to previous state %d", *PREVIOUS_BATTLE_MENU_STATE);
}



