#include "battle_menu.h"
#include "../impl.h"
#include "../inventories/inventory_utils.h"

// Refactor this by implementing the structure in question so the code is not ugly
SISTERRAY_API u32* initializeBattleItemMenuCursor() {
    CursorContext* cursorContextPtr = (CursorContext*)(dword_DC20D8 + (448 * (*ACTIVE_MENU_OWNER_PARTY_INDEX)));
    srLogWrite("ptr to cursor row max: %p", &cursorContextPtr[0]);
    if (!(*REMEMBER_CURSOR_POSITION)) 
    {
        cursorContextPtr->relativeRowIndex = 0;
        cursorContextPtr->relativeColumnIndex = 0;
        cursorContextPtr->baseRowIndex = 0;
        cursorContextPtr->baseColumnIndex = 0;
    }

    /*Initialize Cursor Context for the battle menu*/
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

    cursorContextArray = (CursorContext*)(dword_DC20D8 + 448 * (*ACTIVE_MENU_OWNER_PARTY_INDEX)); //In battle each actor has their own array of menu context structures
    if (gContext.battleInventory->slots_in_use > 3)
    {
        renderContextStruct[0] = (u16)3;           // items_visible
        renderContextStruct[1] = (u16)gContext.battleInventory->slots_in_use;;// set the current items visible
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
}

/*Return True if the character is usable by the character attempting to use it*/
bool isUsableInBattle(u16 itemID) {
    auto restrictionMask = get_restriction_mask(itemID);
    auto characterMask = gContext.itemOnUseData.get_resource(itemID).characterRestrictionMask;
    bool itemIsUsuable = (bool)(!(restrictionMask & 2));

    auto party_member_index = (*ACTIVE_MENU_OWNER_PARTY_INDEX);
    auto characterID = (PARTY_STRUCT_ARRAY)[party_member_index].characterID;
    bool characterCanUse = (bool)!(characterMask & (1 << characterID));

    return (itemIsUsuable && characterCanUse);
}

bool isThrowable(u16 itemID) {
    auto restrictionMask = get_restriction_mask(itemID);
    auto characterMask = gContext.itemOnUseData.get_resource(itemID).characterRestrictionMask;
    bool itemIsThrowable = (bool)(!(restrictionMask & 8)); // Use new throwable variable

    auto party_member_index = (*ACTIVE_MENU_OWNER_PARTY_INDEX);
    auto characterID = (PARTY_STRUCT_ARRAY)[party_member_index].characterID;
    bool characterCanUse = bool(characterMask & (1 << characterID));

    return (itemIsThrowable && characterCanUse);
}

SISTERRAY_API void battleItemMenuInputHandler() {
    u8* viewContextPtr; 
    u16 flatInventoryIndex;
    u16 itemID;
    u8 targetData;
    bool actionSucceeded;

    viewContextPtr = (u8*)(dword_DC20D8 + 448 * (*ACTIVE_MENU_OWNER_PARTY_INDEX));
    if (*BATTLE_MENU_STATE == 5) {
        if (*byte_9AC111) {
            *byte_9AC111 = 0;
            *ACCEPTING_BATTLE_INPUT = 1;
        }

        if (!(*ACCEPTING_BATTLE_INPUT)) {
            handleCursorPositionUpdate((u32*)viewContextPtr);

            if (!(*((u32*)viewContextPtr + 12))) {
                if (checkInputReceived(32)) {
                    *ACCEPTING_BATTLE_INPUT = 1;
                    flatInventoryIndex = *((u16*)viewContextPtr + 10) + *((u16*)viewContextPtr + 2) + *((u16 *)viewContextPtr);
                    itemID = gContext.battleInventory->get_resource(flatInventoryIndex).item_id;
                    actionSucceeded = didItemUseSucceed(itemID);

                    if (actionSucceeded) {
                        playMenuSound(1);
                        targetData = gContext.battleInventory->get_resource(flatInventoryIndex).targetFlags;

                        *GLOBAL_ACTION_USED = itemID;
                        *GLOBAL_USED_ACTION_TARGET_DATA = targetData;
                        *GLOBAL_USED_MENU_INDEX = flatInventoryIndex;
                        *BATTLE_MENU_STATE = 0;
                        *PREVIOUS_BATTLE_MENU_STATE = 5;

                        setCursorTargetingData();

                        *GLOBAL_USED_ITEM_RESTORE = getRestoreTypeGlobal(itemID); //Display characters HP's if the item can heal
                        if (*GLOBAL_USED_ITEM_RESTORE != -1)
                            initHandlerCursorState(-1, -1, 21);
                    }
                    else {
                        playMenuSound(3);
                    }
                }
                else if (checkInputReceived(64)) {
                    playMenuSound(4);
                    *ACCEPTING_BATTLE_INPUT = 1;
                    *BATTLE_MENU_STATE = 1;
                    setHandlerState(5, 3);
                }
            }
        }
    }
}

bool didItemUseSucceed(u16 itemID) {
    bool isItemUsable;
    bool emptyItem = (itemID == 0xFFFF);

    if (emptyItem)
        return false;

    if (*COMMAND_TRIGGER_INDEX != 3 && *COMMAND_TRIGGER_INDEX != 10)
        isItemUsable = isThrowable(itemID);
    else
        isItemUsable = isUsableInBattle(itemID);

    return isItemUsable;
}

//Set the restore type global used by executing healing actions
u16 getRestoreTypeGlobal(i16 itemID) {
    auto itemType = gContext.itemTypeData.get_resource(itemID).item_type;
    if(itemType != 0) {
        return 0xFFFF;
    }
    return gContext.items.get_resource(itemID).resource_conditions;
}

// states should always boe 0 (inactive) 1 (prepare on next update) 2 (active) or 3 (executing)
i32 setHandlerState(u16 handlerIndex, i8 state) {
    (HANDLER_STATE_ARRAY)[handlerIndex] = state;
    return handlerIndex;
}
