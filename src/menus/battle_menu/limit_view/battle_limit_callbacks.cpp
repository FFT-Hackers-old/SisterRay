
#include "battle_limit_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initializeBattleLimitMenu() {
    CursorContext limitSelection = { 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 0, 0, 0, 1 };
    Cursor spellChoiceCursor = { limitSelection, 8, 364, 32, 156 };
    auto battleMenu = gContext.menuWidgets.getElement(BATTLE_MENU_NAME);
    setStateCursor(battleMenu, BATTLE_LIMIT_STATE, spellChoiceCursor, 0);
    setStateCursor(battleMenu, BATTLE_LIMIT_STATE, spellChoiceCursor, 1);
    setStateCursor(battleMenu, BATTLE_LIMIT_STATE, spellChoiceCursor, 2);
}

void registerLimitMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_MENU});
    gContext.eventBus.addListener(DRAW_BATTLE_MENU, (SrEventCallback)&drawBattleLimitViewWidget, modName);
    gContext.eventBus.addListener(INIT_BATTLE_MENU, (SrEventCallback)&initBattleLimitViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleEnterLimit, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&handleExitLimit, modName, contextKeys);
}
