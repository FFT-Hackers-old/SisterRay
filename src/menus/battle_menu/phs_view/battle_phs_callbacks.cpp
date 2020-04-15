
#include "battle_phs_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initializeBattlePHSMenu() {
    CursorContext phsSelection = { 0, 0, 3, 1, 0, 0, 3, 2, 0, 0, 0, 0, 0, 1 };
    Cursor phsChoiceCursor = { phsSelection, 8, 364, 22, 160 };
    auto battleMenu = gContext.menuWidgets.getElement(BATTLE_MENU_NAME);
    setStateCursor(battleMenu, BATTLE_PHS_STATE, phsChoiceCursor, 0);
    setStateCursor(battleMenu, BATTLE_PHS_STATE, phsChoiceCursor, 1);
    setStateCursor(battleMenu, BATTLE_PHS_STATE, phsChoiceCursor, 2);
}

void registerPHSViewListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_MENU});
    gContext.eventBus.addListener(DRAW_BATTLE_MENU, (SrEventCallback)&drawBattlePHSViewWidget, modName);
    gContext.eventBus.addListener(INIT_BATTLE_MENU, (SrEventCallback)&initBattlePHSViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleSelectPHS, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&handleExitPHS, modName, contextKeys);
}
