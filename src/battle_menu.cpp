#include "battle_menu.h"
#include "impl.h"

// Refactor this by implementing the structure in question so the code is not ugly
SISTERRAY_API u32* initializeBattleItemMenuCursor() {
    u32* cursorContextPtr = (dword_DC20D8 + 448 * (*ACTIVE_MENU_OWNER_PARTY_INDEX));
    srLogWrite("ptr to cursor context: %p", cursorContextPtr);
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
    cursorContextPtr[10] = 2;
    cursorContextPtr[11] = 0;
    cursorContextPtr[8] = 0;
    cursorContextPtr[9] = 0;
    cursorContextPtr[12] = 0;
    cursorContextPtr[13] = 1;

    return cursorContextPtr;
}

// Rewrite this handler 
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

    viewContexPtr = (u16*)(dword_DC20D8 + 896 * (*ACTIVE_MENU_OWNER_PARTY_INDEX)); //Making the temporary assumption that this is a dword ptr
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

            if ((*commandTriggerIndex) != 3 && (*commandTriggerIndex) != 10)
                textColor = (isThrowable(itemID) != 0) ? 0 : 7;
            else
                textColor = (isUsableInBattle(itemID) != 0) ? 0 : 7;

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
    auto restrictionMask = gContext.battle_inventory->get_resource(itemID).restrictionMask;
    auto characterMask = gContext.item_on_use_data.get_resource(itemID).characterRestrictionMask;
    bool itemIsUsuable = (bool)(restrictionMask & 2);

    auto party_member_index = (*ACTIVE_MENU_OWNER_PARTY_INDEX);
    auto characterID = (activePartyStructArray)[party_member_index].characterID;
    bool characterCanUse = bool(characterMask & (1 << characterID));

    return (itemIsUsuable && characterCanUse);
}

bool isThrowable(u16 itemID) {
    auto restrictionMask = gContext.battle_inventory->get_resource(itemID).restrictionMask;
    auto characterMask = gContext.item_on_use_data.get_resource(itemID).characterRestrictionMask;
    bool itemIsThrowable = (bool)(restrictionMask & 8); // Use new throwable variable

    auto party_member_index = (*ACTIVE_MENU_OWNER_PARTY_INDEX);
    auto characterID = (activePartyStructArray)[party_member_index].characterID;
    bool characterCanUse = bool(characterMask & (1 << characterID));

    return (itemIsThrowable && characterCanUse);
}
