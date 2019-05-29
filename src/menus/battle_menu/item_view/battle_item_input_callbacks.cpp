#include "battle_item_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"

using namespace BattleItemWidgetNames;

/*Spell selection handler*/
void handleSelectItem(const BattleSpellInputEvent* event) {
    auto itemCursorChoice = getStateCursor(event->menu, event->menuState)->context;
    if (*byte_9AC111) {
        *byte_9AC111 = 0;
        *ACCEPTING_BATTLE_INPUT = 1;
    }
    if (*ACCEPTING_BATTLE_INPUT)
        return;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (itemCursorChoice.maxColumnBound * (itemCursorChoice.relativeRowIndex + itemCursorChoice.baseRowIndex)) + itemCursorChoice.relativeColumnIndex;
    auto itemID = gContext.battleInventory->get_resource(flatIndex).item_id;
    bool didSucceed = didItemUseSucceed(itemID);
    if (didSucceed) {
        playMenuSound(1);
        *ISSUED_ACTION_ID = itemID;
        *GLOBAL_USED_ACTION_TARGET_DATA = gContext.battleInventory->get_resource(flatIndex).targetFlags;
        *GLOBAL_USED_MENU_INDEX = flatIndex;
        setCursorTargetingData();
        *BATTLE_MENU_STATE = 0;
        *PREVIOUS_BATTLE_MENU_STATE = 5;
    }
    else {
        playMenuSound(3);
    }
}


void handleExitItem(const BattleSpellInputEvent* event) {
    playMenuSound(4);
    *ACCEPTING_BATTLE_INPUT = 1;
    *BATTLE_MENU_STATE = 1;
    setHandlerState(5, 3);
}
