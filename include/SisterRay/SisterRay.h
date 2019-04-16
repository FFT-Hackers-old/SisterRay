#ifndef SISTER_RAY_H
#define SISTER_RAY_H

#include <stddef.h>
#include <stdint.h>
#include <SisterRay/constants.h>
#include <SisterRay/game_structs.h>
#include <SisterRay/routines.h>
#include <SisterRay/data_addresses.h>

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
    EQUIP_MENU_INPUT_OK,
    EQUIP_MENU_INPUT_CANCEL,
    EQUIP_MENU_INPUT_SQUARE,
    EQUIP_MENU_INPUT_TRIANGLE,
    EQUIP_MENU_INPUT_R1,
    EQUIP_MENU_INPUT_L1
} SrEventType;

typedef u32 SrEventListener;
typedef void (*SrEventCallback)(const void*);

/* SisterRay API */
typedef SrEventListener (PFNSRADDLISTENERPROC)(SrEventType eventType, SrEventCallback callback);

typedef const void* (*PFNSRLOADFUNCTIONPROC)(const char*);
typedef const void* (*PFNSRREGISTERFUNCTIONPROC)(const char*, const void*);


enum color { COLOR_GRAY, COLOR_UNKNOWN_1, COLOR_RED, COLOR_UNKNOWN_2, COLOR_UNKNOWN_3, COLOR_TEAL, COLOR_GREEN, COLOR_WHITE };
/*SisterRay Menu/Widget API*/
typedef struct _Menu Menu;

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
} drawTextParams;
typedef void(*PFNSETTEXTPARAMSPROC)(drawTextParams*, i32, i32, char*, color, float);

typedef struct TextWidget_ TextWidget;
typedef void (*PFNSRSETTEXTPARAMSPROC)(drawTextParams*, i32, i32, const char*, color, float);
typedef void (*PFNSRNEWTEXTWIDGETPROC)(Widget*, drawTextParams, char*);
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
} drawNumberParams;
typedef void(*PFNSETNUMBERPARAMSPROC)(drawNumberParams*, i32, i32, u32, u8, color, float);

typedef struct NumberWidget_ NumberWidget;
typedef void(*PFNSRSETNUMBERPARAMSPROC)(drawTextParams*, i32, i32, u32, u8, color, float);
typedef void(*PFNSRNEWNUMBERWIDGETPROC)(Widget*, drawNumberParams, char*);
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
} drawBoxParams;
#pragma pack(pop)
typedef void(*PFNSETBOXPARAMSPROC)(drawBoxParams*, i16, i16, u16, u16, float);

typedef struct BoxWidget_ BoxWidget;
typedef void (*PFNSRNEWBOXWIDGETPROC)(Widget*, drawBoxParams, char*);
typedef void (*PFNSRSETBOXPARAMSPROC)(drawBoxParams*, i16, i16, u16, u16, float);
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
} drawArrowParams;
typedef void(*PFNARROWPARAMSPROC)(drawArrowParams*, i32, i32, u8, color, float);

typedef struct ArrowWidget_ ArrowWidget;
typedef void(*PFNSRNEWARROWWIDGETPROC)(Widget*, drawArrowParams, char*);
typedef void(*PFNSRSETARROWPARAMSPROC)(drawArrowParams*, i32, i32, u8, color, float);

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    u8* materiaSlotData;
} drawSlotsParams;
typedef void(*PFNSLOTSPARAMSPROC)(drawSlotsParams*, i32, i32, u8*);

typedef struct SlotsWidget_ SlotsWidget;
typedef void(*PFNSRNEWSLOTSWIDGETPROC)(Widget*, drawSlotsParams, char*);
typedef void(*PFNSRSETSLOTSPARAMSPROC)(drawSlotsParams*, i32, i32, u8*);
typedef void(*PFNSRUPDATEMATERIASLOTSPROC)(Widget*, u8*);

typedef struct CollectionWidget_ CollectionWidget;

typedef void(*SRLISTUPDATERPROC)(CollectionWidget*, Widget*, u16);
typedef struct {
    CursorContext* cursor;
    SRLISTUPDATERPROC updater;
    u32 xCoordinate;
    u32 yCoordinate;
    u16 rowSpacing;
    u16 columnSpacing;
} drawGridParams;

typedef struct CursorGridWidget_ CursorGridWidget;
typedef void (*PFNSRNEWGRIDWIDGETPROC)(Widget*, drawGridParams, char*, u16);

/* Mog re-exports */
typedef void  (*PFNSRREPLACEFUNCTIONPROC)(void* dst, void* newAddr);
typedef void* (*PFNSRREDIRECTFUNCTIONPROC)(void* dst, void* newAddr);
typedef void  (*PFNSRREPLACESKIPPROC)(void* dst, size_t len);
typedef void  (*PFNSRREPLACENOPPROC)(void* dst, size_t len);
typedef void* (*PFNSRVIRTUALADDRESSPROC)(uint32_t fixedAddr);

#endif
