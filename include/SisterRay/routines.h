#ifndef SISTER_RAY_ROUTINES_H
#define SISTER_RAY_ROUTINES_H

#include <SisterRay/types.h>
#include "game_module.h"

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
#define EQUIP_MENU_UPDATE_HANDLER       ((void*)0x705D16)
#define LOAD_ABILITY_DATA_HANDLER       ((void*)0x5C94D3)
#define LOAD_FORMATION_HANDLER          ((void*)0x5D1050)
#define EXECUTE_AI_SCRIPT_HANDLER       ((void*)0x5C86E0)
#define EXECUTE_FORMATION_SCRIPT_HANDLER ((void*)0x5C8931)
#define ENQUEUE_SCRIPT_ACTION           ((void*)0x5D969C)
#define TRANSFORM_ENEMY_COMMAND         ((void*)0x5C93A1)
#define GET_MP_COST                     ((void*)0x5D975B)
#define MAT_MATERIA_HANDLER             ((void*)0x70CF0B)
#define DISPATCH_AUTO_ACTIONS           ((void*)0x5C8CFA)
#define RECALCULATE_DERIVED_STATS       ((void*)0x5CB2CC)
#define UPDATE_COMMANDS_ACTIVE          ((void*)0x437679)
#define DISPATCH_AUTO_ACTIONS           ((void*)0x5C8CFA)
#define DISPATCH_BATTLE_UPDATES         ((void*)0x6CE8B3)
#define INIT_BATTLE_DATA                ((void*)0x6DB716)

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

typedef i32(*pfnsub6F535C)(i32, i32);
#define captureDirectionInput           ((pfnsub6F535C)0x6F535C)

typedef i32(*pfnsub6F7BFB)(i32, i32, i32, i32, i32, i32, i32, i32, i32, float);
#define drawComplexGameAsset          ((pfnsub6F7BFB)0x6F7BFB)

typedef void(*pfnsub6F7346)(i32, i32, i32, i32, i32, float);
#define gameDrawBar          ((pfnsub6F7346)0x6F7346)

/*These are routines used in the inventory menu handler*/
typedef i32(*pfnsub6F0D7D)(i32, i32, float);
#define sub_6F0D7D                      ((pfnsub6F0D7D)0x6F0D7D)

typedef i32(*pfnsub6FAB2F)(i32, i32, const u32, u8, float);
#define sub_6FAB2F                      ((pfnsub6FAB2F)0x6FAB2F)

typedef void**(*pfnsub41963C)(u32, i32, i32);
#define load_kernel_object_text         ((pfnsub41963C)0x41963C)

typedef i32(*pfnsub6EB3B8)(i32, i32, float);
#define displayCursor                  ((pfnsub6EB3B8)0x6EB3B8)

typedef i32(*pfnsub6F5B03)(i32, i32, const char*, u8, float); //The last argument might be a float, not sure yet
#define gameDrawString        ((pfnsub6F5B03)0x6F5B03)

typedef i32(*pfnsub7150A0)(u16);
#define item_is_usable                  ((pfnsub7150A0)0x7150A0)

typedef i32(*pfnsub6F7270)(i32, float);
#define renderSideScroller                      ((pfnsub6F7270)0x6F7270)

typedef i16(*pfnsub6C62A2)(i32, i32, i32, i32);
#define renderHPAndStatus        ((pfnsub6C62A2)0x6C62A2)

/*This function sets some structs of size 0x70 based on the party member ID, not sure what they are yet*/
typedef u8*(*pfnsub6E6C5B)(i32, i32, i32, i32);
#define displayPortrait                      ((pfnsub6E6C5B)0x6E6C5B)

typedef u16*(*pfnsub6E7BA1)(u16*, i16, i16, i16, i16);
#define initialize_menu_window_struct          ((pfnsub6E7BA1)0x6E7BA1)

typedef i32(*pfnsub6E7D20)(i16*, float);
#define gameDrawBox                      ((pfnsub6E7D20)0x6E7D20)

/*This function sets a variable based on the icon type neede in the menu and then calls a function to display it*/
typedef i32(*pfnsub717691)(i32, i32, i32, i32, i32);
#define displayVisibleItemIcon           ((pfnsub717691)0x717691)

/*Not sure what this function does*/
typedef u8(*pfnsub6F5C0C)(i32, i32, i8, i8, i32);
#define gameDrawAsset                   ((pfnsub6F5C0C)0x6F5C0C)

typedef i32(*pfnsub6F9739)(i32, i32, u32, u8, u8, float);
#define gameDrawNumbers                 ((pfnsub6F9739)0x6F9739)

typedef i32(*pfnsub41AB74)(i32);
#define checkInputReceived2             ((pfnsub41AB74)0x41AB74)

typedef i32(*pfnsub6F53F1)(i32);
#define checkInputReceived              ((pfnsub6F53F1)0x6F53F1)

typedef void(*pfnsub74580A)(i32);
#define playMenuSound                   ((pfnsub74580A)0x74580A)

typedef void(*pfnsub6F4DB2)(u32*);
#define handleCursorPositionUpdate      ((pfnsub6F4DB2)0x6F4DB2)

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

typedef i32(*pfnsub717923)(const char*);
#define displayNewBoxString             ((pfnsub717923)0x717923)

typedef i32(*pfnsub6FA347)(void);
#define sub_6FA347                      ((pfnsub6FA347)0x6FA347)

typedef i32(*pfnsub6FA12F)(i32, i32, i32, i32);
#define sub_6FA12F                      ((pfnsub6FA12F)0x6FA12F)

typedef i16*(*pfnsub6C98A6)(void);
#define sub_6C98A6                      ((pfnsub6C98A6)0x6C98A6)

typedef void(*pfnsub6C497C)(i32, char);
#define sub_6C497C                      ((pfnsub6C497C)0x6C497C)

typedef i32(*pfnsub6C9808)(void);
#define checkMenuInputEnabled       ((pfnsub6C9808)0x6C9808)

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
#define setActiveMenu                      ((pfnsub6C6AEE)0x6C6AEE) //Sets some data for transition from EQUIP to main menu

typedef i8(*pfnsub5CB127)(void);   
#define updateMiscPartyStats            ((pfnsub5CB127)0x5CB127)

typedef i32(*pfnsub6CBCF3)(i8);
#define returnMateriaType               ((pfnsub6CBCF3)0x6CBCF3)

typedef i32(*pfnsub6CC2C9)(i8);
#define sub_6CC2C9                      ((pfnsub6CC2C9)0x6CC2C9)

typedef i32(*pfnsub707891)(i32, i32);
#define displayGearStats                ((pfnsub707891)0x707891)

typedef i32(*pfnsub70760F)(i32, i32, i32);
#define renderMateriaSlots              ((pfnsub70760F)0x70760F)

typedef i32(*pfnsub70E831)(i32, i32, i32, i32);
#define renderOccupiedSlots             ((pfnsub70E831)0x70E831)

typedef i32(*pfnsub6F54A2)(u8*);
#define sub_6F54A2                      ((pfnsub6F54A2)0x6F54A2) //Does returns x position from a text ptr, probably a centering util

//The following functions are used by the battle module
typedef void(*pfnsub5CA722)(i32, i32);
#define copyAdditionalEffects           ((pfnsub5CA722)0x5CA722)

typedef void(*pfnsub435139)(i32, u8, u8, i16);
#define srCreateEvent                   ((pfnsub435139)0x435139)

typedef i32(*pfnsub5C8684)(u32, u32);
#define getLimitRelativeIndex           ((pfnsub5C8684)0x5C8684)

typedef i32(*pfnsub5D7FE4)(i32, i32, i32);
#define runAIScript              ((pfnsub5D7FE4)0x5D7FE4)

typedef u8(*pfnsub5C8BA1)(void);
#define getRandomNumber          ((pfnsub5C8BA1)0x5C8BA1)

typedef i32(*pfnsub5D9550)(i32, i32, i32);
#define sub_5D9550               ((pfnsub5D9550)0x5D9550)

typedef i32(*pfnsub43258A)(u8*);
#define enqueueBattleAction      ((pfnsub43258A)0x43258A)

typedef u8*(*pfnsub436E15)(u8, u8, u8, i16);
#define createAnimationEvent     ((pfnsub436E15)0x436E15)

typedef i32(*pfnsub437185)(i32, i32, u8, i32);
#define sub_437185               ((pfnsub437185)0x437185)

typedef i16(*pfnsub437506)(void);
#define getActorCycleTop         ((pfnsub437506)0x437506)


typedef void(*pfnsub43753A)(void);
#define cycleActors              ((pfnsub437506)0x43758A)

typedef void(*pfnsub6CE882)(i16);
#define sub_6CE882   ((pfnsub6CE882)0x6CE882)

typedef void(*pfnsub6D1B5C)(void*, i16);
#define gameDrawTargetingCursors     ((pfnsub6D1B5C)0x6D1B5C)


typedef GameContext*(*pfnsub676578)(void);
#define getGraphicsCtx             ((pfnsub676578)0x676578)

typedef void(*pfnsub41A21E)(void*);
#define sub_41A21E                 ((pfnsub41A21E)0x41A21E)

typedef void(*pfnsub4320AC)(void);
#define incrementTimers            ((pfnsub4320AC)0x4320AC)

typedef void(*pfnsub6E59BC)(u8);
#define sub_6E59BC                 ((pfnsub6E59BC)0x6E59BC)

typedef void(*pfnsub6F63FC)(void*, float);
#define gameDrawResourceBar        ((pfnsub6F63FC)0x6F638C)

typedef void(*pfnsub432AA2)(u16, u16, u16, u16);
#define gameIssueChosenAction     ((pfnsub432AA2)0x432AA2)

typedef void(*pfnsub66067A)(u32, u32, u32, u32, void*);
#define setViewport               ((pfnsub66067A)0x66067A)

typedef void(*pfnsub6DC1EB)(void);
#define runContextCallbacks       ((pfnsub6DC1EB)0x6DC1EB)

typedef u32(*pfnsub41AB67)(u32);
#define sub_41AB67                ((pfnsub41AB67)0x41AB67)

// const pfnsub6E59BC sub_deez_nuts = ((pfnsub6E59BC)0x6E59BC);
#endif
