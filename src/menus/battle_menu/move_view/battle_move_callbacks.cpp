
#include "battle_move_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initializeBattleMoveMenu() {
    CursorContext commandSelection = { 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 1, 1, 0, 0 };
    Cursor moveChoiceCursor = { commandSelection, 5, 350, 24, 88 };
    auto battleMenu = gContext.menuWidgets.getElement(BATTLE_MENU_NAME);
    setStateCursor(battleMenu, BATTLE_MOVE_STATE, moveChoiceCursor, 0);
    setStateCursor(battleMenu, BATTLE_MOVE_STATE, moveChoiceCursor, 1);
    setStateCursor(battleMenu, BATTLE_MOVE_STATE, moveChoiceCursor, 2);
}

void registerMoveMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_MENU});
    gContext.eventBus.addListener(INIT_BATTLE_MENU, (SrEventCallback)&initBattleMoveViewWidget, modName);
    gContext.eventBus.addListener(DRAW_BATTLE_MENU, (SrEventCallback)&drawBattleMoveViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleSelectMove, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_RIGHT, (SrEventCallback)&goToCommand, modName, contextKeys);
}
