#ifndef SISTER_RAY_ROUTINES_H
#define SISTER_RAY_ROUTINES_H

#include <SisterRay/types.h>

#define FORMULA_TABLE_LOCATION          ((void*)0x8FF1F8)
#define PHYSICAL_DAMAGE_FORMULA         ((void*)0x5DE5DF)
#define MAGICAL_DAMAGE_FORMULA          ((void*)0x5DE9B8)
#define CURRENT_HP_GRAVITY              ((void*)0x5DEA6D)
#define MAX_HP_GRAVITY                  ((void*)0x5DEAF7)
#define CURE_FORMULA                    ((void*)0x5DEB81)
#define CONSTANT_20                     ((void*)0x5DEBE5)
#define CONSTANT_16                     ((void*)0x5DEC0A)
#define RECOVERY_FORMULA                ((void*)0x5DEC52)
#define THROW_FORMULA                   ((void*)0x5DEC8A)
#define COIN_FORMULA                    ((void*)0x5DECAA)
#define MAIN_COMMAND_FORMULA            ((void*)0x435D81)
#define APPLY_ELEMENTAL_MODIFIERS       ((void*)0x5DB593)
#define POISON_SETUP_ROUTINE            ((void*)0x5C9FC0)
#define RUN_ANIMATION_SCRIPT            ((void*)0x41FBA4)
#define LOOKUP_ADDITIONAL_EFFECT        ((void*)0x4281B1)
#define POISON_ENQUEUE_CALLBACK         ((void*)0x434DB0)
#define DECREMENT_COUNTERS              ((void*)0x43526A)
#define APPLY_DAMAGE_ROUTINE            ((void*)0x5D9940)
#define LOAD_ABILITY_DATA               ((void*)0x5C94D3)
#define INIT_INVENTORY_CURSOR_STATE     ((void*)0x714EF2)
#define MAIN_INVENTORY_HANDLER          ((void*)0x715105)
#define INIT_BATTLE_INVENTORY           ((void*)0x5D1520)
#define RENDER_BATTLE_ITEM_MENU         ((void*)0x6DEBFE)
#define INIT_BATTLE_ITEM_MENU_CURSOR    ((void*)0x6D982C)
#define BATTLE_ITEM_MENU_INPUT_HANDLER  ((void*)0x6D98E3)

typedef void(*pfnnullmasks)();
typedef void(*pfnenqueueaction)(u16, u16, u8, u8, u16);
typedef i32(*pfnsadnessmodifier)(i32);
typedef i32(*pfnsplitdamagequadramagic)(i32, u32);
typedef i32(*pfnbarriersmodifier)(i32);
typedef u32(*pfnrandomvariation)(i32);
typedef u32(*pfnmpturbo)(i32);
typedef void(*pfnpoisonsetup)();
typedef void(*pfnloadabilitydata)();

#define nullMasks                       ((pfnnullmasks)0x5DBCDD)
#define enqueueAction                   ((pfnenqueueaction)0x432687)
#define sadnessModifier                 ((pfnsadnessmodifier)0x5DE958)
#define spltDamageQuadraMagicModifier   ((pfnsplitdamagequadramagic)0x5DE8F4)
#define barriersModifier                ((pfnbarriersmodifier)0x5DE82C)
#define randomVariation                 ((pfnrandomvariation)0x5DE988)
#define mpTurbo                         ((pfnmpturbo)0x5DE958)
#define poisonSetUp                     ((pfnpoisonsetup)0x5C9FC0)
#define loadAbilityData                 ((pfnloadabilitydata)0x5C94D3)


/*These are routines used in the inventory menu handler*/
typedef i32(*pfnsub6F0D7D)(i32, i32, float);
#define sub_6F0D7D                      ((pfnsub6F0D7D)0x6F0D7D)

typedef i32(*pfnsub6FAB2F)(i32, i32, const u32, u8, float);
#define sub_6FAB2F                      ((pfnsub6FAB2F)0x6FAB2F)

typedef void**(*pfnsub41963C)(u32, i32, i32);
#define load_kernel_object_text         ((pfnsub41963C)0x41963C)

typedef i32(*pfnsub6EB3B8)(i32, i32, float);
#define display_cursor                  ((pfnsub6EB3B8)0x6EB3B8)

typedef i32(*pfnsub6F5B03)(i32, i32, const char*, u8, u32); //The last argument might be a float, not sure yet
#define displayTextAtLocation        ((pfnsub6F5B03)0x6F5B03)

typedef i32(*pfnsub7150A0)(u16);
#define item_is_usable                  ((pfnsub7150A0)0x7150A0)

typedef i32(*pfnsub6F7270)(i32, float);
#define renderSideScroller                      ((pfnsub6F7270)0x6F7270)

typedef i16(*pfnsub6C62A2)(i32, i32, i32, i32);
#define render_HP_bar_and_status        ((pfnsub6C62A2)0x6C62A2)

/*This function sets some structs of size 0x70 based on the party member ID, not sure what they are yet*/
typedef u8*(*pfnsub6E6C5B)(i32, i32, i32, i32);
#define sub_6E6C5B                      ((pfnsub6E6C5B)0x6E6C5B)

typedef u16*(*pfnsub6E7BA1)(u16*, i16, i16, i16, i16);
#define initialize_menu_window_struct          ((pfnsub6E7BA1)0x6E7BA1)

typedef i32(*pfnsub6E7D20)(i16*, float);
#define draw_menu_box                      ((pfnsub6E7D20)0x6E7D20)

/*This function sets a variable based on the icon type neede in the menu and then calls a function to display it*/
typedef i32(*pfnsub717691)(i32, i32, i32, i32, i32);
#define displayVisibleItemIcon       ((pfnsub717691)0x717691)

/*Not sure what this function does*/
typedef u8(*pfnsub6F5C0C)(i32, i32, i8, i8, i32);
#define sub_6F5C0C                      ((pfnsub6F5C0C)0x6F5C0C)

typedef i32(*pfnsub6F9739)(i32, i32, u32, u8, u8, i32);
#define renderNumbers                      ((pfnsub6F9739)0x6F9739)

typedef i32(*pfnsub41AB74)(i32);
#define checkInputReceived2            ((pfnsub41AB74)0x41AB74)

typedef i32(*pfnsub6F53F1)(i32);
#define checkInputReceived              ((pfnsub6F53F1)0x6F53F1)

typedef void(*pfnsub74580A)(i32);
#define playMenuSound                 ((pfnsub74580A)0x74580A)

typedef void(*pfnsub6F4DB2)(u32*);
#define handleCursorPositionUpdate          ((pfnsub6F4DB2)0x6F4DB2)

typedef i32(*pfnsub6CBA6A)(u8, u16);
#define heal_character_at_index         ((pfnsub6CBA6A)0x6CBA6A)

typedef i32(*pfnsub6CBBBF)(u8, u16);
#define restore_party_member_mp         ((pfnsub6CBBBF)0x6CBBBF)

typedef bool(*pfnsub7178BF)(u32);
#define check_member_HP_full            ((pfnsub7178BF)0x7178BF)

typedef bool(*pfnsub7178F1)(u32);
#define check_member_MP_full            ((pfnsub7178F1)0x7178F1)

typedef i32(*pfnsub6F4D30)(u32*, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32);
#define set_cursor_data_values          ((pfnsub6F4D30)0x6F4D30)

typedef i32(*pfnsub6C9812)(i32, i32);
#define set_some_transition_data        ((pfnsub6C9812)0x6C9812)

typedef i32(*pfnsub6C6AEE)(i32);
#define set_more_transition_data        ((pfnsub6C6AEE)0x6C6AEE)

typedef i32(*pfnsub6CA32D)(void);
#define save_crystal_handler            ((pfnsub6CA32D)0x6CA32D)

typedef i32(*pfnsub717923)(char*);
#define display_menu_string             ((pfnsub717923)0x717923)

typedef i32(*pfnsub6FA347)(void);
#define sub_6FA347                      ((pfnsub6FA347)0x6FA347)

typedef i32(*pfnsub6FA12F)(i32, i32, i32, i32);
#define sub_6FA12F                      ((pfnsub6FA12F)0x6FA12F)

typedef i16*(*pfnsub6C98A6)(void);
#define sub_6C98A6                      ((pfnsub6C98A6)0x6C98A6)

typedef void(*pfnsub6C497C)(i32, char);
#define sub_6C497C                      ((pfnsub6C497C)0x6C497C)

typedef i32(*pfnsub6C9808)(void);
#define is_input_handling_enabled       ((pfnsub6C9808)0x6C9808)

typedef bool(*pfnsub715026)(i32);
#define knows_all_prereq_limits         ((pfnsub715026)0x715026)

typedef u8(*pfnsub6C545B)(i32);
#define recalculateBaseStats            ((pfnsub6C545B)0x6C545B)

typedef i32(*pfnsub5CB2CC)(u8);
#define recalculateDerivedStats         ((pfnsub5CB2CC)0x5CB2CC)

typedef i32(*pfnsub714FA3)(void);
#define sub_714FA3                      ((pfnsub714FA3)0x714FA3)

typedef u8(*pfnsub6E5C52)(void);
#define setCursorTargetingData          ((pfnsub6E5C52)0x6E5C52)

typedef i32(*pfnsub6D0AF9)(i16, i16, i16);
#define initHandlerCursorState          ((pfnsub6D0AF9)0x6D0AF9)

typedef i32(*pfnsub6C9812)(i32, i32);
#define sub_6C9812                      ((pfnsub6C9812)0x6C9812) //Sets some data for transition from EQUIP to main menu

typedef i32(*pfnsub6C6AEE)(i32);
#define sub_6C6AEE                      ((pfnsub6C6AEE)0x6C6AEE) //Sets some data for transition from EQUIP to main menu

typedef i8(*pfnsub5CB127)(void);   
#define updateMiscPartyStats            ((pfnsub5CB127)0x5CB127)

#endif
