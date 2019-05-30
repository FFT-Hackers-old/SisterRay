#ifndef SISTER_RAY_H
#define SISTER_RAY_H

#include <stddef.h>
#include <stdint.h>
#include <SisterRay/constants.h>
#include <SisterRay/game_structs.h>
#include <SisterRay/routines.h>
#include <SisterRay/data_addresses.h>
#include <SisterRay/game_data_types.h>
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
    DRAW_BATTLE_SPELL,
    INIT_BATTLE_SPELL,
    DRAW_BATTLE_SUMMON,
    INIT_BATTLE_SUMMON,
    DRAW_BATTLE_ESKILL,
    INIT_BATTLE_ESKILL,
    DRAW_BATTLE_ITEM,
    INIT_BATTLE_ITEM,
    MENU_INPUT_OK,
    MENU_INPUT_CANCEL,
    MENU_INPUT_SQUARE,
    MENU_INPUT_TRIANGLE,
    MENU_INPUT_R1,
    MENU_INPUT_L1,
    MENU_INPUT_RIGHT,
    MENU_INPUT_LEFT,
    ENABLE_ACTIONS,
    APPLY_SUPPORT
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


enum color { COLOR_GRAY, COLOR_UNKNOWN_1, COLOR_RED, COLOR_UNKNOWN_2, COLOR_UNKNOWN_3, COLOR_TEAL, COLOR_GREEN, COLOR_WHITE };
/*SisterRay Menu/Widget API*/

typedef struct Widget_ Widget;
typedef Widget* (*PFNSRGETCHILDPROC)(Widget*, char*);
typedef void(*PFNSRMOVEWIDGETPROC)(Widget*, u32, u32);
typedef void(*PFNSRENABLEWIDGETPROC)(Widget*);
typedef void(*PFNSRDISABLEWIDGETPROC)(Widget*);
typedef i32(*PFNSRGETWIDGETXPROC)(Widget*);
typedef i32(*PFNSRGETWIDGETYPROC)(Widget*);

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    const char* stringToDraw;
    color textColor;
    float priority;
} DrawTextParams;
typedef void(*PFNSETTEXTPARAMSPROC)(DrawTextParams*, i32, i32, char*, color, float);

typedef struct TextWidget_ TextWidget;
typedef void (*PFNSRSETTEXTPARAMSPROC)(DrawTextParams*, i32, i32, const char*, color, float);
typedef void (*PFNSRNEWTEXTWIDGETPROC)(Widget*, DrawTextParams, char*);
typedef void (*PFNSRUPDATETEXTPROC)(Widget*, const char*);
typedef void (*PFNSRUPDATETEXTCOLORPROC)(Widget*, color);
typedef void (*PFNSRUPDATETEXTPRIORITYPROC)(Widget*, float);
typedef const char* (*PFNSRGETEXTPROC)(Widget*);
typedef color (*PFNSRGETTEXTCOLORPROC)(Widget*);
typedef float (*PFNSRGETTEXTPRIORITY)(Widget*);

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    u32 numberToDraw;
    u8 charCount;
    color numberColor;
    float numberThickness;
} DrawNumberParams;
typedef void(*PFNSETNUMBERPARAMSPROC)(DrawNumberParams*, i32, i32, u32, u8, color, float);

typedef struct NumberWidget_ NumberWidget;
typedef void(*PFNSRSETNUMBERPARAMSPROC)(DrawTextParams*, i32, i32, u32, u8, color, float);
typedef void(*PFNSRNEWNUMBERWIDGETPROC)(Widget*, DrawNumberParams, char*);
typedef void(*PFNSRUPDATENUMBERPROC)(Widget*, u32);
typedef void(*PFNSRUPDATENUMBERCOLORPROC)(Widget*, color);
typedef void(*PFNSRUPDATENUMBERPRIORITYPROC)(Widget*, float);
typedef u32* (*PFNSRGENUMBERPROC)(Widget*);
typedef color(*PFNSRGETNUMBERCOLORPROC)(Widget*);
typedef float(*PFNSRGETNUMBERPRIORITY)(Widget*);

#pragma pack(push, 1)
typedef struct {
    i16 drawDistance1;
    i16 drawDistance2;
    i16 drawDistance3;
    i16 drawDistance4;
    float boxFloat;
} DrawBoxParams;
#pragma pack(pop)
typedef void(*PFNSETBOXPARAMSPROC)(DrawBoxParams*, i16, i16, u16, u16, float);

typedef struct BoxWidget_ BoxWidget;
typedef void (*PFNSRNEWBOXWIDGETPROC)(Widget*, DrawBoxParams, char*);
typedef void (*PFNSRSETBOXPARAMSPROC)(DrawBoxParams*, i16, i16, u16, u16, float);
typedef void (*PFNSRRESIZEBOXPROC)(Widget*, i16, i16,i16, i16);
typedef void (*PFNSRUPDATEBOXPRIORITYPROC)(Widget*, float);
typedef i16  (*PFNSRGETBOXDRAWDISTANCEPROC)(Widget*, i32);
typedef float  (*PFNSRGETBOXPRIORITYPROC)(Widget*);

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    u8 partyIndex;
    float priority;
} drawPortraitParams;
typedef void(*PFNPORTRAITPARAMSPROC)(drawPortraitParams*, i32, i32, u8, float);

typedef struct PortraitWidget_ PortraitWidget;
typedef void(*PFNSRNEWPORTRAITWIDGETPROC)(Widget*, drawPortraitParams, char*);
typedef void(*PFNSRSETPORTRAITPARAMSPROC)(drawPortraitParams*, i32, i32, u8, float);
typedef void(*PFNSRUPDATEPORTRAITPARTYINDEXPROC)(Widget*, u8);

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    u8 partyIndex;
    float priority;
} drawHPBarParams;
typedef void(*PFNHPBARPARAMSPROC)(drawHPBarParams*, i32, i32, u8, float);

typedef struct HPBarWidget_ HPBarWidget;
typedef void(*PFNSRNEWHPBARWIDGETPROC)(Widget*, drawHPBarParams, char*);
typedef void(*PFNSRSETHPBARPARAMSPROC)(drawHPBarParams*, i32, i32, u8, float);
typedef void(*PFNUPDATEHPBARPARTYINDEXPROC)(Widget*, u8);

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    u8 arrowCode;
    color arrowColor;
    float arrowPriority;
} DrawSimpleAssetParams;

typedef struct SimpleAssetWidget_ SimpleAssetWidget;
typedef void(*PFNSRNEWARROWWIDGETPROC)(Widget*, DrawSimpleAssetParams, char*);
typedef void(*PFNSRSETARROWPARAMSPROC)(DrawSimpleAssetParams*, i32, i32, u8, color, float);

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    u8* materiaSlotData;
    MateriaInventoryEntry* materiaData;
} drawSlotsParams;

typedef struct SlotsWidget_ SlotsWidget;
typedef void(*PFNSRNEWSLOTSWIDGETPROC)(Widget*, drawSlotsParams, char*);
typedef void(*PFNSRSETSLOTSPARAMSPROC)(drawSlotsParams*, i32, i32, u8*, MateriaInventoryEntry*);
typedef void(*PFNSRUPDATEMATERIASLOTSPROC)(Widget*, u8*);

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    i32 unk1;
    i32 unk2;
    i32 unk3;
    i32 unk4;
    i32 unk5; //not sure, best guess
    i32 unk6;
    i32 unk7;
    float priority;
} DrawGameAssetParams;

typedef struct GameAssetWidget_ GameAssetWidget;
typedef void(*PFNSRNEWGASSETWIDGETPROC)(Widget*, DrawGameAssetParams, char*);
typedef void(*PFNSRSETGASSETPARAMSPROC)(DrawGameAssetParams*, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32);

typedef struct CollectionWidget_ CollectionWidget;

typedef struct _Cursor Cursor;

typedef void(*SRLISTUPDATERPROC)(CollectionWidget*, Widget*, u16);
typedef Widget*(*SRLISTALLOCPROC)(const char*, i32, i32);
typedef struct {
    Cursor* cursor;
    SRLISTUPDATERPROC updater;
    i32 xCoordinate;
    i32 yCoordinate;
    SRLISTALLOCPROC allocator;
} drawGridParams;

typedef struct CursorGridWidget_ CursorGridWidget;
typedef void (*PFNSRNEWGRIDWIDGETPROC)(Widget*, drawGridParams, char*, u16);

typedef struct {
    SRLISTUPDATERPROC updater;
    i32 xCoordinate;
    i32 yCoordinate;
    u16 columnCount;
    u16 rowCount;
    u16 columnSpacing;
    u16 rowSpacing;
    SRLISTALLOCPROC allocator;
} DrawStaticGridParams;

typedef struct StaticGridWidget_ StaticGridWidget;
typedef void(*PFNSNEWSGRIDWIDGETPROC)(Widget*, DrawStaticGridParams, char*, u16);

typedef struct _Menu Menu;
typedef Menu* (*PFNSRGETMENUPROC)(char*);
typedef void(*PFNSRADDSTATEPROC)(Menu*, CursorContext*);
typedef CursorContext* (*PFNSRGETSTATECURSORPROC)(Menu*, i32);
typedef void(*PFNSRSETSTATECURSORPROC)(Menu*, i32, CursorContext*);
typedef i32(*PFNSRGETMENUSTATEPROC)(Menu*);
typedef void(*PFNSRSETMENUSTATEPROC)(Menu*, i32);
typedef Widget* (*PFNSRGETWIDGETPROC)(Menu*);

/* Mog re-exports */
typedef void  (*PFNSRREPLACEFUNCTIONPROC)(void* dst, void* newAddr);
typedef void* (*PFNSRREDIRECTFUNCTIONPROC)(void* dst, void* newAddr);
typedef void  (*PFNSRREPLACESKIPPROC)(void* dst, size_t len);
typedef void  (*PFNSRREPLACENOPPROC)(void* dst, size_t len);
typedef void* (*PFNSRVIRTUALADDRESSPROC)(uint32_t fixedAddr);

#endif
