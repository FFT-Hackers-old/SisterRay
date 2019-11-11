
#include "battle_command_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initializeBattleCommandMenu() {
    CursorContext commandSelection = { 0, 0, 4, 4, 0, 0, 4, 4, 0, 0, 1, 1, 0, 0 };
    Cursor cmdChoiceCursor = { commandSelection, 156, 350, 24, 88 };
    auto battleMenu = gContext.menuWidgets.get_element(BATTLE_MENU_NAME);
    setStateCursor(battleMenu, BATTLE_CMD_STATE, summonChoiceCursor, 0);
    setStateCursor(battleMenu, BATTLE_CMD_STATE, summonChoiceCursor, 1);
    setStateCursor(battleMenu, BATTLE_CMD_STATE, summonChoiceCursor, 2);
}

void registerCommandMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_MENU});
    gContext.eventBus.addListener(INIT_BATTLE_COMMAND, (SrEventCallback)&initBattleCommandViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleSelectCommand, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&handleExitSpell, modName, contextKeys);
}
