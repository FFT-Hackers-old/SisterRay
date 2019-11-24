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

#define dword_DD17F0                         (u16*)0xDD17F0
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
#define BATTLE_ACTIVE_ACTOR_ID               (u8*)0xDC3C7C
#define COMMAND_TRIGGER_INDEX                (u8*)0xDC3C74


#define ISSUED_COMMAND_ID                    (u8*)0xDC3C70

#define ISSUED_ACTION_ID                     (u16*)0xDC3C78
#define ISSUED_ACTION_TARGET_DATA            (u16*)0xDC3C84
#define ISSUED_ACTION_MENU_INDEX             (u16*)0xDC3C54
#define ISSUED_ACTION_RESTORE_FLAG           (u16*)0xDC2088
#define ISSUED_ACTION_TARGET_TYPE            (u8*)0xDC3C90  
#define ISSUED_ACTION_TARGET_INDEX           (u8*)0xDC3C94
#define ISSUED_ACTION_STARTING_ROW           (u8*)0xDC3C58
#define ISSUED_ACTION_VALID_TARGET_MASK      (u16*)0xDC3C60

#define W_FIRST_ACTION_USED                  (u16*)0xDC38F4
#define W_FIRST_ACTION_INDEX                 (u16*)0xDC3638
#define W_FIRST_TARGETING_CURSOR_TYPE        (u8*)0xDC35C0
#define W_FIRST_VALID_TARGET_MASK            (u16*)0xDC35B0
#define W_FIRST_TARGET_INDEX                 (u8*)0xDC1F58
#define W_FIRST_STARTING_ROW                 (u8*)0xDC35BC

#define ACCEPTING_BATTLE_INPUT               (u32*)0xDC35AC //checked in menu handlers to determine if input can be accepted. Maybe pause?
#define HANDLER_STATE_ARRAY                  (u8*)0xDC2068
#define byte_9AC111                          (u8*)0x9AC111
#define BATTLE_PAUSED                        (u8*)0xDC7E70
#define CURSOR_MEMORY_ACTIVE                 (u32*)0xDC3C5C
// End

//These Globals are used to communicate between update loops by the Equip Menu
#define EQUIP_MENU_STATE                     (u32*)0xDCA6A0
#define EQUIP_MENU_PARTY_INDEX               (u32*)0xDCA4A4
#define EQUIP_MENU_CURSOR_CONTEXTS           (u32*)0xDCA5C0
#define DID_MATERIA_GEAR_CHANGE              (u32*)0xDCA7E8 // set immediately after an item is equiped
#define VIEW_PERSISTENT_ACTOR_INDEX          (u32*)0xDC1288
#define word_DD1BC0                          (u16*)0xDD1BC0
#define dword_DC1290                         (u32*)0xDC1290
#define byte_DC1148                          (u8*)0xDC1148
#define RECYCLE_SLOT_OFFSET_TABLE            (u32*)0x919928
#define byte_DC0B4B                          (u8*)0xDC0B4B
#define dword_DCA5C4                         (u32*)0xDCA5C4 //Stops one from de-equipping accessories if set to 2... Sephiroth maybe?
#define dword_DCA4A0                         (u32*)0xDCA4A0 //points to some constants which are used to draw strings athe right places. relocate in future
#define word_DCA490                          (u16*)0xDCA490
#define dword_DCA628                         (u32*)0xDCA628
// End

#define MAT_MENU_PARTY_INDEX                 (u32*)0xDD1638
//These Globals are used to communicate between update loops by the magic menu
#define enabledSpellTypeMask                 (u8*)0xDC0927
//

#define CURRENT_PARTY_MEMBER_ARRAY           ((u32*)0xDC0230) /*Length 3, contains party member ID, from the savemap*/


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
#define ENEMY_ACTOR_IDX                      (u32*)0xDC3C64
#define gBattlePaused                        (u8*)0xDC0E70
#define BATTLE_PAUSED_GLOBAL                 (u8*)0xDC0E6C
#define BATTLE_TARGETED_GLOBAL               (u8*)0xDC3C9C
#define LIMIT_ACTIVE_MASK                    (u8*)0x9A889E
#define word_9A889A                          (u16*)0x9A889A
#define W_COMMAND_ENABLED                    (u8*)0xDC3C88
#define BATTLE_ROW_MASKS                     ((u16*)0x9A88AA)

#endif
