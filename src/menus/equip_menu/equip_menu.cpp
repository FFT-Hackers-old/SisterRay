#include "equip_menu.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../menu.h"
#include <memory>

using namespace EquipWidgetNames;

SISTERRAY_API void equipMenuUpdateHandler(i32 updateStateMask) {
    Menu* menu = gContext.menuWidgets.getElement("EQUIP_MENU");

    renderMenuType();
    runMenu(menu, updateStateMask);
    displayMenuCursors(menu, getMenuState(menu), updateStateMask);
}

/*Need a self-updating bar*/
void displayMenuObjects(CursorContext* cursorContextArray, u32 menuState, i32 stateControlMask) {
    u16 equippableGearCount;

    switch (menuState) {
        case 1: {
            //sub_6FA12F(316, 171, 324, 303); //I can't figure out what this does...
            equippableGearCount = gContext.gearViewData.slotsInUse;
            drawScrollerParams* arguments = (drawScrollerParams*)word_DCA490;
            setSideScrollerArguments(arguments, 8, equippableGearCount, cursorContextArray[1].baseRowIndex, 618, 171, 17, 303);
            renderMenuSideScroller(arguments, 0.2);
            //sub_6FA347();
            break;
        }
    }
}

/*Create a Focus object making this configurable*/
void displayMenuCursors(Menu* menu, u16 menuState, u32 stateControlMask) {
    auto gearTypeChoiceCursor = getStateCursor(menu, 0);
    switch (menuState) {
        case 0: {
            drawCursor(gearTypeChoiceCursor, 0.1f);
            break;
        }
        case 1: {
            auto gearChoiceCursor = getStateCursor(menu, 1);
            drawFlashingCursor(gearTypeChoiceCursor, stateControlMask, 0.1f);
            drawCursor(gearChoiceCursor, 0.1f);
            break;
        }
        default: {
            break;
        }
    }
}
