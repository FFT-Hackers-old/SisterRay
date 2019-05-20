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
DrawSimpleAssetParams Sign(i32 xCoordinate, i32 yCoordinate, color color, float priority, bool isNegative = false);
DrawSimpleAssetParams Percent(i32 xCoordinate, i32 yCoordinate, color color, float priority);
DrawSimpleAssetParams Arrow(i32 xCoordinate, i32 yCoordinate, color color, float priority);

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
SISTERRAY_API void transformAsset(Widget* widgetToUpdate, i32 type1, i32 type2, i32 type3, i32 type4);
//The following are a set of helper methods for fetching parameters that trigger load a specific game asset
DrawGameAssetParams MateriaSphere(i32 xCoordinate, i32 yCoordinate, i32 sphereColor, float priority);
DrawGameAssetParams MateriaStar(i32 xCoordinate, i32 yCoordinate, i32 sphereColor, float priority, bool shaded);

#endif
