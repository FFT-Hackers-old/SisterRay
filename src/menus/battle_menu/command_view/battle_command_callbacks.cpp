
#include "battle_command_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleSpellWidgetNames;

void initializeBattleCommandMenu() {
    CursorContext commandSelection = { 0, 0, 4, 4, 0, 0, 4, 4, 0, 0, 1, 1, 0, 0 };
    Cursor cmdChoiceCursor = { commandSelection, 156, 350, 24, 88 };

    Cursor cursorArray[3] = { cmdChoiceCursor, cmdChoiceCursor, cmdChoiceCursor }; /*One cursor per party member*/
    auto battleSpellMenu = createMenu(INIT_BATTLE_COMMAND, 3, &cursorArray[0]);
    gContext.menuWidgets.add_element("BATTLE_COMMAND", battleSpellMenu);
    gContext.menuWidgets.initializeMenu("BATTLE_COMMAND", BATTLE_SPELL_MENU_NAME);
}

void registerCommandMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_COMMAND_VIEW});
    gContext.eventBus.addListener(INIT_BATTLE_COMMAND, (SrEventCallback)&initBattleCommandViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleSelectCommand, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&handleExitSpell, modName, contextKeys);
}
