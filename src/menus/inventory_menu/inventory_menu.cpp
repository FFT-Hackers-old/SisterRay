#include "inventory_menu.h"
#include "../../impl.h"
#include "windows.h"
#include "../menu_utils.h"

using namespace InventoryWidgetNames;

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
    Menu* menu = gContext.menuWidgets.get_element(INVENTORY_MENU_NAME);
    runMenu(menu, updateStateMask);
    displayInventoryCursorStates(menu, menu->currentState, updateStateMask);
}

/*Might just want to include cursors in the draw callbacks*/
void displayInventoryCursorStates(Menu* menu, u16 menuState, u32 updateStateMask) {
    u32 inventory_arrange_type = *(INVENTORY_ARRANGE_TYPE);
    u16 item_ID;

    sub_6C98A6();
    auto viewChoiceCursor = getStateCursor(menu, 0);
    switch (menu->currentState) {
        case 0: {
            drawCursor(viewChoiceCursor, 0.1f);
            break;
        }
        case 1: {
            auto itemChoiceCursor = getStateCursor(menu, 1);
            drawFlashingCursor(viewChoiceCursor, updateStateMask, 0.1f);
            drawCursor(itemChoiceCursor, 0.1f);
            break;
        }
        case 2: {
            auto itemChoiceCursor = getStateCursor(menu, 1);
            drawFlashingCursor(viewChoiceCursor, updateStateMask, 0.1f);
            drawFlashingCursor(itemChoiceCursor, updateStateMask, 0.1f);

            if (!(*use_on_characters_enabled)) {
                auto characterChoiceCursor = getStateCursor(menu, 2);
                item_ID = gContext.inventory->get_resource(itemChoiceCursor->context.baseRowIndex + itemChoiceCursor->context.relativeRowIndex).item_id;;
                if (!(gContext.itemOnUseData.get_resource(item_ID).target_all))
                    drawCursor(characterChoiceCursor, 0.0f);
                else
                    displayCursor(0, 120 * (updateStateMask % 3) + 161, 0.0);
            }
            if (*use_on_characters_enabled)
                --(*use_on_characters_enabled);
            break;
        }
        case 3: {
            drawFlashingCursor(viewChoiceCursor, updateStateMask, 0.1f);
            auto keyItemChoiceCursor = getStateCursor(menu, 3);
            drawCursor(keyItemChoiceCursor, 0.001f);
            break;
        }
        case 4: {
            auto arrangeTypeChoice = getStateCursor(menu, 4);
            drawFlashingCursor(viewChoiceCursor, updateStateMask, 0.1f);
            drawCursor(arrangeTypeChoice, 0.001f);
            break;
        }
        case 5: {
            auto customSortChoice = getStateCursor(menu, 5);
            drawFlashingCursor(viewChoiceCursor, updateStateMask, 0.1f);

            if (*ITEM_TO_SWAP_SELECTED) {
                if (updateStateMask & 2) {
                    i32 pixelOffsetToSelectedItem = 37 * (*TEMP_ABSOLUTE_CURSOR_INDEX) - 37 * customSortChoice->context.baseRowIndex + 9 * (*dword_DD1B54) - 9;
                    if (pixelOffsetToSelectedItem > -37 && pixelOffsetToSelectedItem < 1369)
                        displayCursor(291, pixelOffsetToSelectedItem + 113, 0.0);
                }
            }
            drawCursor(customSortChoice, 0.001f);
            break;

        }
        default:
            break;
    }
}

/*This function displaces texts depending on current "view" in the menu*/
/*void displayInventoryViews(i32 updateStateMask) {
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
    *word_DD17F6 = 618;
    *word_DD17F8 = 102;
    *word_DD17FA = 17;
    *word_DD17FC = 372;
    renderSideScroller((i32)dword_DD17F0, 0.1f);
}*/

