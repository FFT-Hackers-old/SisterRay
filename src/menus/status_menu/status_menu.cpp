#include "status_menu.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../menu.h"
#include <memory>
#include "status_widget_names.h"

using namespace StatusWidgetNames;

SISTERRAY_API void statusMenuUpdateHandler(i32 updateStateMask) {
    Menu* menu = gContext.menuWidgets.getElement(STATUS_MENU_NAME);
    renderMenuType();
    runMenu(menu, updateStateMask);
    displayStatMenuCursors( menu, getMenuState(menu), updateStateMask);
}

void displayStatMenuCursors(Menu* menu, u16 menuState, u32 stateControlMask) {
    auto gearTypeChoiceCursor = getStateCursor(menu, 1);
    switch (menuState) {
    case 0: {
        break;
    }
    case 1: {
        auto gearChoiceCursor = getStateCursor(menu, 1);
        drawCursor(gearChoiceCursor, 0.1f);
        break;
    }
    default: {
        break;
    }
    }
}
