#include "battle_item_menu.h"
#include "../../../impl.h"

using namespace BattleItemWidgetNames;

void battleItemUpdateHandler(i32 updateStateMask) {
    Menu* menuObject = gContext.menuWidgets.get_element("BATTLE_ITEM");

    sub_6C98A6();
    auto menuWidget = menuObject->menuWidget;
    setMenuState(menuObject, *BATTLE_ACTIVE_ACTOR_ID); //Here each party member is treated as a "menu state" for the view, which we set to always match the greater battle menu

    std::vector<std::string> names = { PARTY_1_SPELL_GRID, PARTY_2_SPELL_GRID, PARTY_3_SPELL_GRID };
    for (i32 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID) {
            enableWidget(getChild(getChild(menuWidget, BATTLE_ITEM_MENU_NAME), names[partyIdx]));
            break;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_ITEM_MENU_NAME), names[partyIdx]));
    }

    BattleSpellInputEvent event = { menuObject, menuObject->currentState };
    gContext.eventBus.dispatch(DRAW_BATTLE_ITEM, &event);
    drawWidget(menuWidget);

    drawCursor(getStateCursor(menuObject, event.menuState), 0.1f); /*Here we just draw the cursor for each state, as they are independent party states*/
}

void battleItemInputHandler() {
    if (*ACCEPTING_BATTLE_INPUT)
        return;

    if (*BATTLE_MENU_STATE != 5)
        return;
    Menu* menuObject = gContext.menuWidgets.get_element("BATTLE_ITEM");
    dispatchMenuInput(0, menuObject, BATTLE_ITEM_VIEW);
}


/*SISTERRAY_API u32* initializeBattleItemMenuCursor() {
    CursorContext* cursorContextPtr = (CursorContext*)(dword_DC20D8 + (448 * (*BATTLE_ACTIVE_ACTOR_ID)));
    if (!(*REMEMBER_CURSOR_POSITION))
    {
        cursorContextPtr->relativeRowIndex = 0;
        cursorContextPtr->relativeColumnIndex = 0;
        cursorContextPtr->baseRowIndex = 0;
        cursorContextPtr->baseColumnIndex = 0;
    }

    cursorContextPtr->viewColumnBound = 1;
    cursorContextPtr->viewRowBound = 3;
    cursorContextPtr->maxColumnBound = 1;
    cursorContextPtr->maxRowBound = gContext.battleInventory->slots_in_use; //max number of cursor updates "down"
    cursorContextPtr->ninth_dword = 0;
    cursorContextPtr->tenth_dword = 0;
    cursorContextPtr->eleventh_dword = 2;
    cursorContextPtr->twelth_dword = 0;
    cursorContextPtr->thirteenth_dword = 0;
    cursorContextPtr->fourteenh_dword = 1;

    return (u32*)&(cursorContextPtr[0]);
}

SISTERRAY_API i32 renderBattleItemView() {
    const char *fetchedName;
    CursorContext* cursorContextArray;
    u16 numberOfVisibleItems;
    u16 baseCursorPosition;
    u16 itemID;
    u8 itemQuantity;
    u16 flatInventoryIndex;
    u8 textColor;
    u16 renderContextStruct[7];

    cursorContextArray = (CursorContext*)(dword_DC20D8 + 448 * (*BATTLE_ACTIVE_ACTOR_ID)); //In battle each actor has their own array of menu context structures
    if (gContext.battleInventory->slots_in_use > 3)
    {
        renderContextStruct[0] = (u16)3;           // items_visible
        renderContextStruct[1] = (u16)gContext.battleInventory->slots_in_use;;
        renderContextStruct[2] = (u16)(cursorContextArray[0].baseRowIndex); // base row byte
        renderContextStruct[3] = (u16)614;
        renderContextStruct[4] = (u16)354;
        renderContextStruct[5] = (u16)20;
        renderContextStruct[6] = (u16)100;
        renderSideScroller((i32)&(renderContextStruct[0]), 0.40099999f);
    }
    u16 unknownLocal = 8 * cursorContextArray[0].ninth_dword;            // This determines the number of items that are in view at a given time
    baseCursorPosition = cursorContextArray[0].baseRowIndex;

    if (unknownLocal)
        numberOfVisibleItems = 4;
    else
        numberOfVisibleItems = 3;

    for (int visibleRow = 0; visibleRow < numberOfVisibleItems; ++visibleRow)// relative_index
    {
        flatInventoryIndex = visibleRow + baseCursorPosition;
        if (gContext.battleInventory->get_resource(flatInventoryIndex).item_id != 0xFFFF)// if there is no item in the inventory at this index
        {
            itemID = gContext.battleInventory->get_resource(flatInventoryIndex).item_id;
            itemQuantity = gContext.battleInventory->get_resource(flatInventoryIndex).quantity;

            if ((*COMMAND_TRIGGER_INDEX) != 3 && (*COMMAND_TRIGGER_INDEX) != 10)
                textColor = (isThrowable(itemID)) ? COLOR_WHITE : COLOR_GRAY;
            else
                textColor = (isUsableInBattle(itemID)) ? COLOR_WHITE : COLOR_GRAY;

            displayVisibleItemIcon(94, unknownLocal + 32 * visibleRow + 356, itemID, 0, 0.40099999);
            gameDrawAsset(366, unknownLocal + 32 * visibleRow + 364, 0xD5u, textColor, 0.40099999);
            gameDrawNumbers(378, unknownLocal + 32 * visibleRow + 364, itemQuantity, 2, textColor, 1053642719);

            //Now display the item names
            fetchedName = getNameFromItemID(itemID);
            displayTextAtLocation(130, unknownLocal + 32 * visibleRow + 360, fetchedName, (color)textColor, 1053642719);
        }
    }
    return 0;
}*/
