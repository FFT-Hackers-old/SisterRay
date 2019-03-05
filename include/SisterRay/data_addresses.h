#ifndef SR_ADDRESSES_H
#define SR_ADDRESSES_H

#define MATERIA_DATA_PTR                     (void*)0xdbdf60
#define ITEM_DATA_PTR                        (void*)0xDBD160
#define WEAPON_DATA_PTR                      (void*)0xDBE730
#define ARMOR_DATA_PTR                       (void*)0xdbcce0
#define ACCESSORY_DATA_PTR                   (void*)0xdbcae0

#define SAVEMAP_INVENTORY_PTR                (void*)0xDC0234
#define BATTLE_INVENTORY_PTR                 (void*)0x9AC354

#define INVENTORY_MENU_STATE                 (u32*)0xDD19C8
#define INVENTORY_CURRENT_PARTY_INDEX        (u32*)0xDD1A8C
#define TEMP_COLUMN_INDEX                    (u32*)0xDD1A10
#define TEMP_ABSOLUTE_CURSOR_INDEX           (u32*)0xDD1A14
#define INVENTORY_MENU_CURSOR_CONTEXTS       (i32*)0xDD1A18
#define INVENTORY_ARRANGE_TYPE               (u32*)0xDD1AFC

#define KEY_ITEMS_INVENTORY_PTR              (u8*)0xDD19D0
#define dword_DD1A80                         (u32*)0xDD1A80 //If 0, can't use items... maybe inventory empty?
#define MENU_TEXTS                           (u8*)0x921168

#define GLOBAL_MENU_VIEW_SIZE                (u16*)0xDD17F0
#define GLOBAL_MENU_ROW_COUNT                (u16*)0xDD17F2
#define GLOBAL_MENU_ROW_BASE                 (u16*)0xDD17F4
#define word_DD17F6                          (u16*)0xDD17F6
#define word_DD17F8                          (u16*)0xDD17F8
#define word_DD17FA                          (u16*)0xDD17FA
#define word_DD17FC                          (u16*)0xDD17FC
#define use_on_characters_enabled            (u32*)0xDD1B68
#define ITEM_TO_SWAP_SELECTED                (u32*)0xDD1B6C

// These Globals are use to communicate between update loops by the battle module
#define BATTLE_MENU_STATE                    (u32*)0x91EF9C
#define PREVIOUS_BATTLE_MENU_STATE           (u32*)0x91EF98
#define ACTIVE_MENU_OWNER_PARTY_INDEX        (u8*)0xDC3C7C
#define COMMAND_TRIGGER_INDEX                (u8*)0xDC3C74
#define GLOBAL_BATTLE_ITEM_USED              (u16*)0xDC3C78
#define GLOBAL_USED_ITEM_TARGET_DATA         (u16*)0xDC3C84
#define GLOBAL_USED_INVENTORY_INDEX          (u16*)0xDC3C54
#define GLOBAL_USED_ITEM_RESTORE             (u16*)0xDC2088
#define ACCEPTING_BATTLE_INPUT               (u32*)0xDC35AC //checked in menu handlers to determine if input can be accepted. Maybe pause?
#define HANDLER_STATE_ARRAY                  (u8*)0xDC2068
#define byte_9AC111                          (u8*)0x9AC111
// End

#define CURRENT_PARTY_MEMBER_ARRAY           (u32*)0xDC0230 /*Length 3, contains party member ID, from the savemap*/


#define dword_DD1AE4                         (u32*)0xDD1AE4 // Not sure what this is, but it is only called in key-item view contexts
#define dword_DD1A3C                         (u32*)0xDD1A3C
#define dword_DD18C0                         (u32*)0xDD18C0
#define dword_DC1210                         (u32*)0xDC1210
#define dword_DD1A48                         (u32*)0xDD1A48
#define dword_DD1B54                         (u32*)0xDD1B54
#define dword_DC130C                         (u32*)0xDC130C
#define dword_921C98                         (u32*)0x921C98
#define byte_DC0C3C                          (u8*)0xDC0C3C
#define byte_DD18C8                          (u8*)0xDD18C8
#define dword_DC20D8                         (u32*)0xDC20D8
#define REMEMBER_CURSOR_POSITION             (u32*)0xDC3C5C

#endif
