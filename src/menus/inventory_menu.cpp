#include "inventory_menu.h"
#include "../impl.h"
#include "windows.h"
#include "menu_utils.h"
#include "../inventories/inventory_utils.h"

#define SIZE_OF_CURSOR_STRUCT  (i32)0xE // this is the size in i32, for ptr arithmetic

SISTERRAY_API i32 onEnterInventory() {
    i32 ret;

    CursorContext* cursorContextArray = (CursorContext*)INVENTORY_MENU_CURSOR_CONTEXTS;
    *INVENTORY_MENU_STATE = 1;
    CursorContext* inventoryOptionCursorContext = &(cursorContextArray[0]);
    CursorContext* inventoryViewCursorContext = &(cursorContextArray[1]);
    CursorContext* characterViewCursorContext = &(cursorContextArray[2]);
    setContextCursorData(inventoryOptionCursorContext, 0, 0, 3, 1, 0, 0, 3, 1, 0, 0, 1, 0, 0, 0);
    setContextCursorData(inventoryViewCursorContext, 0, 0, 1, 10, 0, 0, 1, gContext.inventory->current_capacity(), 0, 0, 0, 0, 0, 1);
    setContextCursorData(characterViewCursorContext, 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 0, 1, 0, 0); //initialize cursor for character pane

    ret = sub_714FA3();
    if (*dword_DC130C == 1)
        *dword_DD18C0 = (u32)dword_921C98;
    else
        *dword_DD18C0 = (u32)&((menuWindowConfig)[0]);
    return ret;
}

SISTERRAY_API void inventoryMenuUpdateHandler(i32 updateStateMask)
{
    displayActiveCursorStates(updateStateMask);
    displayInventoryViews(updateStateMask);
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
    char* fetchedDescription;

    u16 item_ID;

    sub_6C98A6();
    if (inventoryMenuState == 2)
    {
        if (!(*use_on_characters_enabled)) {
            item_ID = gContext.inventory->get_resource(baseRowIndex + relativeRowIndex).item_id;;
            if (!(gContext.item_on_use_data.get_resource(item_ID).target_all))
                displayCursor(0, 120 * partyMemberIndex + 161, 0.0f); //if the cursor isn't targeting all
            else
                displayCursor(0, 120 * (updateStateMask % 3) + 161, 0.0); // if the cursor is targeting all
        }
        if (updateStateMask & 2) //This and causes the cursor to flash on repeated updates
            displayCursor(298, 37 * relativeRowIndex + 109, 0.1f);
        if (*use_on_characters_enabled)
            --(*use_on_characters_enabled);
    }
    switch (inventoryMenuState)
    {
    case 0:                                   // Nothing Selected, Default State
        displayCursor(93 * cursorContextArray[0].relativeColumnIndex + 13, 26, 0.1f); //display cursor at the selected view
        break;
    case 1:                                   // Use Selected - Selecting Item
        if (updateStateMask & 2)
            displayCursor(93 * cursorContextArray[0].relativeColumnIndex + 13, 26, 0.1f);
        displayCursor(298, 37 * relativeRowIndex + 109, 0.1f);
        if (gContext.inventory->get_resource(baseRowIndex + relativeRowIndex).item_id != 0xFFFF)
        {
            fetchedDescription = getDescriptionFromID(gContext.inventory->get_resource(baseRowIndex + relativeRowIndex).item_id);
            displayTextAtLocation(27, 64, fetchedDescription, COLOR_WHITE, 0.1f);
        }
        break;
    case 2:                                   // Use Selected - Targeting Party
        if (updateStateMask & 2)
            displayCursor(93 * cursorContextArray[0].relativeColumnIndex + 13, 26, 0.0f);
        if (gContext.inventory->get_resource(baseRowIndex + relativeRowIndex).item_id != 0xFFFF)
        {
            fetchedDescription = getDescriptionFromID(gContext.inventory->get_resource(baseRowIndex + relativeRowIndex).item_id);
            displayTextAtLocation(27, 64, fetchedDescription, COLOR_WHITE, 0.1f);
        }
        break;
    case 3:                                   // Browsing Key Items
        if (updateStateMask & 2)
            displayCursor(93 * cursorContextArray[0].relativeColumnIndex + 13, 26, 0.001f);

        baseKeyItemRow = cursorContextArray[3].baseRowIndex;
        relativeKeyItemRow = cursorContextArray[3].relativeRowIndex;
        relativeKeyItemColumn = cursorContextArray[3].relativeColumnIndex;
        displayCursor(293 * relativeKeyItemColumn + 5, 36 * relativeKeyItemRow + 129, 0.001f);

        flatKeyItemInventoryIndex = 2 * (baseKeyItemRow)+2 * (relativeKeyItemRow)+(relativeKeyItemColumn);
        if ((KEY_ITEMS_INVENTORY_PTR)[flatKeyItemInventoryIndex] != 0xFFFF) //If there is a key item at cursor matrix position
        {
            i32 keyItemID = (KEY_ITEMS_INVENTORY_PTR)[flatKeyItemInventoryIndex];
            fetchedDescription = (char*)load_kernel_object_text(0xEu, keyItemID, 0); //The returned arg here is the item description
            displayTextAtLocation(27, 64, fetchedDescription, COLOR_WHITE, 0.1f);
        }
        break;
    case 4:                                   //Selecting an Arrange Method
        if (updateStateMask & 2)
            displayCursor(93 * cursorContextArray[0].relativeColumnIndex + 13, 26, 0.001f);
        displayCursor(*(dword_DD18C0 + 24) - 30, *(dword_DD18C0 + 26) + 26 * inventory_arrange_type + 17, 0.001f);
        for (int j = 0; j < 8; ++j) {            // Loop over arrange types
            fetchedDescription = gContext.game_strings.inventory_menu_texts.get_string(j + 3); //read the arrange type text from an in memory 12 char byte array skipping "use, arrange, and key item"
            displayTextAtLocation(*(dword_DD18C0 + 24) + 13, *(dword_DD18C0 + 26) + 26 * j + 13, fetchedDescription, COLOR_WHITE, 0.001f);
        }
        gameDrawBox((i16*)(&(menuWindowConfig)[3]), 0.001f);
        break;
    case 5:                                   // Inside Custom Sort
        if (updateStateMask & 2)
            displayCursor(93 * cursorContextArray[0].relativeColumnIndex + 13, 26, 0.1f);

        baseSortRow = cursorContextArray[5].baseRowIndex;
        relativeSortRow = cursorContextArray[5].relativeRowIndex;

        if (gContext.inventory->get_resource(baseSortRow + relativeSortRow).item_id != 0xFFFF)
        {
            fetchedDescription = getDescriptionFromID(gContext.inventory->get_resource(baseRowIndex + relativeRowIndex).item_id);
            displayTextAtLocation(27, 64, fetchedDescription, COLOR_WHITE, .1f);
        }
        break;
    default:
        break;
    }
}

/*This function displaces texts depending on current "view" in the menu*/
void displayInventoryViews(i32 updateStateMask) {
    u32 inventoryMenuState = *(INVENTORY_MENU_STATE);
    CursorContext* cursorContextArray = (CursorContext*)(INVENTORY_MENU_CURSOR_CONTEXTS); //might be better to call this 'current menu view'
    int mainViewContextIndex;

    if (cursorContextArray[0].relativeColumnIndex != 2)     // Something with party members unless on cursor positioned on key items
    {
        renderCharacterPortraits();
    }

    for (int menu_text_index = 0; menu_text_index < 3; ++menu_text_index) //display the "Use, Arrange, and Key Item fields
        displayTextAtLocation(93 * menu_text_index + 57, 17, gContext.game_strings.inventory_menu_texts.get_string(menu_text_index), COLOR_WHITE, .1f);
    sub_6FA12F(0, 102, 640, 372);

    if (cursorContextArray[0].relativeColumnIndex == 2)     // If cursor positioned on key items
    {
        renderKeyItemsView();
    }
    else
    {
        if (inventoryMenuState == 5) {

            auto baseSortRow = cursorContextArray[5].baseRowIndex;
            auto relativeSortRow = cursorContextArray[5].relativeRowIndex;
            if (ITEM_TO_SWAP_SELECTED) {
                if (updateStateMask & 2) {
                    i32 pixelOffsetToSelectedItem = 37 * (*TEMP_ABSOLUTE_CURSOR_INDEX) - 37 * baseSortRow + 9 * (*dword_DD1B54) - 9;
                    if (pixelOffsetToSelectedItem > -37 && pixelOffsetToSelectedItem < 1369) // display the flashing cursor if it's visible on the menu
                        displayCursor(291, pixelOffsetToSelectedItem + 113, 0.0);
                }
            }
            displayCursor(298, 37 * relativeSortRow + 113, 0.001f);
            mainViewContextIndex = 5;                 // Set the local to 5 if we're in custom sort
        }
        else
        {
            mainViewContextIndex = 1;                 //Set it to 1 otherwise
        }
        /*Render the main "item list" view regardless of whether custom arrange is active or not*/
        renderMainInventoryView(mainViewContextIndex);
    }
    sub_6FA347();
    gameDrawBox((i16*)(&(menuWindowConfig)[0]), 0.111f);
    gameDrawBox(((i16*)&(menuWindowConfig)[1]), 0.2f);
    gameDrawBox(((i16*)&(menuWindowConfig)[2]), 0.30000000f);
}


void renderMainInventoryView(i32 mainViewContextIndex) {
    CursorContext* cursorContextArray = (CursorContext*)(INVENTORY_MENU_CURSOR_CONTEXTS);
    u32 relativeRowIndex = cursorContextArray[1].relativeRowIndex;
    char* kernelObjectName;
    u16 itemID;
    u8 itemQuantity;
    i32 visibleItemInventoryIndex;
    i32 baseColumnIndex;
    i32 baseRowIndex;
    i32 textColor;

    drawScrollerParams* arguments = (drawScrollerParams*)dword_DD17F0;
    setSideScrollerArguments(arguments, 10, gContext.inventory->current_capacity(), cursorContextArray[mainViewContextIndex].baseRowIndex, 618, 102, 17, 372);
    renderMenuSideScroller(arguments, 0.2);
    int displayed_row_count = ((dword_DD1A48)[14 * mainViewContextIndex] != 0) + 10;

    baseRowIndex = cursorContextArray[mainViewContextIndex].baseRowIndex;
    baseColumnIndex = cursorContextArray[mainViewContextIndex].baseColumnIndex;
    for (i32 visibleItem = 0; visibleItem < displayed_row_count; ++visibleItem) {
        i32 baseRowIndex = cursorContextArray[mainViewContextIndex].baseRowIndex;
        visibleItemInventoryIndex = baseRowIndex + visibleItem;
        if (gContext.inventory->get_resource(visibleItemInventoryIndex).item_id != 0xFFFF) {
            itemID = gContext.inventory->get_resource(visibleItemInventoryIndex).item_id;
            itemQuantity = gContext.inventory->get_resource(visibleItemInventoryIndex).quantity;
            textColor = usableInInventoryMenu(itemID) ? 0 : 7;
            kernelObjectName = getNameFromItemID(itemID);

            displayTextAtLocation(373, 37 * visibleItem + 9 * baseColumnIndex + 109, kernelObjectName, (color)textColor, .1f);
            displayVisibleItemIcon(343, 37 * visibleItem + 9 * baseColumnIndex + 105, itemID, 0, .1f);
            gameDrawAsset(548, 37 * visibleItem + 9 * baseColumnIndex + 114, 213, (u8)textColor, .1f);
            gameDrawNumbers(550, 37 * visibleItem + 9 * baseColumnIndex + 112, itemQuantity, 3, (u8)textColor, .1f);
        }
    }
}


void renderCharacterPortraits() {
    u16 characterMenuBoxLocal[4] = {0, (u16)96, (u16)300, u16(384)}; //

    for (int currentPartyMember = 0; currentPartyMember < 3; ++currentPartyMember) { //loop over and render character portraits, probably
        if ((CURRENT_PARTY_MEMBER_ARRAY)[currentPartyMember] != 0xFF) {         //if there is a party member in that slot
            renderHPAndStatus(133, 120 * currentPartyMember + 126, currentPartyMember, 1036831949); //possibly display picture?
            displayPortrait(37, 120 * currentPartyMember + 116, currentPartyMember, 1036831949);
        }
    }

    // initialize_menu_window_struct((u16*)&unk_local_struct, (u16)0, (u16)96, (u16)300, (u16)384); //set some values in a struct/array used in the next call
    gameDrawBox((i16*)(&characterMenuBoxLocal), 0.1f); //this does a bunch of shit with the above struct
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

    for (int visibleItemItemRow = 0; visibleItemItemRow < 12; ++visibleItemItemRow) {
        int flat_key_item_index_base = 2 * visibleItemItemRow + 2 * cursorContextArray[3].baseRowIndex;
        for (int keyItemColumn = 0; keyItemColumn < 2; ++keyItemColumn) {
            if ((KEY_ITEMS_INVENTORY_PTR)[keyItemColumn + flat_key_item_index_base] != 0xFF) {
                keyItemName = (char *)load_kernel_object_text(0xEu, (KEY_ITEMS_INVENTORY_PTR)[(u8)(keyItemColumn + flat_key_item_index_base)], 8); //fetch the name of the key item
                displayTextAtLocation(293 * keyItemColumn + 53, 36 * visibleItemItemRow + 9 * (*dword_DD1AE4) + 124, keyItemName, COLOR_WHITE, .1f); //display the text
            }
        }
    }
}


/*WIP here, need a more generic way to introduce "usable" items through a function registry or something similar*/
void handleInventoryInput(i32 a1) {
    CursorContext* cursorContextArray = (CursorContext*)INVENTORY_MENU_CURSOR_CONTEXTS;
    u32* inventoryMenuState = INVENTORY_MENU_STATE;
    u32 partyMemberIndex = *(INVENTORY_CURRENT_PARTY_INDEX);
    u32 baseRowIndex = cursorContextArray[1].baseRowIndex;
    u32 relativeRowIndex = cursorContextArray[1].relativeRowIndex;
    i32 baseSortRow;
    i32 relativeSortRow;
    u16 itemID;

    handleCursorPositionUpdate((u32*)&(cursorContextArray[*inventoryMenuState]));
    switch ((*inventoryMenuState))
    {
    case 0:
        if (checkInputReceived2(32)) {               // handling inputs?
            playMenuSound(1);
            if (cursorContextArray[0].relativeColumnIndex) {
                if (cursorContextArray[0].relativeColumnIndex == 1) {
                    setContextCursorData((CursorContext*)(&(cursorContextArray[4])), 0, 0, 1, 8, 0, 0, 1, 8, 0, 0, 0, 1, 0, 0); //Sets cursor data for the arrange menu
                    *inventoryMenuState = 4;         // Arrange Menu
                }
                else if (cursorContextArray[0].relativeColumnIndex == 2) {
                    setContextCursorData((CursorContext*)(&(cursorContextArray[3])), 0, 0, 2, 10, 0, 0, 2, 32, 0, 0, 2, 0, 0, 1); //sets cursor data for the key items menu
                    *inventoryMenuState = 3;         // Key Items Menu
                }
            }
            else {
                *inventoryMenuState = 1;             // use looking at item list
            }
        }
        else if (checkInputReceived(64)) {
            playMenuSound(4);
            set_some_transition_data(5, 0); // These functions handle transitioning to different parts of the menu
            set_more_transition_data(0);
        }
        break;
    case 1:
        if (!(*dword_DD1A80)) {
            if (checkInputReceived2(32)) {
                if (gContext.inventory->get_resource(baseRowIndex + relativeRowIndex).item_id == 0xFFFF) {
                    playMenuSound(3);
                }
                else {
                    itemID = gContext.inventory->get_resource(baseRowIndex + relativeRowIndex).item_id;
                    if (usableInInventoryMenu(itemID)) {
                        playMenuSound(3);
                    }
                    else if (!(gContext.item_on_use_data.get_resource(itemID).requires_target)) {  
                        gContext.untargeted_handlers.get_handler(itemID)();

                    }
                    else { //If the item is usable, but requires a target, then jump to state 2
                        playMenuSound(1);
                        *use_on_characters_enabled = 0; //Setting this to 0 enables items to be used on characters in menu state 2
                        *inventoryMenuState = 2;
                    }
                }
            }
            else if (checkInputReceived2(64)) {
                playMenuSound(4);
                *inventoryMenuState = 0;
            }
        }
        break;
    case 2:
        if (*use_on_characters_enabled)
            return;
        if (checkInputReceived2(32)) {  //if "ok" input was received
            u16 inventory_index = baseRowIndex + relativeRowIndex;
            itemID = gContext.inventory->get_resource(inventory_index).item_id;
            u8 character_ID = (CURRENT_PARTY_MEMBER_ARRAY)[partyMemberIndex];
            if (character_ID == 0xFF && !(gContext.item_on_use_data.get_resource(itemID).target_all)) { // Can't use item on empty party member unless it is megalixer or tent
                playMenuSound(3);
                return;
            }
            /*Handle the on use effects for every item in the game, this needs to be made more generic*/
            handleUsableItemEffects(itemID, inventory_index);
        }
        else if (checkInputReceived(64)) {
            playMenuSound(4);
            *inventoryMenuState = 1;
        }
        break;
    case 3:
        if (checkInputReceived2(64))                // Exit Key Items
        {
            playMenuSound(4);
            *inventoryMenuState = 0;
        }
        break;
    case 4:
        if (checkInputReceived2(32))                // If OK received during arrange menu
        {
            playMenuSound(1);
            if (*INVENTORY_ARRANGE_TYPE)
            {
                sort_inventory(*INVENTORY_ARRANGE_TYPE); //Arranging the inventory, this method will have to be rewritten
                *inventoryMenuState = 0;
            }
            else
            {
                setContextCursorData((CursorContext*)&(cursorContextArray[5]), 0, 0, 1, 10, 0, 0, 1, gContext.inventory->current_capacity(), 0, 0, 0, 0, 0, 1);// Custom Sort Cursor Data Copy?
                *ITEM_TO_SWAP_SELECTED = 0; //Clear the globals that are used by the custom sort routine for swapping items
                *TEMP_COLUMN_INDEX = 0;
                *TEMP_ABSOLUTE_CURSOR_INDEX = 0;
                *inventoryMenuState = 5; //Move to custom sort state
            }
        }
        else if (checkInputReceived2(64))           // Arrange Cancel Handler
        {
            playMenuSound(4);
            *inventoryMenuState = 0;
        }
        break;
    case 5:                                   // Custom Arrange Handler
        if (checkInputReceived2(32))
        {

            baseSortRow = cursorContextArray[5].baseRowIndex;
            relativeSortRow = cursorContextArray[5].relativeRowIndex;
            if (*ITEM_TO_SWAP_SELECTED) // If this is already set when input is received, then switch the items. It's really a bool value
            {
                if (*ITEM_TO_SWAP_SELECTED == 1)
                {
                    playMenuSound(1);
                    /*This code swaps two items in the inventory*/
                    InventoryEntry temp_entry = gContext.inventory->get_resource(*TEMP_ABSOLUTE_CURSOR_INDEX); //copy cursor start
                    gContext.inventory->get_resource(*TEMP_ABSOLUTE_CURSOR_INDEX) = gContext.inventory->get_resource(baseSortRow + relativeSortRow); 
                    gContext.inventory->get_resource(baseSortRow + relativeSortRow) = temp_entry;
                    *ITEM_TO_SWAP_SELECTED = 0;
                }
            }
            else
            {
                playMenuSound(1);
                *TEMP_COLUMN_INDEX = cursorContextArray[5].relativeColumnIndex; //copy first dword of struct set in previous state here, seems to always be 0
                *TEMP_ABSOLUTE_CURSOR_INDEX = baseSortRow + relativeSortRow; //custom sort base row and relative offsets copied when you select an item to swap
                *ITEM_TO_SWAP_SELECTED = 1;
            }
        }
        else if (checkInputReceived2(64))
        {
            playMenuSound(4);
            *inventoryMenuState = 0;
        }
        break;
    default:
        return;
    }
}

/*This routine handles executing menu usable item routines*/
void handleUsableItemEffects(u16 item_ID, u16 inventory_index) {
    u32 partyMemberIndex = *(INVENTORY_CURRENT_PARTY_INDEX);
    auto itemWasUsed = false;

    /*Call the appropriate function handler for using items on a character/the party*/
    itemWasUsed = gContext.on_use_handlers.get_handler(item_ID)((u16)partyMemberIndex, item_ID, inventory_index);
    if (itemWasUsed) {
        gContext.inventory->decrementInventoryEntry(inventory_index, 1);
        if (gContext.inventory->get_resource(inventory_index).item_id == 0xFFFF)// If the Inventory Entry is -1, i.e it has been used up
            *INVENTORY_MENU_STATE = 1;
    }
}
