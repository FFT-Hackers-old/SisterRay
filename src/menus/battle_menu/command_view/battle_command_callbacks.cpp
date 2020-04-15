
#include "battle_command_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initializeBattleCommandMenu() {
    CursorContext commandSelection = { 0, 0, 4, 4, 0, 0, 4, 4, 0, 0, 1, 1, 0, 0 };
    Cursor cmdChoiceCursor = { commandSelection, 0x90 - 36, 0x150 + 6, 24, 88 };
    auto battleMenu = gContext.menuWidgets.getElement(BATTLE_MENU_NAME);
    setStateCursor(battleMenu, BATTLE_CMD_STATE, cmdChoiceCursor, 0);
    setStateCursor(battleMenu, BATTLE_CMD_STATE, cmdChoiceCursor, 1);
    setStateCursor(battleMenu, BATTLE_CMD_STATE, cmdChoiceCursor, 2);
}

void registerCommandMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_MENU});
    gContext.eventBus.addListener(INIT_BATTLE_MENU, (SrEventCallback)&initBattleCommandViewWidget, modName);
    gContext.eventBus.addListener(DRAW_BATTLE_MENU, (SrEventCallback)&drawBattleCommandViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleSelectCommand, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_LEFT, (SrEventCallback)&openMove, modName, contextKeys);
}
