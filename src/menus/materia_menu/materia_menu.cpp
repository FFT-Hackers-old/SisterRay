#include "materia_menu.h"
#include "../../impl.h"
#include "windows.h"
#include "../menu_utils.h"

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

//Cursors will also need to be done by callback registered to a particular menu state
void displayActiveCursorStates(Menu* menu, u16 menuState, u32 updateStateMask) {
    u16 item_ID;

    sub_6C98A6();
    auto checkArrangeCursor = getStateCursor(menu, 0);
    auto slotChoiceCursor = getStateCursor(menu, 1);
    switch (menu->currentState) {
        case 0: {
            drawCursor(checkArrangeCursor, 0.1f);
            break;
        }
        case 1: {
            auto slotChoiceCursor = getStateCursor(menu, 1);
            drawCursor(slotChoiceCursor, 0.1f);
            break;
        }
        case 2: {
            auto materiahoiceCursor = getStateCursor(menu, 1);
            drawFlashingCursor(slotChoiceCursor, updateStateMask, 0.1f);
            drawCursor(materiahoiceCursor);
        }
        case 3: {
            drawFlashingCursor(checkArrangeCursor, updateStateMask, 0.1f);
            drawCursor(getStateCursor(menu, 3));
            break;
        }
        case 4: {
            drawFlashingCursor(checkArrangeCursor, updateStateMask, 0.1f);
            drawCursor(getStateCursor(menu, 4));
            break;
        }
        case 5: {
            drawFlashingCursor(checkArrangeCursor, updateStateMask, 0.1f);
            drawCursor(getStateCursor(menu, 5));
        }
        case 6: {
            drawFlashingCursor(checkArrangeCursor, updateStateMask, 0.1f);
            drawCursor(getStateCursor(menu, 5));
        }
        default:
            break;
    }

    /*switch (MateriaMenuState)
    {
        case 8:
            goto LABEL_30;
        case 9:
            displayCursor(362, 26 * dword_DD14B4 + 214, 0.001);
        LABEL_30:
            if (MateriaMenuState == 9)
            {
                if (updateStateMask & 2)
                    displayCursor(
                        *(signed __int16 *)(dword_DD1218 + 40) - 30,
                        *(signed __int16 *)(dword_DD1218 + 42) + 26 * dword_DD147C + 11,
                        0.001);
            }
            else
            {
                displayCursor(
                    *(signed __int16 *)(dword_DD1218 + 40) - 30,
                    *(signed __int16 *)(dword_DD1218 + 42) + 26 * dword_DD147C + 11,
                    0.001);
            }
            for (i = 0; i < 4; ++i)
                displayTextAtLocation(
                    *(signed __int16 *)(dword_DD1218 + 40) + 13,
                    *(signed __int16 *)(dword_DD1218 + 42) + 26 * i + 9,
                    &byte_920BA8[20 * (i + 10)],
                    7u,
                    0.0099999998);
            LOWORD(v1) = (unsigned int)drawMenuBox((signed __int16 *)(dword_DD1218 + 40), 0.0099999998);
            break;
        case 10:
            if (updateStateMask & 2)
                displayCursor(347, 26 * dword_DD14B4 + 214, 0.0);
            displayCursor(
                *(signed __int16 *)(dword_DD1218 + 48) + 53,
                *(signed __int16 *)(dword_DD1218 + 50) + 26 * dword_DD14EC + 51,
                0.0);
            displayTextAtLocation(
                *(signed __int16 *)(dword_DD1218 + 48) + 13,
                *(signed __int16 *)(dword_DD1218 + 50) + 13,
                aO_15,
                7u,
                0.0);
            displayTextAtLocation(
                *(signed __int16 *)(dword_DD1218 + 48) + 107,
                *(signed __int16 *)(dword_DD1218 + 50) + 45,
                asc_920B3C,
                7u,
                0.0);
            displayTextAtLocation(
                *(signed __int16 *)(dword_DD1218 + 48) + 107,
                *(signed __int16 *)(dword_DD1218 + 50) + 73,
                a7ait,
                7u,
                0.0);
            LOWORD(v1) = (unsigned int)drawMenuBox((signed __int16 *)(dword_DD1218 + 48), 0.0);
            break;
        default:
            return v1;
            */
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
