#include "status_menu_callbacks.h"
#include "status_widget_names.h"
#include "../menu.h"
#include <unordered_set>
#include "../../impl.h"

using namespace StatusWidgetNames;

void initializeStatMenu() {
    auto statusMenu = createMenu(INIT_STATUS_MENU, DRAW_STATUS_MENU, STATUS_MENU_CONTEXT, 3);
    gContext.menuWidgets.addElement(STATUS_MENU_NAME, statusMenu);
    gContext.menuWidgets.initializeMenu(STATUS_MENU_NAME, STATUS_WIDGET_NAME);
}

void registerStatMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({ STATUS_MENU_CONTEXT });

    gContext.eventBus.addListener(INIT_STATUS_MENU, (SrEventCallback)&initStatCharDataWidget, modName);
    gContext.eventBus.addListener(DRAW_STATUS_MENU, (SrEventCallback)&handleStatusMenuChangeCharacter, modName);

    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&exitMenuListener, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_L1, (SrEventCallback)&changeCharLeft, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_R1, (SrEventCallback)&changeCharRight, modName, contextKeys);
}
