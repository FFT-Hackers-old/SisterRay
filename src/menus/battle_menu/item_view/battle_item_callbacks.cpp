#include "battle_item_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initializeBattleItemMenu() {
    CursorContext itemContext = { 0, 0, 1, 3, 0, 0, 1, gContext.battleInventory->slotsInUse, 0, 0, 0, 0, 0, 1 };
    Cursor itemChoiceCursor = { itemContext, 36, 364, 32, 156 };
    auto battleMenu = gContext.menuWidgets.getElement(BATTLE_MENU_NAME);
    setStateCursor(battleMenu, BATTLE_ITEM_STATE, itemChoiceCursor, 0);
    setStateCursor(battleMenu, BATTLE_ITEM_STATE, itemChoiceCursor, 1);
    setStateCursor(battleMenu, BATTLE_ITEM_STATE, itemChoiceCursor, 2);
}

void registerItemMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_MENU});
    gContext.eventBus.addListener(INIT_BATTLE_MENU, (SrEventCallback)&initBattleItemViewWidget, modName);
    gContext.eventBus.addListener(DRAW_BATTLE_MENU, (SrEventCallback)&drawBattleItemViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleSelectItem, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&handleExitItem, modName, contextKeys);
}
