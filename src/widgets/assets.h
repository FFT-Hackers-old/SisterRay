#ifndef ASSETS_H
#define ASSETS_H

#include "base_widget.h"

#pragma pack(push, 1)
typedef struct {
    u16 viewSize;
    u16 maxRows;
    u16 baseRow;
    u16 position1;
    u16 position2;
    u16 position3;
    u16 position4;
    float priority;
    void set(u16 viewSizeArg, u16 maxRowArg, u16 baseRowArg, u16 position1Arg, u16 position2Arg, u16 position3Arg, u16 position4Arg, float priorityArg) {
        viewSize = viewSizeArg;
        maxRows = maxRowArg;
        baseRow = baseRowArg;
        position1 = position1Arg;
        position2 = position2Arg;
        position3 = position3Arg;
        position4 = position4Arg;
        priority = priorityArg;
    }
} drawScrollerParams;
#pragma pack(pop)


typedef struct {
    Widget widget;
    u16 viewSize;
    u16 maxRows;
    u16 baseRows;
    u16 position1;
    u16 position2;
    u16 position3;
    u16 position4;
    float priority;
} ScrollerWidget;

void drawScrollerWidget(ScrollerWidget* scrollerWidget);
const WidgetClass kScrollerWidgetClass = { (SRWIDGETDRAWPROC)drawScrollerWidget };
const WidgetClass* ScrollerWidgetKlass();
ScrollerWidget* createScrollerWidget(drawScrollerParams params, std::string name);
bool isScrollerWidget(Widget* widget);

struct PortraitWidget_ {
    Widget widget;
    u8 partyIndex;
    float priority;
};

void drawPortraitWidget(PortraitWidget* portraitWidget);
const WidgetClass kPortraitWidgetClass = { (SRWIDGETDRAWPROC)drawPortraitWidget };
const WidgetClass* PortraitWidgetKlass();
PortraitWidget* createPortraitWidget(drawPortraitParams params, std::string name);
bool isPortraitWidget(Widget* widget);

struct HPBarWidget_ {
    Widget widget;
    u8 partyIndex;
    float priority;
};

void drawHPBarWidget(HPBarWidget* hpBarWidget);
const WidgetClass kHPBarWidgetClass = { (SRWIDGETDRAWPROC)drawHPBarWidget };
const WidgetClass* HPBarWidgetKlass();
HPBarWidget* createHPBarWidget(drawHPBarParams params, std::string name);
bool isHPBarWidget(Widget* widget);

struct SimpleAssetWidget_ {
    Widget widget;
    u8 code;
    color arrowColor;
    float priority;
};

void drawSimpleAssetWidget(SimpleAssetWidget* arrowWidget);
const WidgetClass kSimpleAssetWidgetClass = { (SRWIDGETDRAWPROC)drawSimpleAssetWidget };
const WidgetClass* SimpleAssetWidgetKlass();
SimpleAssetWidget* createSimpleGameAssetWidget(DrawSimpleAssetParams params, std::string name);
bool isSimpleAssetWidget(Widget* widget);
DrawSimpleAssetParams Sign(i32 xCoordinate, i32 yCoordinate, color color, float priority, bool isPositive = false);
DrawSimpleAssetParams Percent(i32 xCoordinate, i32 yCoordinate, color color, float priority);
DrawSimpleAssetParams Arrow(i32 xCoordinate, i32 yCoordinate, color color, float priority);
DrawSimpleAssetParams Cross(i32 xCoordinate, i32 yCoordinate, color color, float priority);

struct SlotsWidget_ {
    Widget widget;
    u8* materiaSlotsData;
    MateriaInventoryEntry* materiaData;
};

void drawSlotsWidget(SlotsWidget* slotsWidget);
const WidgetClass kSlotsWidgetClass = { (SRWIDGETDRAWPROC)drawSlotsWidget };
const WidgetClass* SlotsWidgetKlass();
SlotsWidget* createSlotsWidget(drawSlotsParams params, std::string name);
bool isSlotsWidget(Widget* widget);

struct BarWidget_ {
    Widget widget;
    i32 length;
    i32 thickness;
    i32 unk2;
    float priority;
};

void drawBarWidget(BarWidget* barWidget);
const WidgetClass kBarWidgetClass = { (SRWIDGETDRAWPROC)drawBarWidget };
const WidgetClass* BarWidgetKlass();
BarWidget* createBarWidget(DrawBarParams params, std::string name);
bool isBarWidget(Widget* widget);
void updateBarLength(BarWidget* barWidget, u32 length);
void updateBarColor(BarWidget* barWidget, i32 colorMask);

struct ResourceBarWidget_ {
    Widget widget;
    i16 length;
    i16 thickness;
    i16 seg1start;
    i16 seg1end;
    i16 seg2start;
    i16 seg2end;
    i32 colorMask;
    float priority;
};

void drawResourceBarWidget(ResourceBarWidget* barWidget);
const WidgetClass kResourceBarWidgetClass = { (SRWIDGETDRAWPROC)drawResourceBarWidget };
const WidgetClass* ResourceBarWidgetKlass();
ResourceBarWidget* createResourceBarWidget(DrawResourceBarParams params, std::string name);
bool isResourceBarWidget(Widget* widget);
void updateResourceBarColor(ResourceBarWidget* barWidget, i32 colorMask);
void updateSegment(ResourceBarWidget* barWidget, i16 start, i16 end, i16 midpoint);

struct GameAssetWidget_ {
    Widget widget;
    i32 unk1;
    i32 unk2;
    i32 unk3;
    i32 unk4;
    i32 unk5;
    i32 unk6;
    i32 unk7;
    float priority;
};

void drawGameAssetWidget(GameAssetWidget* gameAssetWidget);
const WidgetClass kGameAssetWidgetClass = { (SRWIDGETDRAWPROC)drawGameAssetWidget };
const WidgetClass* GameAssetWidgetKlass();
GameAssetWidget* createGameAssetWidget(DrawGameAssetParams params, std::string name);
bool isGameAssetWidget(Widget* widget);
SISTERRAY_API void setGameAssetParams(DrawGameAssetParams* params, i32 xCoordinate, i32 yCoordinate, i32 arg3, i32 arg4, i32 arg5, i32 arg6, i32 arg7, i32 arg8, i32 arg9, float priority);
SISTERRAY_API void updateAssetType(Widget* widgetToUpdate, i32 type);
SISTERRAY_API void transformAsset(Widget* widgetToUpdate, i32 type1Array, i32 type2Array, i32 type3, i32 type4);
//The following are a set of helper methods for fetching parameters that trigger load a specific game asset
DrawGameAssetParams MateriaSphere(i32 xCoordinate, i32 yCoordinate, i32 sphereColor, float priority);
DrawGameAssetParams MateriaStar(i32 xCoordinate, i32 yCoordinate, i32 sphereColor, float priority, bool shaded);
DrawGameAssetParams ItemIcon(i32 xCoordinate, i32 yCoordinate, i32 iconType, float priority);
DrawGameAssetParams AllArrow(i32 xCoordinate, i32 yCoordinate, float priority);
DrawGameAssetParams BarBorder(i32 xCoordinate, i32 yCoordinate, float priority);
DrawGameAssetParams BarrierBarBorder(i32 xCoordinate, i32 yCoordinate, float priority);
DrawGameAssetParams BattleTexts(i32 xCoordinate, i32 yCoordinate, float priority, int type);
void setStarShaded(Widget* widgetToUpdate, bool shaded);
void updateItemIcon(Widget* widgetToUpdate, i32 iconType);

#endif
