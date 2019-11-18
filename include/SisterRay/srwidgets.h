#ifndef SR_WIDGETS_H
#define SR_WIDGETS_H

#include <SisterRay/types.h>

#pragma pack(push, 1)
typedef struct {
    u32 relativeColumnIndex;
    u32 relativeRowIndex;
    u32 viewColumnBound;
    u32 viewRowBound;
    u32 baseColumnIndex;
    u32 baseRowIndex;
    u32 maxColumnBound;
    u32 maxRowBound;
    u32 ninth_dword;
    u32 tenth_dword;
    u32 eleventh_dword;
    u32 twelth_dword;
    u32 thirteenth_dword;
    u32 fourteenh_dword;
} CursorContext;
#pragma pack(pop)

typedef struct _Cursor Cursor;
typedef struct _TransitionData TransitionData;
enum color { COLOR_GRAY, COLOR_UNKNOWN_1, COLOR_RED, COLOR_UNKNOWN_2, COLOR_UNKNOWN_3, COLOR_TEAL, COLOR_GREEN, COLOR_WHITE };

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
typedef void(*PFNSRSETTEXTPARAMSPROC)(DrawTextParams*, i32, i32, const char*, color, float);
typedef void(*PFNSRNEWTEXTWIDGETPROC)(Widget*, DrawTextParams, char*);
typedef void(*PFNSRUPDATETEXTPROC)(Widget*, const char*);
typedef void(*PFNSRUPDATETEXTCOLORPROC)(Widget*, color);
typedef void(*PFNSRUPDATETEXTPRIORITYPROC)(Widget*, float);
typedef const char* (*PFNSRGETEXTPROC)(Widget*);
typedef color(*PFNSRGETTEXTCOLORPROC)(Widget*);
typedef float(*PFNSRGETTEXTPRIORITY)(Widget*);


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
typedef void(*PFNSRNEWBOXWIDGETPROC)(Widget*, DrawBoxParams, char*);
typedef void(*PFNSRSETBOXPARAMSPROC)(DrawBoxParams*, i16, i16, u16, u16, float);
typedef void(*PFNSRRESIZEBOXPROC)(Widget*, i16, i16, i16, i16);
typedef void(*PFNSRUPDATEBOXPRIORITYPROC)(Widget*, float);
typedef i16(*PFNSRGETBOXDRAWDISTANCEPROC)(Widget*, i32);
typedef float(*PFNSRGETBOXPRIORITYPROC)(Widget*);


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
    i32 length;
    i32 thickness;
    i32 unk2;
    float priority;
} DrawBarParams;

typedef struct BarWidget_ BarWidget;
typedef void(*PFNSRNEWBARSWIDGETPROC)(Widget*, DrawBarParams, char*);
typedef void(*PFNSRSETBARPARAMSPROC)(BarWidget*, DrawBarParams);

#pragma pack(push, 1)
typedef struct {
    i16 xCoordinate;
    i16 yCoordinate;
    i16 length;
    i16 thickness;
    i16 seg1start;
    i16 seg1end;
    i16 seg2start;
    i16 seg2end;
    float priority;
} DrawResourceBarParams;
#pragma pack(pop)

typedef struct ResourceBarWidget_ ResourceBarWidget;
typedef void(*PFNSRNEWRESOURCEBARSWIDGETPROC)(Widget*, DrawResourceBarParams, char*);
typedef void(*PFNSRSETRESOURCEBARPARAMSPROC)(ResourceBarWidget*, DrawResourceBarParams);


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

//Collection Widgets
typedef struct CollectionWidget_ CollectionWidget;
typedef void(*SRLISTUPDATERPROC)(CollectionWidget*, Widget*, u16);
typedef Widget*(*SRLISTALLOCPROC)(const char*, i32, i32);
typedef struct {
    const char* cursorName;
    u32 menuState;
    SRLISTUPDATERPROC updater;
    i32 xCoordinate;
    i32 yCoordinate;
    SRLISTALLOCPROC allocator;
    u32 cursorIdx;
} drawGridParams;

typedef struct CursorGridWidget_ CursorGridWidget;
typedef void(*PFNSRNEWGRIDWIDGETPROC)(Widget*, drawGridParams, char*, u16);

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

//Menu types
typedef struct _Menu Menu;
typedef Menu* (*PFNSRGETMENUPROC)(char*);
typedef void(*PFNSRADDSTATEPROC)(Menu*, CursorContext*);
typedef CursorContext* (*PFNSRGETSTATECURSORPROC)(Menu*, i32);
typedef void(*PFNSRSETSTATECURSORPROC)(Menu*, i32, CursorContext*);
typedef i32(*PFNSRGETMENUSTATEPROC)(Menu*);
typedef void(*PFNSRSETMENUSTATEPROC)(Menu*, i32);
typedef Widget* (*PFNSRGETWIDGETPROC)(Menu*);

#endif // !SR_WIDGETS_H
