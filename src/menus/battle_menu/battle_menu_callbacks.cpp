#include "battle_menu_callbacks.h"
#include "../menu.h"
#include <unordered_set>
#include "../../impl.h"

using namespace BattleMenuWidgetNames;

void initializeBattleMenu() {
    auto battleItemMenu = createMenu(INIT_BATTLE_MENU, 32, &cursorArray[0]);
    gContext.menuWidgets.add_element(BATTLE_MENU_NAME, battleItemMenu);
    gContext.menuWidgets.initializeMenu(BATTLE_MENU_NAME, BATTLE_MENU_WIDGET_NAME);
}

void registerItemMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    gContext.eventBus.addListener(INIT_BATTLE_ITEM, (SrEventCallback)&initBattleItemViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleSelectItem, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&handleExitItem, modName, contextKeys);
}
