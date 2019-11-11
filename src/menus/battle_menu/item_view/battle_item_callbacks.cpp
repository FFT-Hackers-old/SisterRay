#include "battle_item_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleItemWidgetNames;

void initializeBattleItemMenu() {
    CursorContext itemContext = { 0, 0, 1, 3, 0, 0, 1, gContext.battleInventory->slots_in_use, 0, 0, 0, 0, 0, 1 };
    Cursor itemCursor = { itemContext, 36, 364, 32, 156 };
    auto battleMenu = gContext.menuWidgets.get_element(BATTLE_MENU_NAME);
    setStateCursor(battleMenu, BATTLE_ITEM_STATE, summonChoiceCursor, 0);
    setStateCursor(battleMenu, BATTLE_ITEM_STATE, summonChoiceCursor, 1);
    setStateCursor(battleMenu, BATTLE_ITEM_STATE, summonChoiceCursor, 2);
}

void registerItemMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_MENU});
    gContext.eventBus.addListener(INIT_BATTLE_ITEM, (SrEventCallback)&initBattleItemViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleSelectItem, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&handleExitItem, modName, contextKeys);
}
