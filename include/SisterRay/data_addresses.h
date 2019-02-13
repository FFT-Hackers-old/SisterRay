#ifndef SR_ADDRESSES_H
#define SR_ADDRESSES_H

#define MATERIA_DATA_PTR (void*)0x00dbdf60
#define ARMOR_DATA_PTR (void*)0x00dbcce0
#define ACCESSORY_DATA_PTR (void*)0x00dbcae0

#define SAVEMAP_INVENTORY_PTR = (void*)0x00DC0234
#define BATTLE_INVENTORY_PTR = (void*)0x009AC354

#define INVENTORY_MENU_STATE                 (u32*)0x00DD19C8
#define INVENTORY_CURRENT_PARTY_INDEX        (u32*)0x00DD1A8C
#define UNKNOWN_INVENTORY_MENU_GLOBAL_DD1B68 (u32*)0x00DD1B68
#define VISIBLE_ITEM_START                   (u32*)0x00DD1A64
#define RELATIVE_ITEM_INDEX                  (u32*)0x00DD1A54
#define INVENTORY_CURSOR_POSITION            (u32*)0x00DD1A18
#define INVENTORY_ARRANGE_TYPE               (u32*)0x00DD1AFC

#define KEY_ITEMS_INVENTORY_PTR  (u8*)0xDD19D0
#define KEY_ITEMS_VIEW_BASE_ROW  (u32*)0xDD1AD4 //Moves as the cursor scrolls
#define KEY_ITEMS_ROW_INDEX      (u32*)0xDD1AC4
#define KEY_ITEMS_COL_INDEX      (u32*)0xDD1AC0 //Always 0 or 1
#define MENU_TEXTS               (u8*)0x921168

#define GLOBAL_MENU_VIEW_SIZE    (u32*)0xDD17F0
#define GLOBAL_MENU_ROW_COUNT    (u32*)0xDD17F2
#define GLOBAL_MENU_ROW_BASE     (u32*)0xDD17F4
#define word_DD17F6              (u32*)0xDD17F6
#define word_DD17F8              (u32*)0xDD17F8
#define word_DD17FA              (u32*)0xDD17FA
#define word_DD17FC              (u32*)0xDD17FC

#define CURSOR_STRUCT_VISIBLE_BASE_MEMBER     (u32*)0xDD1A2C

#define CURRENT_PARTY_MEMBER_ARRAY     (u32*)0xDC0230 /*Length 3, contains party member ID, from the savemap*/

#define dword_DD1AE4             (u32*)0xDD1AE4 // Not sure what this is, but it is only called in key-item view contexts
#define dword_DD1A3C             (u32*)0xDD1A3C
#endif
