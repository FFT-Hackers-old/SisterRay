#ifndef SISTER_RAY_H
#define SISTER_RAY_H

#include <stddef.h>
#include <stdint.h>
#include <SisterRay/constants.h>
#include <SisterRay/game_structs.h>
#include <SisterRay/routines.h>
#include <SisterRay/data_addresses.h>
#include <SisterRay/game_data_types.h>
#include <SisterRay/srwidgets.h>
#include <SisterRay/srparty.h>
#include <SisterRay/scene_data.h>

/*Macro for handling compilation in both C and C++*/
#if defined(__cplusplus)
# define SISTERRAY_API extern "C"
#else
# define SISTERRAY_API
#endif

#if defined(__cplusplus)
# define SISTERRAY_GLOBAL extern "C"
#else
# define SISTERRAY_GLOBAL extern
#endif

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof(*x))

#define BASE_PREFIX     "srFF7"

typedef enum {
    NO_TYPE,
    INIT_EQUIP_MENU,
    DRAW_EQUIP_MENU,
    INIT_INVENTORY_MENU,
    DRAW_INVENTORY_MENU,
    INIT_MATERIA_MENU,
    DRAW_MATERIA_MENU,
    DRAW_MATERIA_DATA, //Dispatches on keyed subtypes
    INIT_BATTLE_MENU,
    DRAW_BATTLE_MENU,
    DRAW_BATTLE_COMMAND,
    INIT_BATTLE_COMMAND,
    DRAW_BATTLE_SPELL,
    INIT_BATTLE_SPELL,
    DRAW_BATTLE_SUMMON,
    INIT_BATTLE_SUMMON,
    DRAW_BATTLE_ESKILL,
    INIT_BATTLE_ESKILL,
    DRAW_BATTLE_ITEM,
    INIT_BATTLE_ITEM,
    INIT_BATTLE_CHANGE,
    MENU_INPUT_OK,
    MENU_INPUT_CANCEL,
    MENU_INPUT_SQUARE,
    MENU_INPUT_TRIANGLE,
    MENU_INPUT_R1,
    MENU_INPUT_L1,
    MENU_INPUT_RIGHT,
    MENU_INPUT_LEFT,
    ENABLE_ACTIONS,
    APPLY_SUPPORT,
    ACTION_PRE_COMMAND,
    ACTION_POST_COMMAND
} SrEventType;


typedef enum {
    INVALID_CONTEXT,
    EQUIP_MENU_CONTEXT,
    INVENTORY_MENU_CONTEXT,
    MATERIA_MENU_CONTEXT,
    BATTLE_MENU,
    /*Battle menu is much more modular, leading to separate contexts for each view -- at least for now*/
    BATTLE_SPELL_VIEW,
    BATTLE_SUMMON_VIEW,
    BATTLE_ESKILL_VIEW,
    BATTLE_ITEM_VIEW,
    BATTLE_CHANGE_VIEW,
    BATTLE_COMMAND_VIEW,
    MAT_HIGH_TYPE_0,
    MAT_HIGH_TYPE_1,
    MAT_HIGH_TYPE_2,
    MAT_HIGH_TYPE_3,
    MAT_HIGH_TYPE_4,
    MAT_HIGH_TYPE_5,
    MAT_HIGH_TYPE_6,
    MAT_HIGH_TYPE_7,
    MAT_HIGH_TYPE_8,
    MAT_HIGH_TYPE_9,
    MAT_HIGH_TYPE_10,
    MAT_HIGH_TYPE_11,
    MAT_HIGH_TYPE_12,
    MAT_HIGH_TYPE_13,
    MAT_HIGH_TYPE_14,
    MAT_HIGH_TYPE_15,
    MAT_SUB_TYPE_0,
    MAT_SUB_TYPE_1,
    MAT_SUB_TYPE_2,
    MAT_SUB_TYPE_3,
    MAT_SUB_TYPE_4,
    MAT_SUB_TYPE_5,
    MAT_SUB_TYPE_6,
    MAT_SUB_TYPE_7,
    MAT_SUB_TYPE_8,
    MAT_SUB_TYPE_9,
    MAT_SUB_TYPE_10,
    MAT_SUB_TYPE_11,
    MAT_SUB_TYPE_12,
    MAT_SUB_TYPE_13,
    MAT_SUB_TYPE_14,
    MAT_SUB_TYPE_15,
} SrEventContext;

typedef struct SrEventListener_ SrEventListener;
typedef void (*SrEventCallback)(const void*);

/* SisterRay API */
typedef SrEventListener (PFNSRADDLISTENERPROC)(SrEventType eventType, SrEventCallback callback);

typedef const void* (*PFNSRLOADFUNCTIONPROC)(const char*);
typedef const void* (*PFNSRREGISTERFUNCTIONPROC)(const char*, const void*);

/* Mog re-exports */
typedef void  (*PFNSRREPLACEFUNCTIONPROC)(void* dst, void* newAddr);
typedef void* (*PFNSRREDIRECTFUNCTIONPROC)(void* dst, void* newAddr);
typedef void  (*PFNSRREPLACESKIPPROC)(void* dst, size_t len);
typedef void  (*PFNSRREPLACENOPPROC)(void* dst, size_t len);
typedef void* (*PFNSRVIRTUALADDRESSPROC)(uint32_t fixedAddr);

#endif
