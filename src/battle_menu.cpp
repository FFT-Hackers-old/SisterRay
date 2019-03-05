#include "battle_menu.h"
#include "impl.h"

// Refactor this by implementing the structure in question so the code is not ugly
SISTERRAY_API u32* initializeBattleItemMenuCursor() {
    u32* cursorContextPtr = (dword_DC20D8 + (448 * (*ACTIVE_MENU_OWNER_PARTY_INDEX)));
    srLogWrite("ptr to cursor row max: %p", &cursorContextPtr[0]);
    if (!(*dword_DC3C5C))
    {
        cursorContextPtr[0] = 0;
        cursorContextPtr[1] = 0;
        cursorContextPtr[4] = 0;
        cursorContextPtr[5] = 0;
    }

    /*Initialize Cursor Context for the battle menu*/
    cursorContextPtr[2] = 1;
    cursorContextPtr[3] = 3;
    cursorContextPtr[6] = 1;
    cursorContextPtr[7] = gContext.battle_inventory->slots_in_use; //max number of cursor updates "down"
    srLogWrite("ptr to cursor row max: %p", &cursorContextPtr[7]);
    cursorContextPtr[10] = 2;
    cursorContextPtr[11] = 0;
    cursorContextPtr[8] = 0;
    cursorContextPtr[9] = 0;
    cursorContextPtr[12] = 0;
    cursorContextPtr[13] = 1;

    return &(cursorContextPtr[0]);
}

SISTERRAY_API i32 renderBattleItemView() {
    char *fetchedName;
    u16* viewContexPtr;
    u16 numberOfVisibleItems;
    u16 baseCursorPosition;
    u16 itemID;
    u8 itemQuantity;
    u16 flatInventoryIndex;
    u8 textColor;
    u16 renderContextStruct[7];

    viewContexPtr = (u16*)(dword_DC20D8 + 448 * (*ACTIVE_MENU_OWNER_PARTY_INDEX)); //Making the temporary assumption that this is a dword ptr
    if (gContext.battle_inventory->slots_in_use > 3)
    {
        renderContextStruct[0] = (u16)3;           // items_visible
        renderContextStruct[1] = (u16)gContext.battle_inventory->slots_in_use;;// set the current items visible
        renderContextStruct[2] = (u16)*(viewContexPtr + 10); // base row byte
        renderContextStruct[3] = (u16)614;
        renderContextStruct[4] = (u16)354;
        renderContextStruct[5] = (u16)20;
        renderContextStruct[6] = (u16)100;
        renderSideScroller((i32)&(renderContextStruct[0]), 0.40099999f);
    }
    u16 unknown_local = 8 * *(viewContexPtr + 18);                        // This determines the number of items that are in view at a given time
    baseCursorPosition = *(viewContexPtr + 10);
    if (unknown_local)
        numberOfVisibleItems = 4;
    else
        numberOfVisibleItems = 3;

    for (int visible_row = 0; visible_row < numberOfVisibleItems; ++visible_row)// relative_index
    {
        flatInventoryIndex = visible_row + baseCursorPosition;
        if (gContext.battle_inventory->get_resource(flatInventoryIndex).item_id != 0xFFFF)// if there is no item in the inventory at this index
        {
            itemID = gContext.battle_inventory->get_resource(flatInventoryIndex).item_id;
            itemQuantity = gContext.battle_inventory->get_resource(flatInventoryIndex).quantity;

            if ((*COMMAND_TRIGGER_INDEX) != 3 && (*COMMAND_TRIGGER_INDEX) != 10)
                textColor = (isThrowable(itemID)) ? 7 : 0;
            else
                textColor = (isUsableInBattle(itemID)) ? 7 : 0;

            displayVisibleItemIcon(94, unknown_local + 32 * visible_row + 356, itemID, 0, 0.40099999);
            sub_6F5C0C(366, unknown_local + 32 * visible_row + 364, 0xD5u, textColor, 0.40099999);
            renderNumbers(378, unknown_local + 32 * visible_row + 364, itemQuantity, 2, textColor, 1053642719);

            //Now display the item names
            fetchedName = getNameFromItemID(itemID);
            displayTextAtLocation(130, unknown_local + 32 * visible_row + 360, fetchedName, textColor, 1053642719);
        }
    }
    return 0;
}

/*Return True if the character is usable by the character attempting to use it*/
bool isUsableInBattle(u16 itemID) {
    auto restrictionMask = get_restriction_mask(itemID);
    auto characterMask = gContext.item_on_use_data.get_resource(itemID).characterRestrictionMask;
    bool itemIsUsuable = (bool)(!(restrictionMask & 2));

    auto party_member_index = (*ACTIVE_MENU_OWNER_PARTY_INDEX);
    auto characterID = (activePartyStructArray)[party_member_index].characterID;
    bool characterCanUse = (bool)!(characterMask & (1 << characterID));

    return (itemIsUsuable && characterCanUse);
}

bool isThrowable(u16 itemID) {
    auto restrictionMask = get_restriction_mask(itemID);
    auto characterMask = gContext.item_on_use_data.get_resource(itemID).characterRestrictionMask;
    bool itemIsThrowable = (bool)(!(restrictionMask & 8)); // Use new throwable variable

    auto party_member_index = (*ACTIVE_MENU_OWNER_PARTY_INDEX);
    auto characterID = (activePartyStructArray)[party_member_index].characterID;
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
            update_cursor_position((u32*)viewContextPtr);

            if (!(*((u32*)viewContextPtr + 12))) {
                if (checkInputReceived(32)) {
                    *ACCEPTING_BATTLE_INPUT = 1;
                    flatInventoryIndex = *((u16*)viewContextPtr + 10) + *((u16*)viewContextPtr + 2) + *((u16 *)viewContextPtr);
                    itemID = gContext.battle_inventory->get_resource(flatInventoryIndex).item_id;
                    actionSucceeded = didItemUseSucceed(itemID);

                    if (actionSucceeded) {
                        playMenuSound(1);
                        targetData = gContext.battle_inventory->get_resource(flatInventoryIndex).targetFlags;

                        *GLOBAL_BATTLE_ITEM_USED = itemID;
                        *GLOBAL_USED_ITEM_TARGET_DATA = targetData;
                        *GLOBAL_USED_INVENTORY_INDEX = flatInventoryIndex;
                        *BATTLE_MENU_STATE = 0;
                        *PREVIOUS_BATTLE_MENU_STATE = 5;

                        handleActionCommandIssued();

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
    auto itemType = gContext.item_type_data.get_resource(itemID).item_type;
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
