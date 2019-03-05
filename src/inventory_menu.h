#ifndef INVENTORY_MENU_H
#define INVENTORY_MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>

SISTERRAY_API i32 onEnterInventory();
SISTERRAY_API void inventoryMenuUpdateHandler(int a1);
void display_active_cursor_state(int a1);
void display_inventory_views(int a1);
void render_inventory_main_view(int custom_arrange_active);
void render_character_portraits();
void render_key_items_view();
void handle_inventory_input(int a1);
void handle_usable_item_effects(u16 item_ID, u16 inventory_index);

#endif
