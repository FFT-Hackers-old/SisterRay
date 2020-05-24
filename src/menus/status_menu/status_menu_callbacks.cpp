#include "status_menu_callbacks.h"
#include "status_widget_names.h"
#include "../menu.h"
#include <unordered_set>
#include "../../impl.h"

using namespace StatusWidgetNames;

void initializeStatMenu() {
    gContext.menuWidgets.initializeMenu(STATUS_MENU_NAME, STATUS_WIDGET_NAME);
}

void createStatMenu() {
    auto statusMenu = createMenu(INIT_STATUS_MENU, DRAW_STATUS_MENU, STATUS_MENU_CONTEXT, 5);
    CursorContext combatStatChoice = { 0, 0, 2, 11, 0, 0, 2, 11, 0, 0, 0, 0, 0, 1 };
    Cursor combatStatCursor = { combatStatChoice, 20, 120, 20, 220 };

    setStateCursor(statusMenu, 1, combatStatCursor);
    gContext.menuWidgets.addElement(STATUS_MENU_NAME, statusMenu);
}

void registerStatMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({ STATUS_MENU_CONTEXT });

    gContext.eventBus.addListener(INIT_STATUS_MENU, (SrEventCallback)&initStatCharDataWidget, modName);
    gContext.eventBus.addListener(INIT_STATUS_MENU, (SrEventCallback)&initPrimaryStatsView, modName);
    gContext.eventBus.addListener(INIT_STATUS_MENU, (SrEventCallback)&initStatCombatDataWidget, modName);
    gContext.eventBus.addListener(DRAW_STATUS_MENU, (SrEventCallback)&handleStatusMenuChangeCharacter, modName);
    gContext.eventBus.addListener(DRAW_STATUS_MENU, (SrEventCallback)&handleActivateCombatStats, modName);

    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&statExitMenuListener, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_L1, (SrEventCallback)&statChangeCharLeft, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_R1, (SrEventCallback)&statChangeCharRight, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_SQUARE, (SrEventCallback)&displayCombatStats, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_SQUARE, (SrEventCallback)&exitCombatStats, modName, contextKeys);
}
