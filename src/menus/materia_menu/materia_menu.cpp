#include "materia_menu.h"
#include "../../impl.h"
#include "windows.h"
#include "../menu_utils.h"


SISTERRAY_API i32 onEnterInventory() {
    i32 ret;

    ret = sub_714FA3();
    if (*dword_DC130C == 1)
        *dword_DD18C0 = (u32)dword_921C98;
    else
        *dword_DD18C0 = (u32)&((menuWindowConfig)[0]);
    return ret;
}

SISTERRAY_API void materiaMenuUpdateHandler(i32 updateStateMask) {
    Menu* menuObject = gContext.menuWidgets.get_element("MATERIA_MENU");
    auto menuWidget = menuObject->menuWidget;
    MatMenuDrawEvent event = { menuObject, menuObject->currentState };

    gContext.eventBus.dispatch(DRAW_MATERIA_MENU, &event);
    drawWidget(menuWidget);

    displayActiveCursorStates(menuObject, menuObject->currentState, updateStateMask);
    if (!is_input_handling_enabled()) {
        handleMateriaMenuInput(updateStateMask, menuObject);
    }
}

//Need to come up with a better abstract for cursors
void displayActiveCursorStates(Menu* menu, u16 menuState, u32 updateStateMask) {
    u16 item_ID;

    sub_6C98A6();
}

void handleMateriaMenuInput(i32 updateStateMask, Menu* menuObject) {
    auto materiaInventoryState = menuObject->currentState;
    auto cursorArray = getStateCursor(menuObject, materiaInventoryState);
    auto menuWidget = menuObject->menuWidget;

    MateriaInputEvent event = { menuObject, materiaInventoryState };
    handleCursorPositionUpdate((u32*)(&(cursorArray->context)));
    auto dispatchContext = std::vector<SrEventContext>({ MATERIA_MENU_CONTEXT });
    if (checkInputReceived(32)) {
        gContext.eventBus.dispatch(MENU_INPUT_OK, &event, dispatchContext);
    }
    else if (checkInputReceived(64)) {
        gContext.eventBus.dispatch(MENU_INPUT_CANCEL, &event, dispatchContext);
    }
    else if (checkInputReceived(4)) {
        gContext.eventBus.dispatch(MENU_INPUT_L1, &event, dispatchContext);
    }
    else if (checkInputReceived(8)) {
        gContext.eventBus.dispatch(MENU_INPUT_R1, &event, dispatchContext);
    }
    else if (checkInputReceived(128)) { //When switching to the materia view, square
        gContext.eventBus.dispatch(MENU_INPUT_SQUARE, &event, dispatchContext);
    }
    else if (checkInputReceived(16)) { //unequip accessory
        gContext.eventBus.dispatch(MENU_INPUT_TRIANGLE, &event, dispatchContext);
    }
}
