#include "inventory_menu.h"
#include "../impl.h"
#include "windows.h"
#include "menu_utils.h"
#include "../inventories/inventory_utils.h"

#define SIZE_OF_CURSOR_STRUCT  (i32)0xE // this is the size in i32, for ptr arithmetic

SISTERRAY_API i32 onEnterInventory() {
    i32 ret;

    ret = sub_714FA3();
    if (*dword_DC130C == 1)
        *dword_DD18C0 = (u32)dword_921C98;
    else
        *dword_DD18C0 = (u32)&((menuWindowConfig)[0]);
    return ret;
}

SISTERRAY_API void inventoryMenuUpdateHandler(i32 updateStateMask) {
    Menu* menuObject = gContext.menuWidgets.get_element("INVENTORY_MENU");
    auto menuWidget = menuObject->menuWidget;
    InventoryDrawEvent event = { menuObject, menuObject->currentState };

    gContext.eventBus.dispatch(DRAW_INVENTORY_MENU, &event);
    drawWidget(menuWidget);

    displayActiveCursorStates(updateStateMask);
    if (!is_input_handling_enabled())
    {
        handleInventoryInput(updateStateMask);
    }
}

void displayActiveCursorStates(i32 updateStateMask) {
    CursorContext* cursorContextArray = (CursorContext*)INVENTORY_MENU_CURSOR_CONTEXTS;
    u32 inventoryMenuState = *(INVENTORY_MENU_STATE);
    u32 partyMemberIndex = *(INVENTORY_CURRENT_PARTY_INDEX);
    u32 baseRowIndex = cursorContextArray[1].baseRowIndex;
    u32 relativeRowIndex = cursorContextArray[1].relativeRowIndex;
    u32 inventory_arrange_type = *(INVENTORY_ARRANGE_TYPE);
    u32 baseKeyItemRow;
    u32 relativeKeyItemRow;
    u32 relativeKeyItemColumn;
    u32 baseSortRow;
    u32 relativeSortRow;
    i32 flatKeyItemInventoryIndex;
    const char* fetchedDescription;

    u16 item_ID;

    sub_6C98A6();
    if (inventoryMenuState == 2)
    {
        if (!(*use_on_characters_enabled)) {
            item_ID = gContext.inventory->get_resource(baseRowIndex + relativeRowIndex).item_id;;
            if (!(gContext.item_on_use_data.get_resource(item_ID).target_all))
                displayCursor(0, 120 * partyMemberIndex + 161, 0.0f); 
            else
                displayCursor(0, 120 * (updateStateMask % 3) + 161, 0.0); 
        }
        if (updateStateMask & 2) //This and causes the cursor to flash on repeated updates
            displayCursor(298, 37 * relativeRowIndex + 109, 0.1f);
        if (*use_on_characters_enabled)
            --(*use_on_characters_enabled);
    }
    switch (inventoryMenuState) {
        case 0: {
            displayCursor(93 * cursorContextArray[0].relativeColumnIndex + 13, 26, 0.1f); //display cursor at the selected view
            break;
        }
        case 1: {
            if (updateStateMask & 2)
                displayCursor(93 * cursorContextArray[0].relativeColumnIndex + 13, 26, 0.1f);
            displayCursor(298, 37 * relativeRowIndex + 109, 0.1f);
            break;
        }
        case 2: {
            if (updateStateMask & 2)
                displayCursor(93 * cursorContextArray[0].relativeColumnIndex + 13, 26, 0.0f);
            break;
        }
        case 3: {
            if (updateStateMask & 2)
                displayCursor(93 * cursorContextArray[0].relativeColumnIndex + 13, 26, 0.001f);
            baseKeyItemRow = cursorContextArray[3].baseRowIndex;
            relativeKeyItemRow = cursorContextArray[3].relativeRowIndex;
            relativeKeyItemColumn = cursorContextArray[3].relativeColumnIndex;
            displayCursor(293 * relativeKeyItemColumn + 5, 36 * relativeKeyItemRow + 129, 0.001f);
            break;
        }
        case 4: { //Sort type widget is needed here
            if (updateStateMask & 2)
                displayCursor(93 * cursorContextArray[0].relativeColumnIndex + 13, 26, 0.001f);
            displayCursor(*(dword_DD18C0 + 24) - 30, *(dword_DD18C0 + 26) + 26 * inventory_arrange_type + 17, 0.001f);
            for (int j = 0; j < 8; ++j) {            // Loop over arrange types
                fetchedDescription = gContext.game_strings.inventory_menu_texts.get_string(j + 3);
                displayTextAtLocation(*(dword_DD18C0 + 24) + 13, *(dword_DD18C0 + 26) + 26 * j + 13, fetchedDescription, COLOR_WHITE, 0.001f);
            }
            gameDrawBox((i16*)(&(menuWindowConfig)[3]), 0.001f);
            break;
        }
        case 5: {
            if (updateStateMask & 2)
                displayCursor(93 * cursorContextArray[0].relativeColumnIndex + 13, 26, 0.1f);
            auto baseSortRow = cursorContextArray[5].baseRowIndex;
            auto relativeSortRow = cursorContextArray[5].relativeRowIndex;
            if (ITEM_TO_SWAP_SELECTED) {
                if (updateStateMask & 2) {
                    i32 pixelOffsetToSelectedItem = 37 * (*TEMP_ABSOLUTE_CURSOR_INDEX) - 37 * baseSortRow + 9 * (*dword_DD1B54) - 9;
                    if (pixelOffsetToSelectedItem > -37 && pixelOffsetToSelectedItem < 1369
                        displayCursor(291, pixelOffsetToSelectedItem + 113, 0.0);
                }
            }
            displayCursor(298, 37 * relativeSortRow + 113, 0.001f);
            break;

        }
        default:
            break;
    }
}

/*This function displaces texts depending on current "view" in the menu*/
void displayInventoryViews(i32 updateStateMask) {
    u32 inventoryMenuState = *(INVENTORY_MENU_STATE);
    CursorContext* cursorContextArray = (CursorContext*)(INVENTORY_MENU_CURSOR_CONTEXTS); //might be better to call this 'current menu view'
    int mainViewContextIndex;

    if (inventoryMenuState == 5) {
        mainViewContextIndex = 5;              
    }
    else {
        mainViewContextIndex = 1; 
    }
    sub_6FA347();
}


void renderMainInventoryView(i32 mainViewContextIndex) {
    CursorContext* cursorContextArray = (CursorContext*)(INVENTORY_MENU_CURSOR_CONTEXTS);
    u32 relativeRowIndex = cursorContextArray[1].relativeRowIndex;

    drawScrollerParams* arguments = (drawScrollerParams*)dword_DD17F0;
    setSideScrollerArguments(arguments, 10, gContext.inventory->current_capacity(), cursorContextArray[mainViewContextIndex].baseRowIndex, 618, 102, 17, 372);
    renderMenuSideScroller(arguments, 0.2);
}


void renderKeyItemsView() {
    CursorContext* cursorContextArray = (CursorContext*)(INVENTORY_MENU_CURSOR_CONTEXTS);
    char* keyItemName;

    *dword_DD17F0 = 10;
    *GLOBAL_MENU_ROW_COUNT = 32; //Number of items in one column in key items menu
    *GLOBAL_MENU_ROW_BASE = cursorContextArray[2].baseRowIndex;
    *word_DD17F6 = 618; /*Still need to figure out what these values do*/
    *word_DD17F8 = 102;
    *word_DD17FA = 17;
    *word_DD17FC = 372;
    renderSideScroller((i32)dword_DD17F0, 0.1f);
}

void handleInventoryMenuInput(i32 updateStateMask, Menu* menuObject) {
    auto inventoryMenuState = menuObject->currentState;
    auto cursorArray = getStateCursor(menuObject, inventoryMenuState);
    auto menuWidget = menuObject->menuWidget;

    InventoryInputEvent event = { menuObject, inventoryMenuState };
    handleCursorPositionUpdate((u32*)cursorArray);
    auto dispatchContext = std::vector<SrEventContext>({ INVENTORY_MENU_CONTEXT });
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
