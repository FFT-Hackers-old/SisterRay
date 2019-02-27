#ifndef SR_ADDRESSES_H
#define SR_ADDRESSES_H

#define MATERIA_DATA_PTR   (void*)0xdbdf60
#define ITEM_DATA_PTR      (void*)0xDBD160
#define WEAPON_DATA_PTR    (void*)0xDBE730
#define ARMOR_DATA_PTR     (void*)0xdbcce0
#define ACCESSORY_DATA_PTR (void*)0xdbcae0

#define SAVEMAP_INVENTORY_PTR  (void*)0x00DC0234
#define BATTLE_INVENTORY_PTR  (void*)0x009AC354

#define INVENTORY_MENU_STATE                 (u32*)0xDD19C8
#define INVENTORY_CURRENT_PARTY_INDEX        (u32*)0xDD1A8C
#define VISIBLE_ITEM_START                   (u32*)0xDD1A64
#define RELATIVE_ITEM_INDEX                  (u32*)0xDD1A54
#define CUSTOM_SORT_VIEW_BASE                (u32*)0xDD1B44
#define CUSTOM_SORT_RELATIVE_INDEX           (u32*)0xDD1B34
#define UNKNOWN_CUSTOM_SORT_GLOBAL           (u32*)0xDD1A10
#define CUSTOM_SORT_TEMP_INDEX               (u32*)0xDD1A14
#define INVENTORY_CURSOR_POSITION            (i32*)0xDD1A18
#define INVENTORY_ARRANGE_TYPE               (u32*)0xDD1AFC

#define KEY_ITEMS_INVENTORY_PTR  (u8*)0xDD19D0
#define KEY_ITEMS_VIEW_BASE_ROW  (u32*)0xDD1AD4 //Moves as the cursor scrolls
#define KEY_ITEMS_ROW_INDEX      (u32*)0xDD1AC4
#define KEY_ITEMS_COL_INDEX      (u32*)0xDD1AC0 //Always 0 or 1
#define dword_DD1A80             (u32*)0xDD1A80 //If 0, can't use items... maybe inventory empty?
#define MENU_TEXTS               (u8*)0x921168

#define GLOBAL_MENU_VIEW_SIZE    (u16*)0xDD17F0
#define GLOBAL_MENU_ROW_COUNT    (u16*)0xDD17F2
#define GLOBAL_MENU_ROW_BASE     (u16*)0xDD17F4
#define word_DD17F6              (u16*)0xDD17F6
#define word_DD17F8              (u16*)0xDD17F8
#define word_DD17FA              (u16*)0xDD17FA
#define word_DD17FC              (u16*)0xDD17FC
#define use_on_characters_enabled (u32*)0xDD1B68
#define ITEM_TO_SWAP_SELECTED    (u32*)0xDD1B6C


#define CURSOR_STRUCT_PTR        (u32*)0xDD1AF8
#define CURSOR_STRUCT_VISIBLE_BASE_MEMBER     (u32*)0xDD1A2C

#define CURRENT_PARTY_MEMBER_ARRAY     (u32*)0xDC0230 /*Length 3, contains party member ID, from the savemap*/

#define dword_DD1AE4             (u32*)0xDD1AE4 // Not sure what this is, but it is only called in key-item view contexts
#define dword_DD1A3C             (u32*)0xDD1A3C
#define dword_DD18C0             (u32*)0xDD18C0
#define dword_DC1210             (u32*)0xDC1210
#define dword_DD1B30             (u32*)0xDD1B30
#define dword_DD1A48             (u32*)0xDD1A48
#define dword_DD1B54             (u32*)0xDD1B54 
#define byte_DC0C3C              (u8*)0xDC0C3C
#define byte_DD18C8              (u8*)0xDD18C8

#endif
