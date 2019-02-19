#include "inventory_menu.h"
#include "impl.h"
#include "windows.h"

SISTERRAY_API void new_inventory_menu_handler(int a1)
{
    if (dword_DC130C == 1) // Unclear if this is actually ever set
    {
    }
    else
    {
        display_active_cursor_state(a1);
        display_inventory_views(a1);
    }
    if (!sub_6C9808())
    {
        handle_inventory_input(a1);
    }
}

void display_active_cursor_state(int a1) {
    u32 inventory_menu_state = *(INVENTORY_MENU_STATE);
    u32 party_member_index = *(INVENTORY_CURRENT_PARTY_INDEX);
    u32 use_on_characters_enabled = *(UNKNOWN_INVENTORY_MENU_GLOBAL_DD1B68);
    u32 active_window_base_row = *(VISIBLE_ITEM_START);
    u32 relative_item_index = *(RELATIVE_ITEM_INDEX);
    u32 inventory_arrange_type = *(INVENTORY_ARRANGE_TYPE);
    int* inventory_cursor_position = (INVENTORY_CURSOR_POSITION);
    u8* MenuTexts[132] = MENU_TEXTS;
    void** fetched_description;

    u16 item_ID;

    sub_6C98A6(dword_DC1210);
    if (inventory_menu_state == 2)
    {
        if (!use_on_characters_enabled)
        {
            item_ID = gContext.inventory.data[active_window_base_row + relative_item_index].item_id;;
            if (item_ID != 6 && item_ID != 70)
                sub_6EB3B8(0, 120 * party_member_index + 161, 0.0);
            else
                sub_6EB3B8(0, 120 * (a1 % 3) + 161, 0.0);
        }
        if (a1 & 2)
            sub_6EB3B8(298, 37 * relative_item_index + 109, 0.1);
        if (use_on_characters_enabled)
            --use_on_characters_enabled;
    }
    NopInt32();
    switch (inventory_menu_state)
    {
    case 0:                                   // Nothing Selected, Default State
        sub_6EB3B8(93 * inventory_cursor_position[0] + 13, 26, 0.1);
        break;
    case 1:                                   // Use Selected - Selecting Item
        if (a1 & 2)
            sub_6EB3B8(93 * inventory_cursor_position[0] + 13, 26, 0.1);
        sub_6EB3B8(298, 37 * relative_item_index + 109, 0.1);
        if (gContext.inventory.data[active_window_base_row + relative_item_index].item_id != 0xFFFF)
        {
            fetched_description = load_kernel_object_text(4u, gContext.inventory.data[active_window_base_row + relative_item_index].item_id, 0);
            display_text_at_location(27, 64, fetched_description, 7, 1036966167);
        }
        break;
    case 2:                                   // Use Selected - Targeting Party
        if (a1 & 2)
            sub_6EB3B8(93 * inventory_cursor_position[0] + 13, 26, 0.0);
        if (gContext.inventory.data[active_window_base_row + relative_item_index].item_id != 0xFFFF)
        {
            fetched_description = load_kernel_object_text(4u, gContext.inventory.data[active_window_base_row + relative_item_index].item_id, 0);
            display_text_at_location(27, 64, fetched_description, 7, 1036966167);
        }
        break;
    case 3:                                   // Browsing Key Items
        if (a1 & 2)
            sub_6EB3B8(93 * inventory_cursor_position[0] + 13, 26, 0.001);
        sub_6EB3B8(293 * (*KEY_ITEMS_COL_INDEX) + 5, 36 * (*KEY_ITEMS_ROW_INDEX) + 129, 0.001);
        if (KEY_ITEMS_INVENTORY_PTR[2 * (*KEY_ITEMS_VIEW_BASE_ROW) + 2 * (*KEY_ITEMS_ROW_INDEX) + (*KEY_ITEMS_COL_INDEX)] != 0xFFFF) //If there is a key item at cursor matrix position
        {
            int cursor_array_position = KEY_ITEMS_INVENTORY_PTR[2 * (*KEY_ITEMS_VIEW_BASE_ROW) + 2 * (*KEY_ITEMS_ROW_INDEX) + (*KEY_ITEMS_COL_INDEX)];
            fetched_description = load_kernel_object_text(0xEu, cursor_array_position, 0); //The returned arg here is the item description
            display_text_at_location(27, 64, fetched_description, 7, 1036966167);
        }
        break;
    case 4:                                   //Selecting an Arrange Method
        if (a1 & 2)
            sub_6EB3B8(93 * inventory_cursor_position[0] + 13, 26, 0.001);
        sub_6EB3B8(*(dword_DD18C0 + 24) - 30, *(dword_DD18C0 + 26) + 26 * inventory_arrange_type + 17, 0.001);
        for (int j = 0; j < 8; ++j)            // Loop over arrange types
            fetched_description = &(MenuTexts[12 * (j + 3)]) //read the arrange type text from an in memory 12 char byte array skipping "use, arrange, and key item"
            display_text_at_location(*(dword_DD18C0 + 24) + 13, *(dword_DD18C0 + 26) + 26 * j + 13, fetched_description, 7, 1008981770);
        sub_6E7D20(dword_DD18C0 + 24, 1008981770);
        break;
    case 5:                                   // Inside Custom Sort
        if (a1 & 2)
            sub_6EB3B8(93 * inventory_cursor_position[0] + 13, 26, 0.0);
        if (gContext.inventory.data[CUSTOM_SORT_VIEW_BASE + CUSTOM_SORT_RELATIVE_INDEX] != 0xFFFF)
        {
            fetched_description = load_kernel_object_text(4u, gContext.inventory.data[active_window_base_row + relative_item_index].item_id, 0);
            display_text_at_location(27, 64, fetched_description, 7, 1036966167);
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
    u8* menu_texts[132] = MENU_TEXTS;

    int menu_state_local;

    if (inventory_cursor_position[0] != 2)     // Something with party members unless on cursor positioned on key items
    {
        render_character_portraits();
    }

    for (int menu_text_index = 0; menu_text_index < 3; ++menu_text_index) //display the "Use, Arrange, and Key Item fields
        display_text_at_location(93 * menu_text_index + 57, 17, &menu_texts[12 * menu_text_index], 7, 1036966167);
    sub_6FA12F(0, 102, 640, 372);

    if (inventory_cursor_position[0] == 2)     // If cursor positioned on key items
    {
        render_key_items_view();
    }
    else
    {
        if (inventory_menu_state == 5)            // if currently in custom sort
        {
            if (ITEM_TO_SWAP_SELECTED)
            {
                if (a1 & 2)
                {
                    v45 = 37 * CUSTOM_SORT_TEMP_INDEX - 37 * CUSTOM_SORT_VIEW_BASE + 9 * dword_DD1B54 - 9;
                    if (v45 > -37 && v45 < 1369)
                        sub_6EB3B8(291, v45 + 113, 0.0);
                }
            }
            sub_6EB3B8(298, 37 * CUSTOM_SORT_RELATIVE_INDEX + 113, 0.0099999998);
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
    sub_6E7D20(dword_DD18C0, 1038308344);
    sub_6E7D20(dword_DD18C0 + 8, 1045220557);
    sub_6E7D20(dword_DD18C0 + 16, 1050253722);
}


void render_inventory_main_view(int custom_arrange_active) {
    u32 relative_item_index = *(RELATIVE_ITEM_INDEX);

    void** kernel_object_name;
    u16 item_ID;
    u8 item_quantity;
    int visible_item_inventory_index;
    int text_color;
    u8 single_byte_color;

    // The next three locals are used in some computation I don't currently understand the point of, rewrite later
    int unk_local_1;
    int unk_local_2;
    int unk_local_3;

    *GLOBAL_MENU_VIEW_SIZE = 10;                         // number of rows active in an inventory view
    *GLOBAL_MENU_ROW_COUNT = 320;                        // max size of the inventory.. let's change it
    *GLOBAL_MENU_ROW_BASE = CURSOR_STRUCT_VISIBLE_BASE_MEMBER[14 * custom_arrange_active];
    *word_DD17F6 = 618;
    *word_DD17F8 = 102;
    *word_DD17FA = 17;
    *word_DD17FC = 372;
    sub_6F7270(GLOBAL_MENU_VIEW_SIZE, 0.1); //Gets the number of rows in the current view and does something.
    int displayed_row_count = (dword_DD1A48[14 * custom_arrange_active] != 0) + 10;

    NopInt32();
    for (int visible_item = 0; visible_item < displayed_row_count; ++visible_item) {
        visible_item_inventory_index = visible_item + CURSOR_STRUCT_VISIBLE_BASE_MEMBER[14 * custom_arrange_active];
        if (gContext.inventory.data[visible_item_inventory_index].item_id != 0xFFFF) {
            item_ID = gContext.inventory.data[visible_item_inventory_index].item_id;
            // Do some stuff to assemble an argument for display text
            text_color = -((item_is_usable(item_ID) & 4) != 0);
            LOBYTE(unk_local_1) = unk_local_1 & 0xF9;
            unk_local_3 = unk_local_1 + 7;
            kernel_object_name = load_kernel_object_text(4u, item_ID, 8);
            LOBYTE(unk_local_2) = unk_local_3;
            // End do some stuff to assemble argument for siplay text
            display_text_at_location(373, 37 * visible_item + 9 * dword_DD1A3C[14 * custom_arrange_active] + 109, kernel_object_name, unk_local_2, 1036966167);
        }
    }
    for (int visible_item = 0; visible_item < displayed_row_count; ++visible_item) {
        visible_item_inventory_index = visible_item + CURSOR_STRUCT_VISIBLE_BASE_MEMBER[14 * custom_arrange_active];
        if (gContext.inventory.data[visible_item_inventory_index].item_id != 0xFFFF) {
            item_ID = gContext.inventory.data[visible_item_inventory_index + relative_item_index].item_id;
            item_quantity = gContext.inventory.data[visible_item_inventory_indexm + relative_item_index].quantity;
            text_color = (item_is_usable(item_ID) & 4) != 0 ? 0 : 7; // This sets something based on whether the item is usable, assuming it's text color

            display_visible_item_icon(343, 37 * visible_item + 9 * dword_DD1A3C[14 * custom_arrange_active] + 105, item_ID, 0, 1036966167);
            LOBYTE(single_byte_color) = text_color;
            sub_6F5C0C(548, 37 * visible_item + 9 * dword_DD1A3C[14 * custom_arrange_active] + 114, 213, v10, 1036966167);
            sub_6F9739(550, 37 * visible_item + 9 * dword_DD1A3C[14 * custom_arrange_active] + 112, item_quantity, 3, text_color, 1036966167);
        }
    }
}


void render_character_portraits() {
    u16* unk_local_struct[4];

    for (int current_party_member = 0; current_party_member < 3; ++current_party_member) { //loop over and render character portraits, probably
        if (CURRENT_PARTY_MEMBER_ARRAY[current_party_member] != 0xFF) {         //if there is a party member in that slot
            render_HP_bar_and_status(133, 120 * current_party_member + 126, current_party_member, 1036831949); //possibly display picture?
            sub_6E6C5B(37, 120 * current_party_member + 116, current_party_member, 1036831949);
        }
    }
    initialize_some_struct(unk_local_struct, 0, 96, 300, 384); //set some values in a struct/array used in the next call
    sub_6E7D20(unk_local_struct, 1036831949); //this does a bunch of shit with the above struct
}


void render_key_items_view() {
    u8** key_item_name;

    *GLOBAL_MENU_VIEW_SIZE = 10;
    *GLOBAL_MENU_ROW_COUNT = 32; //Number of items in one column in key items menu
    *GLOBAL_MENU_ROW_BASE = (*KEY_ITEMS_VIEW_BASE_ROW);
    *word_DD17F6 = 618; /*Still need to figure out what these values do*/
    *word_DD17F8 = 102;
    *word_DD17FA = 17;
    *word_DD17FC = 372;
    sub_6F7270(GLOBAL_MENU_VIEW_SIZE, 0.1);

    for (int visible_key_item_row = 0; visible_key_item_row < 12; ++visible_key_item_row) {
        int flat_key_item_index_base = 2 * visible_key_item_row + 2 * (*KEY_ITEMS_VIEW_BASE_ROW);
        for (int key_item_column = 0; key_item_column < 2; ++key_item_column) {
            if (KEY_ITEMS_INVENTORY_PTR[key_item_column + flat_key_item_index_base] != 0xFF) {
                key_item_name = load_kernel_object_text(0xEu, KEY_ITEMS_INVENTORY_PTR[key_item_column + flat_key_item_index_base], 8); //fetch the name of the key item
                display_text_at_location(293 * key_item_column + 53, 36 * visible_key_item_row + 9 * dword_DD1AE4 + 124, key_item_name, 7, 1036966167); //display the text
            }
        }
    }
}


/*WIP here, need a more generic way to introduce "usable" items through a function registry or something similar*/
void handle_inventory_input(int a1) {
    i32 inventory_menu_state = *(INVENTORY_MENU_STATE);
    i32 inventory_cursor_position = (INVENTORY_CURSOR_POSITION);
    u32 party_member_index = *(INVENTORY_CURRENT_PARTY_INDEX);
    u32 active_window_base_row = *(VISIBLE_ITEM_START);
    u32 relative_item_index = *(RELATIVE_ITEM_INDEX);
    u16 item_ID;

    update_cursor_position(&inventory_cursor_position[14 * inventory_menu_state]);
    switch (inventory_menu_state)
    {
    case 0:
        if (check_received_input(32)) {               // handling inputs?
            play_menu_sound(1);
            if (inventory_cursor_position[0]) {
                if (inventory_cursor_position[0] == 1) {
                    set_cursor_data_values(CURSOR_STRUCT_PTR, 0, 0, 1, 8, 0, 0, 1, 8, 0, 0, 0, 1, 0, 0); //Sets cursor data for the arrange menu
                    inventory_menu_state = 4;         // Arrange Menu
                }
                else if (inventory_cursor_position[0] == 2) {
                    set_cursor_data_values(KEY_ITEMS_COL_INDEX, 0, 0, 2, 10, 0, 0, 2, 32, 0, 0, 2, 0, 0, 1); //sets cursor data for the key items menu
                    inventory_menu_state = 3;         // Key Items Menu
                }
            }
            else {
                inventory_menu_state = 1;             // use looking at item list
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
                if (gContext.inventory.data[active_window_base_row + relative_item_index].item_id == 0xFFFF) {
                    play_menu_sound(3);
                }
                else {
                    item_ID = gContext.inventory.data[active_window_base_row + relative_item_index].item_id;
                    if (item_is_usable(item_ID) & 4) {
                        play_menu_sound(3);
                    }
                    else if (item_ID == 98) {        //Save Crytal case
                        play_menu_sound(263);
                        *byte_DC0C3C |= 1u;  //Save Crystal used?
                        set_some_transition_data(5, 0);
                        set_more_transition_data(0);
                        save_crystal_handler();
                    }
                    /*else if (item_ID == 103) {       //Earth Harp, disabled in the main game
                        play_menu_sound(263);
                        byte_DC00A5 = 1;
                        byte_DC00A4 = 6;
                        byte_DC00B2 = 1;
                        byte_DC00B3 = -1;
                        dword_DC00E0 = 0xFFFFFF;
                        byte_DC0129 = 1;
                        byte_DC0128 = 7;
                        byte_DC0136 = 1;
                        byte_DC0137 = -1;
                        dword_DC0164 = 0xFFFFFF;
                    }*/
                    else { //If the item is usable and not save crystal or guidebook, jump to the case 2 handler
                        play_menu_sound(1);
                        *use_on_characters_enabled = 0; //Setting this to 0 enables items to be used on characters in menu state 2
                        inventory_menu_state = 2;
                    }
                }
            }
            else if (check_received_input(64)) {
                play_menu_sound(4);
                inventory_menu_state = 0;
            }
        }
        break;
    case 2:
        if (*use_on_characters_enabled)
            return;
        if (check_received_input(32)) {  //if "ok" input was received
            u16 inventory_index = active_window_base_row + relative_item_index;
            item_ID = gContext.inventory.data[inventory_index].item_id;
            u8 character_ID = CURRENT_PARTY_MEMBER_ARRAY[party_member_index];
            if (character_ID == 0xFF && item_ID != 6 && item_ID != 70) { // Can't use item on empty party member unless it is megalixer or tent
                play_menu_sound(3);
                return;
            }
            /*Handle the on use effects for every item in the game, this needs to be made more generic*/
            handle_usable_item_effects(item_ID, inventory_index);
        }
        else if (check_cancel_input(64)) {
            play_menu_sound(4);
            inventory_menu_state = 1;
        }
        break;
    case 3:
        if (check_received_input(64))                // Exit Key Items
        {
            play_menu_sound(4);
            inventory_menu_state = 0;
        }
        break;
    case 4:
        if (check_received_input(32))                // If OK received during arrange menu
        {
            play_menu_sound(1);
            if (*INVENTORY_ARRANGE_TYPE)
            {
                sort_inventory(*INVENTORY_ARRANGE_TYPE); //Arranging the inventory, this method will have to be rewritten
                inventory_menu_state = 0;
            }
            else
            {
                set_cursor_data_values(&dword_DD1B30, 0, 0, 1, 10, 0, 0, 1, 320, 0, 0, 0, 0, 0, 1);// Custom Sort Cursor Data Copy?
                *ITEM_TO_SWAP_SELECTED = 0; //Clear the globals that are used by the custom sort routine for swapping items
                *UNKNOWN_CUSTOM_SORT_GLOBAL = 0;
                *CUSTOM_SORT_TEMP_INDEX = 0;
                inventory_menu_state = 5; //Move to custom sort state
            }
        }
        else if (check_received_input(64))           // Arrange Cancel Handler
        {
            play_menu_sound(4);
            inventory_menu_state = 0;
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
                    i32 v22 = gContext.inventory.data[*CUSTOM_SORT_TEMP_INDEX]; //copy cursor start
                    gContext.inventory.data[*CUSTOM_SORT_TEMP_INDEX] = gContext.inventory.data[*CUSTOM_SORT_VIEW_BASE + *CUSTOM_SORT_RELATIVE_INDEX]; 
                    gContext.inventory.data[*CUSTOM_SORT_VIEW_BASE + *CUSTOM_SORT_RELATIVE_INDEX] = v22;
                    *ITEM_TO_SWAP_SELECTED = 0;
                }
            }
            else
            {
                play_menu_sound(1);
                *UNKNOWN_CUSTOM_SORT_GLOBAL = dword_DD1B30; //copy first dword of struct set in previous state here, seems to always be 0
                *CUSTOM_SORT_TEMP_INDEX = *CUSTOM_SORT_VIEW_BASE + *CUSTOM_SORT_RELATIVE_INDEX; //custom sort base row and relative offsets copied when you select an item to swap
                *ITEM_TO_SWAP_SELECTED = 1;
            }
        }
        else if (check_received_input(64))
        {
            play_menu_sound(4);
            inventory_menu_state = 0;
        }
        break;
    default:
        return;
    }
}

/*This routine handles executing menu usable item routines*/
void handle_usable_item_effects(u16 item_ID, u16 inventory_index) {
    u32 party_member_index = *(INVENTORY_CURRENT_PARTY_INDEX);

    /*Call the appropriate function handler for using items on a character/the party*/
    auto item_was_used = gContext.on_use_handlers[item_ID]((u16)party_member_index, item_ID, inventory_index);
    if (item_was_used) {
        handle_decrement_inventory(inventory_index, 1);
        if (gContext.inventory.data[inventory_index].item_id == 0xFFFF)// If the Inventory Entry is -1, i.e it has been used up
            *INVENTORY_MENU_STATE = 1;
}
