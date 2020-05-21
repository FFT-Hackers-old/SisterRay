#include "status_menu.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../menu.h"
#include <memory>

using namespace StatusWidgetNames;

SISTERRAY_API void statusMenuUpdateHandler(i32 updateStateMask) {
    Menu* menu = gContext.menuWidgets.getElement("STAT_MENU");
    renderMenuType();
    runMenu(menu, updateStateMask);
}
