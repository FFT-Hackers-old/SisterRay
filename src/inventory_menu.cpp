#include "inventory_menu.h"
#include "impl.h"
#include "windows.h"

#define SIZE_OF_CURSOR_STRUCT  (i32)0xE // this is the size in i32, for ptr arithmetic

SISTERRAY_API i32 onEnterInventory() {
    i32 ret;

    *INVENTORY_MENU_STATE = 1;
    i32* top_view_cursor_struct = INVENTORY_CURSOR_POSITION;
    i32* main_item_view_cursor_struct = INVENTORY_CURSOR_POSITION + SIZE_OF_CURSOR_STRUCT;
    i32* key_items_cursor_struct = INVENTORY_CURSOR_POSITION + 2 * SIZE_OF_CURSOR_STRUCT;
    set_cursor_data_values((u32*)top_view_cursor_struct, 0, 0, 3, 1, 0, 0, 3, 1, 0, 0, 1, 0, 0, 0);
    set_cursor_data_values((u32*)main_item_view_cursor_struct, 0, 0, 1, 10, 0, 0, 1, gContext.inventory->current_capacity(), 0, 0, 0, 0, 0, 1);
    set_cursor_data_values((u32*)key_items_cursor_struct, 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 0, 1, 0, 0); //Initialize Cursor for top menu pane
    ret = sub_714FA3();
    if (*dword_DC130C == 1)
        *dword_DD18C0 = (u32)dword_921C98;
    else
        *dword_DD18C0 = (u32)&((menuWindowConfig)[0]);
    return ret;
}

SISTERRAY_API void inventoryMenuUpdateHandler(int a1)
{
    display_active_cursor_state(a1);
    display_inventory_views(a1);
    if (!is_input_handling_enabled())
    {
        handle_inventory_input(a1);
    }
}

void display_active_cursor_state(int a1) {
    u32 inventory_menu_state = *(INVENTORY_MENU_STATE);
    u32 party_member_index = *(INVENTORY_CURRENT_PARTY_INDEX);
    u32 active_window_base_row = *(VISIBLE_ITEM_START);
    u32 relative_item_index = *(RELATIVE_ITEM_INDEX);
    u32 inventory_arrange_type = *(INVENTORY_ARRANGE_TYPE);
    i32* inventory_cursor_position = (INVENTORY_CURSOR_POSITION);
    char* fetched_description;

    u16 item_ID;

    sub_6C98A6();
    if (inventory_menu_state == 2)
    {
        if (!(*use_on_characters_enabled)) {
            item_ID = gContext.inventory->get_resource(active_window_base_row + relative_item_index).item_id;;
            if (!(gContext.item_on_use_data.get_resource(item_ID).target_all))
                display_cursor(0, 120 * party_member_index + 161, 0.0f); //if the cursor isn't targeting all
            else
                display_cursor(0, 120 * (a1 % 3) + 161, 0.0); // if the cursor is targeting all
        }
        if (a1 & 2)
            display_cursor(298, 37 * relative_item_index + 109, 0.1f);
        if (*use_on_characters_enabled)
            --(*use_on_characters_enabled);
    }
    switch (inventory_menu_state)
    {
    case 0:                                   // Nothing Selected, Default State
        display_cursor(93 * inventory_cursor_position[0] + 13, 26, 0.1f); //display cursor at the selected view
        break;
    case 1:                                   // Use Selected - Selecting Item
        if (a1 & 2)
            display_cursor(93 * inventory_cursor_position[0] + 13, 26, 0.1f);
        display_cursor(298, 37 * relative_item_index + 109, 0.1f);
        if (gContext.inventory->get_resource(active_window_base_row + relative_item_index).item_id != 0xFFFF)
        {
            fetched_description = get_description_from_global_id(gContext.inventory->get_resource(active_window_base_row + relative_item_index).item_id);
            displayTextAtLocation(27, 64, fetched_description, 7, 1036966167);
        }
        break;
    case 2:                                   // Use Selected - Targeting Party
        if (a1 & 2)
            display_cursor(93 * inventory_cursor_position[0] + 13, 26, 0.0f);
        if (gContext.inventory->get_resource(active_window_base_row + relative_item_index).item_id != 0xFFFF)
        {
            fetched_description = get_description_from_global_id(gContext.inventory->get_resource(active_window_base_row + relative_item_index).item_id);
            displayTextAtLocation(27, 64, fetched_description, 7, 1036966167);
        }
        break;
    case 3:                                   // Browsing Key Items
        if (a1 & 2)
            display_cursor(93 * inventory_cursor_position[0] + 13, 26, 0.001f);
        display_cursor(293 * (*KEY_ITEMS_COL_INDEX) + 5, 36 * (*KEY_ITEMS_ROW_INDEX) + 129, 0.001f);
        if ((KEY_ITEMS_INVENTORY_PTR)[2 * (*KEY_ITEMS_VIEW_BASE_ROW) + 2 * (*KEY_ITEMS_ROW_INDEX) + (*KEY_ITEMS_COL_INDEX)] != 0xFFFF) //If there is a key item at cursor matrix position
        {
            int cursor_array_position = (KEY_ITEMS_INVENTORY_PTR)[2 * (*KEY_ITEMS_VIEW_BASE_ROW) + 2 * (*KEY_ITEMS_ROW_INDEX) + (*KEY_ITEMS_COL_INDEX)];
            fetched_description = (char*)load_kernel_object_text(0xEu, cursor_array_position, 0); //The returned arg here is the item description
            displayTextAtLocation(27, 64, fetched_description, 7, 1036966167);
        }
        break;
    case 4:                                   //Selecting an Arrange Method
        if (a1 & 2)
            display_cursor(93 * inventory_cursor_position[0] + 13, 26, 0.001f);
        display_cursor(*(dword_DD18C0 + 24) - 30, *(dword_DD18C0 + 26) + 26 * inventory_arrange_type + 17, 0.001f);
        for (int j = 0; j < 8; ++j) {            // Loop over arrange types
            fetched_description = gContext.game_strings.inventory_menu_texts.get_string(j + 3); //read the arrange type text from an in memory 12 char byte array skipping "use, arrange, and key item"
            displayTextAtLocation(*(dword_DD18C0 + 24) + 13, *(dword_DD18C0 + 26) + 26 * j + 13, fetched_description, 7, 1008981770);
        }
        draw_menu_box((i16*)(&(menuWindowConfig)[3]), (float)1008981770); //Does this display text boses?
        break;
    case 5:                                   // Inside Custom Sort
        if (a1 & 2)
            display_cursor(93 * inventory_cursor_position[0] + 13, 26, 0.0f);
        if (gContext.inventory->get_resource(*CUSTOM_SORT_VIEW_BASE + *CUSTOM_SORT_RELATIVE_INDEX).item_id != 0xFFFF)
        {
            fetched_description = get_description_from_global_id(gContext.inventory->get_resource(active_window_base_row + relative_item_index).item_id);
            displayTextAtLocation(27, 64, fetched_description, 7, 1036966167);
        }
        break;
    default:
        break;
    }
}

/*This function displaces texts depending on current "view" in the menu*/
void display_inventory_views(int a1) {
    u32 inventory_menu_state = *(INVENTORY_MENU_STATE);
    int* inventory_cursor_position = (INVENTORY_CURSOR_POSITION); //might be better to call this 'current menu view'

    int menu_state_local;

    if (inventory_cursor_position[0] != 2)     // Something with party members unless on cursor positioned on key items
    {
        render_character_portraits();
    }

    for (int menu_text_index = 0; menu_text_index < 3; ++menu_text_index) //display the "Use, Arrange, and Key Item fields
        displayTextAtLocation(93 * menu_text_index + 57, 17, gContext.game_strings.inventory_menu_texts.get_string(menu_text_index), 7, 1036966167);
    sub_6FA12F(0, 102, 640, 372);

    if (inventory_cursor_position[0] == 2)     // If cursor positioned on key items
    {
        render_key_items_view();
    }
    else
    {
        if (inventory_menu_state == 5)            // if currently in custom sort, animate flashing cursor?
        {
            if (ITEM_TO_SWAP_SELECTED)
            {
                if (a1 & 2)
                {
                    i32 v45 = 37 * (*CUSTOM_SORT_TEMP_INDEX) - 37 * (*CUSTOM_SORT_VIEW_BASE) + 9 * (*dword_DD1B54) - 9;
                    if (v45 > -37 && v45 < 1369)
                        display_cursor(291, v45 + 113, 0.0);
                }
            }
            display_cursor(298, 37 * (*CUSTOM_SORT_RELATIVE_INDEX) + 113, 0.0099999998f);
            menu_state_local = 5;                 // Set the local to 5 if we're in custom sort
        }
        else
        {
            menu_state_local = 1;                 //Set it to 1 otherwise
        }
        /*Render the main "item list" view regardless of whether custom arrange is active or not*/
        render_inventory_main_view(menu_state_local);
    }
    sub_6FA347();
    draw_menu_box((i16*)(&(menuWindowConfig)[0]), 0.111f);
    draw_menu_box(((i16*)&(menuWindowConfig)[1]), 0.2f);
    draw_menu_box(((i16*)&(menuWindowConfig)[2]), 0.30000000f);
}


void render_inventory_main_view(int custom_arrange_active) {
    u32 relative_item_index = *(RELATIVE_ITEM_INDEX);

    char* kernel_object_name;
    u16 item_ID;
    u8 item_quantity;
    int visible_item_inventory_index;
    int text_color;

    *GLOBAL_MENU_VIEW_SIZE = (u16)10;                         // number of rows active in an inventory view
    *GLOBAL_MENU_ROW_COUNT = (u16)gContext.inventory->current_capacity();                        // max size of the inventory.. let's change it
    *GLOBAL_MENU_ROW_BASE = (CURSOR_STRUCT_VISIBLE_BASE_MEMBER)[14 * custom_arrange_active];
    *word_DD17F6 = 618;
    *word_DD17F8 = 102;
    *word_DD17FA = 17;
    *word_DD17FC = 372;
    renderSideScroller((i32)GLOBAL_MENU_VIEW_SIZE, 0.1f); //The address here is passed as an int and then "casted" back and used as a ptr by the function at this point
    int displayed_row_count = ((dword_DD1A48)[14 * custom_arrange_active] != 0) + 10;

    for (int visible_item = 0; visible_item < displayed_row_count; ++visible_item) {
        visible_item_inventory_index = visible_item + (CURSOR_STRUCT_VISIBLE_BASE_MEMBER)[14 * custom_arrange_active];
        if (gContext.inventory->get_resource(visible_item_inventory_index).item_id != 0xFFFF) {
            item_ID = gContext.inventory->get_resource(visible_item_inventory_index).item_id;
            item_quantity = gContext.inventory->get_resource(visible_item_inventory_index).quantity;
            text_color = usable_in_inventory_menu(item_ID) ? 0 : 7; // This sets something based on whether the item is usable, assuming it's text color
            kernel_object_name = getNameFromItemID(item_ID);
            displayTextAtLocation(373, 37 * visible_item + 9 * (dword_DD1A3C)[14 * custom_arrange_active] + 109, kernel_object_name, (u8)text_color, 1036966167);

            displayVisibleItemIcon(343, 37 * visible_item + 9 * (dword_DD1A3C)[14 * custom_arrange_active] + 105, item_ID, 0, 1036966167);
            sub_6F5C0C(548, 37 * visible_item + 9 * (dword_DD1A3C)[14 * custom_arrange_active] + 114, 213, (u8)text_color, 1036966167);
            renderNumbers(550, 37 * visible_item + 9 * (dword_DD1A3C)[14 * custom_arrange_active] + 112, item_quantity, 3, (u8)text_color, 1036966167);
        }
    }
}


void render_character_portraits() {
    u16 unk_local_struct[4] = {0, (u16)96, (u16)300, u16(384)};

    for (int current_party_member = 0; current_party_member < 3; ++current_party_member) { //loop over and render character portraits, probably
        if ((CURRENT_PARTY_MEMBER_ARRAY)[current_party_member] != 0xFF) {         //if there is a party member in that slot
            render_HP_bar_and_status(133, 120 * current_party_member + 126, current_party_member, 1036831949); //possibly display picture?
            sub_6E6C5B(37, 120 * current_party_member + 116, current_party_member, 1036831949);
        }
    }

    // initialize_menu_window_struct((u16*)&unk_local_struct, (u16)0, (u16)96, (u16)300, (u16)384); //set some values in a struct/array used in the next call
    draw_menu_box((i16*)(&unk_local_struct), 0.1f); //this does a bunch of shit with the above struct
}


void render_key_items_view() {
    char* key_item_name;

    *GLOBAL_MENU_VIEW_SIZE = 10;
    *GLOBAL_MENU_ROW_COUNT = 32; //Number of items in one column in key items menu
    *GLOBAL_MENU_ROW_BASE = (*KEY_ITEMS_VIEW_BASE_ROW);
    *word_DD17F6 = 618; /*Still need to figure out what these values do*/
    *word_DD17F8 = 102;
    *word_DD17FA = 17;
    *word_DD17FC = 372;
    renderSideScroller((i32)GLOBAL_MENU_VIEW_SIZE, 0.1f);

    for (int visible_key_item_row = 0; visible_key_item_row < 12; ++visible_key_item_row) {
        int flat_key_item_index_base = 2 * visible_key_item_row + 2 * (*KEY_ITEMS_VIEW_BASE_ROW);
        for (int key_item_column = 0; key_item_column < 2; ++key_item_column) {
            if ((KEY_ITEMS_INVENTORY_PTR)[key_item_column + flat_key_item_index_base] != 0xFF) {
                key_item_name = (char *)load_kernel_object_text(0xEu, (KEY_ITEMS_INVENTORY_PTR)[(u8)(key_item_column + flat_key_item_index_base)], 8); //fetch the name of the key item
                displayTextAtLocation(293 * key_item_column + 53, 36 * visible_key_item_row + 9 * (*dword_DD1AE4) + 124, key_item_name, 7, 1036966167); //display the text
            }
        }
    }
}


/*WIP here, need a more generic way to introduce "usable" items through a function registry or something similar*/
void handle_inventory_input(int a1) {
    u32* inventory_menu_state = INVENTORY_MENU_STATE;
    i32* inventory_cursor_position = INVENTORY_CURSOR_POSITION;
    u32 party_member_index = *(INVENTORY_CURRENT_PARTY_INDEX);
    u32 active_window_base_row = *(VISIBLE_ITEM_START);
    u32 relative_item_index = *(RELATIVE_ITEM_INDEX);
    u16 item_id;

    update_cursor_position((u32*)&(inventory_cursor_position[14 * (*inventory_menu_state)]));
    switch ((*inventory_menu_state))
    {
    case 0:
        if (check_received_input(32)) {               // handling inputs?
            play_menu_sound(1);
            if (inventory_cursor_position[0]) {
                if (inventory_cursor_position[0] == 1) {
                    set_cursor_data_values(CURSOR_STRUCT_PTR, 0, 0, 1, 8, 0, 0, 1, 8, 0, 0, 0, 1, 0, 0); //Sets cursor data for the arrange menu
                    *inventory_menu_state = 4;         // Arrange Menu
                }
                else if (inventory_cursor_position[0] == 2) {
                    set_cursor_data_values(KEY_ITEMS_COL_INDEX, 0, 0, 2, 10, 0, 0, 2, 32, 0, 0, 2, 0, 0, 1); //sets cursor data for the key items menu
                    *inventory_menu_state = 3;         // Key Items Menu
                }
            }
            else {
                *inventory_menu_state = 1;             // use looking at item list
            }
        }
        else if (check_cancel_input(64)) {
            play_menu_sound(4);
            set_some_transition_data(5, 0); // These functions handle transitioning to different parts of the menu
            set_more_transition_data(0);
        }
        break;
    case 1:
        if (!(*dword_DD1A80)) {
            if (check_received_input(32)) {
                if (gContext.inventory->get_resource(active_window_base_row + relative_item_index).item_id == 0xFFFF) {
                    play_menu_sound(3);
                }
                else {
                    item_id = gContext.inventory->get_resource(active_window_base_row + relative_item_index).item_id;
                    if (usable_in_inventory_menu(item_id)) {
                        play_menu_sound(3);
                    }
                    else if (!(gContext.item_on_use_data.get_resource(item_id).requires_target)) {  
                        gContext.untargeted_handlers.get_handler(item_id)();

                    }
                    else { //If the item is usable, but requires a target, then jump to state 2
                        play_menu_sound(1);
                        *use_on_characters_enabled = 0; //Setting this to 0 enables items to be used on characters in menu state 2
                        *inventory_menu_state = 2;
                    }
                }
            }
            else if (check_received_input(64)) {
                play_menu_sound(4);
                *inventory_menu_state = 0;
            }
        }
        break;
    case 2:
        if (*use_on_characters_enabled)
            return;
        if (check_received_input(32)) {  //if "ok" input was received
            u16 inventory_index = active_window_base_row + relative_item_index;
            item_id = gContext.inventory->get_resource(inventory_index).item_id;
            u8 character_ID = (CURRENT_PARTY_MEMBER_ARRAY)[party_member_index];
            if (character_ID == 0xFF && !(gContext.item_on_use_data.get_resource(item_id).target_all)) { // Can't use item on empty party member unless it is megalixer or tent
                play_menu_sound(3);
                return;
            }
            /*Handle the on use effects for every item in the game, this needs to be made more generic*/
            handle_usable_item_effects(item_id, inventory_index);
        }
        else if (check_cancel_input(64)) {
            play_menu_sound(4);
            *inventory_menu_state = 1;
        }
        break;
    case 3:
        if (check_received_input(64))                // Exit Key Items
        {
            play_menu_sound(4);
            *inventory_menu_state = 0;
        }
        break;
    case 4:
        if (check_received_input(32))                // If OK received during arrange menu
        {
            play_menu_sound(1);
            if (*INVENTORY_ARRANGE_TYPE)
            {
                sort_inventory(*INVENTORY_ARRANGE_TYPE); //Arranging the inventory, this method will have to be rewritten
                *inventory_menu_state = 0;
            }
            else
            {
                set_cursor_data_values(dword_DD1B30, 0, 0, 1, 10, 0, 0, 1, gContext.inventory->current_capacity(), 0, 0, 0, 0, 0, 1);// Custom Sort Cursor Data Copy?
                *ITEM_TO_SWAP_SELECTED = 0; //Clear the globals that are used by the custom sort routine for swapping items
                *UNKNOWN_CUSTOM_SORT_GLOBAL = 0;
                *CUSTOM_SORT_TEMP_INDEX = 0;
                *inventory_menu_state = 5; //Move to custom sort state
            }
        }
        else if (check_received_input(64))           // Arrange Cancel Handler
        {
            play_menu_sound(4);
            *inventory_menu_state = 0;
        }
        break;
    case 5:                                   // Custom Arrange Handler
        if (check_received_input(32))
        {
            if (*ITEM_TO_SWAP_SELECTED) // If this is already set when input is received, then switch the items. It's really a bool value
            {
                if (*ITEM_TO_SWAP_SELECTED == 1)
                {
                    play_menu_sound(1);
                    /*This code swaps two items in the inventory*/
                    InventoryEntry temp_entry = gContext.inventory->get_resource(*CUSTOM_SORT_TEMP_INDEX); //copy cursor start
                    gContext.inventory->get_resource(*CUSTOM_SORT_TEMP_INDEX) = gContext.inventory->get_resource(*CUSTOM_SORT_VIEW_BASE + *CUSTOM_SORT_RELATIVE_INDEX); 
                    gContext.inventory->get_resource(*CUSTOM_SORT_VIEW_BASE + *CUSTOM_SORT_RELATIVE_INDEX) = temp_entry;
                    *ITEM_TO_SWAP_SELECTED = 0;
                }
            }
            else
            {
                play_menu_sound(1);
                *UNKNOWN_CUSTOM_SORT_GLOBAL = *dword_DD1B30; //copy first dword of struct set in previous state here, seems to always be 0
                *CUSTOM_SORT_TEMP_INDEX = *CUSTOM_SORT_VIEW_BASE + *CUSTOM_SORT_RELATIVE_INDEX; //custom sort base row and relative offsets copied when you select an item to swap
                *ITEM_TO_SWAP_SELECTED = 1;
            }
        }
        else if (check_received_input(64))
        {
            play_menu_sound(4);
            *inventory_menu_state = 0;
        }
        break;
    default:
        return;
    }
}

/*This routine handles executing menu usable item routines*/
void handle_usable_item_effects(u16 item_ID, u16 inventory_index) {
    u32 party_member_index = *(INVENTORY_CURRENT_PARTY_INDEX);
    auto item_was_used = false;

    /*Call the appropriate function handler for using items on a character/the party*/
    item_was_used = gContext.on_use_handlers.get_handler(item_ID)((u16)party_member_index, item_ID, inventory_index);
    if (item_was_used) {
        gContext.inventory->handle_decrement_inventory(inventory_index, 1);
        if (gContext.inventory->get_resource(inventory_index).item_id == 0xFFFF)// If the Inventory Entry is -1, i.e it has been used up
            *INVENTORY_MENU_STATE = 1;
    }
}
