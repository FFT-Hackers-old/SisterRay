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
    i32 xCoordinate;
    i32 yCoordinate;
    u8 partyIndex;
    float priority;
} drawPortraitParams;

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    u8 arrowCode;
    color arrowColor;
    float arrowPriority;
} drawArrowParams;

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    u8* materiaSlotData;
} drawSlotsParams;

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    u8 partyIndex;
    float priority;
} drawHPBarParams;

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
ScrollerWidget* createScrollerWidget(drawScrollerParams params, std::string name);
bool isScrollerWidget(Widget* widget);

typedef struct {
    Widget widget;
    u8 partyIndex;
    float priority;
} PortraitWidget;

void drawPortraitWidget(PortraitWidget* portraitWidget);
const WidgetClass kPortraitWidgetClass = { (SRWIDGETDRAWPROC)drawPortraitWidget };
PortraitWidget* createPortraitWidget(drawPortraitParams params, std::string name);
bool isPortraitWidget(Widget* widget);
void updatePortraitPartyIndex(Widget* widget, u8 portraitIndex);

typedef struct {
    Widget widget;
    u8 partyIndex;
    float priority;
} HPBarWidget;

void drawHPBarWidget(HPBarWidget* hpBarWidget);
const WidgetClass kHPBarWidgetClass = { (SRWIDGETDRAWPROC)drawHPBarWidget };
HPBarWidget* createHPBarWidget(drawHPBarParams params, std::string name);
bool isHPBarWidget(Widget* widget);
void updateHPBarPartyIndex(Widget* widget, u8 portraitIndex);

typedef struct {
    Widget widget;
    u8 code;
    color arrowColor;
    float priority;
} ArrowWidget;

void drawArrowWidget(ArrowWidget* arrowWidget);
const WidgetClass kArrowWidgetClass = { (SRWIDGETDRAWPROC)drawArrowWidget };
ArrowWidget* createArrowWidget(drawArrowParams params, std::string name);
bool isArrowWidget(Widget* widget);

typedef struct {
    Widget widget;
    u8* materiaSlotsData;
} SlotsWidget;

void drawSlotsWidget(SlotsWidget* slotsWidget);
const WidgetClass kSlotsWidgetClass = { (SRWIDGETDRAWPROC)drawSlotsWidget };
SlotsWidget* createSlotsWidget(drawSlotsParams params, std::string name);
bool isSlotsWidget(Widget* widget);
void updateMateriaSlots(Widget* widget, u8* materiaSlotsData);

#endif
