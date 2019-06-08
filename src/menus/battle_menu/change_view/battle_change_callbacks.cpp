
#include "battle_change_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleChangeWidgetNames;

#define CHANGE_MENU_ACTION_ACOUNT 3

void initializeBattleChangeMenu() {
    CursorContext changeContext = { 0, 0, 1 , CHANGE_MENU_ACTION_ACOUNT , 0, 0, 1, CHANGE_MENU_ACTION_ACOUNT, 0, 0, 0, 0, 0, 1 };
    Cursor changeCursor = { changeContext, 8, 364, 32, 156 };

    Cursor cursorArray[3] = { changeCursor, changeCursor, changeCursor }; /*One cursor per party member*/
    auto battleSpellMenu = createMenu(INIT_BATTLE_CHANGE, 3, &cursorArray[0]);
    gContext.menuWidgets.add_element("BATTLE_CHANGE", battleSpellMenu);
    gContext.menuWidgets.initializeMenu("BATTLE_CHANGE", BATTLE_CHANGE_MENU_NAME);
}

void registerChangeMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_CHANGE_VIEW});
    gContext.eventBus.addListener(INIT_BATTLE_CHANGE, (SrEventCallback)&initBattleChangeViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleSelectChange, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&handleExitChange, modName, contextKeys);
}
