#include "inventory_input_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../menu_utils.h"
#include "../../inventories/inventory_utils.h"

//INVENTORY_OK_HANDLERS
void chooseViewHandler(const MenuInputEvent* event) {
    if (event->menuState != 0)
        return;

    auto viewChoice = getStateCursor(event->menu, 0)->context;

    playMenuSound(1);
    switch (viewChoice.relativeColumnIndex) {
        case 0: {
            setMenuState(event->menu, 1);
            break;
        }
        case 1: {
            setMenuState(event->menu, 4);
            break;
        }
                
        case 2: {
            setMenuState(event->menu, 3);
            break;
        }
        default: {
            
        }
    }
}

void selectItemHandler(const MenuInputEvent* event) {
    if (event->menuState != 1)
        return;

    auto itemChoice = getStateCursor(event->menu, 1)->context;

    if (gContext.inventory->getResource(itemChoice.baseRowIndex + itemChoice.relativeRowIndex).materiaID == 0xFFFF) {
        playMenuSound(3);
    }
    else {
        auto itemID = gContext.inventory->getResource(itemChoice.baseRowIndex + itemChoice.relativeRowIndex).materiaID;
        if (usableInInventoryMenu(itemID)) {
            playMenuSound(3);
        }
        else if (!(gContext.items.getResource(itemID).auxData.requires_target)) {
            gContext.untargeted_handlers.get_handler(itemID)();
            
        }
        else {
            playMenuSound(1);
            *use_on_characters_enabled = 0;
            setMenuState(event->menu, 2);
        }
    }
}

void executeSwapHandler(const MenuInputEvent* event) {
    if (event->menuState != 5)
        return;

    auto itemChoice = getStateCursor(event->menu, 5)->context;
    if (*ITEM_TO_SWAP_SELECTED) {
        if (*ITEM_TO_SWAP_SELECTED == 1)
        {
            playMenuSound(1);
            auto temp_entry = gContext.inventory->getResource(*TEMP_ABSOLUTE_CURSOR_INDEX);
            gContext.inventory->updateResource(*TEMP_ABSOLUTE_CURSOR_INDEX, gContext.inventory->getResource(itemChoice.baseRowIndex + itemChoice.relativeRowIndex));
            gContext.inventory->updateResource(itemChoice.baseRowIndex + itemChoice.relativeRowIndex, temp_entry);
            *ITEM_TO_SWAP_SELECTED = 0;
        }
    }
    else {
        playMenuSound(1);
        *TEMP_COLUMN_INDEX = itemChoice.relativeColumnIndex; 
        *TEMP_ABSOLUTE_CURSOR_INDEX = itemChoice.baseRowIndex + itemChoice.relativeRowIndex;
        *ITEM_TO_SWAP_SELECTED = 1;
    }
}

void useTargetedItemHandler(const MenuInputEvent* event) {
    if (event->menuState != 2)
        return;

    auto itemChoice = getStateCursor(event->menu, 1)->context;
    u16 inventory_index = itemChoice.baseRowIndex + itemChoice.relativeRowIndex;
    auto itemID = gContext.inventory->getResource(inventory_index).materiaID;
    u32 partyMemberIndex = getStateCursor(event->menu, 2)->context.relativeRowIndex;

    u8 character_ID = (CURRENT_PARTY_MEMBER_ARRAY)[partyMemberIndex];
    if (character_ID == 0xFF && !(gContext.items.getResource(itemID).auxData.target_all)) {
        playMenuSound(3);
        return;
    }
    handleUsableItemEffects(itemID, inventory_index, partyMemberIndex, event->menu);
}

void arrangeItemsHandler(const MenuInputEvent* event) {
    if (event->menuState != 4)
        return;

    playMenuSound(1);
    auto arrangeChoice = getStateCursor(event->menu, 4)->context;
    if (arrangeChoice.relativeRowIndex) {
        sortInventory(arrangeChoice.relativeRowIndex); //Arranging the inventory, this method will have to be rewritten
        setMenuState(event->menu, 0);
    }
    else {
        *ITEM_TO_SWAP_SELECTED = 0; //Clear the globals that are used by the custom sort routine for swapping items
        *TEMP_COLUMN_INDEX = 0;
        *TEMP_ABSOLUTE_CURSOR_INDEX = 0;
        setMenuState(event->menu, 5);
    }
}

//INVENTORY CANCEL HANDLERS
void exitInventoryHandler(const MenuInputEvent* event) {
    if (event->menuState != 0)
        return;

    playMenuSound(4);
    set_some_transition_data(5, 0); // These functions handle transitioning to different parts of the menu
    set_more_transition_data(0);
}

void exitItemView(const MenuInputEvent* event) {
    if (event->menuState != 1)
        return;
    playMenuSound(4);
    setMenuState(event->menu, 0);
}

void exitSwapHandler(const MenuInputEvent* event) {
    if (event->menuState != 5)
        return;

    playMenuSound(4);
    setMenuState(event->menu, 0);

}

void exitTargetingHandler(const MenuInputEvent* event) {
    if (event->menuState != 2)
        return;

    playMenuSound(4);
    setMenuState(event->menu, 1);
}

void exitKeyItemsHandler(const MenuInputEvent* event) {
    if (event->menuState != 3)
        return;

    playMenuSound(4);
    setMenuState(event->menu, 0);
}

void exitArrangeHandler(const MenuInputEvent* event) {
    if (event->menuState != 4)
        return;

    playMenuSound(4);
    setMenuState(event->menu, 0);
}



void handleUsableItemEffects(u16 item_ID, u16 inventory_index, u32 partyMemberIndex, Menu* menu) {
    auto itemWasUsed = false;

    /*Call the appropriate function handler for using items on a character/the party*/
    itemWasUsed = gContext.onUseHandlers.get_handler(item_ID)((u16)partyMemberIndex, item_ID, inventory_index);
    if (itemWasUsed) {
        gContext.inventory->decrementInventoryEntry(inventory_index, 1);
        if (gContext.inventory->getResource(inventory_index).materiaID == 0xFFFF)// If the Inventory Entry is -1, i.e it has been used up
            setMenuState(menu, 1);
    }
}
