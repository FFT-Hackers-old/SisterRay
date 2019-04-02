#ifndef ASSETS_H
#define ASSETS_H

#include "widget.h"
#include "../menus/menu.h"

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
static const WidgetClass kScrollerWidgetClass = { (SRWIDGETDRAWPROC)drawScrollerWidget };
ScrollerWidget* createScrollerWidget(drawScrollerParams params, std::string name);

typedef struct {
    Widget widget;
    u8 partyIndex;
    float priority;
} PortraitWidget;

void drawPortraitWidget(PortraitWidget* portraitWidget);
static const WidgetClass kPortraitWidgetClass = { (SRWIDGETDRAWPROC)drawPortraitWidget };
PortraitWidget* createPortraitWidget(drawPortraitParams params, std::string name);

typedef struct {
    Widget widget;
    u8 partyIndex;
    float priority;
} HPBarWidget;

void drawHPBarWidget(HPBarWidget* hpBarWidget);
static const WidgetClass kHPBarWidgetClass = { (SRWIDGETDRAWPROC)drawHPBarWidget };
HPBarWidget* createHPBarWidget(drawHPBarParams params, std::string name);

typedef struct {
    Widget widget;
    u8 code;
    color arrowColor;
    float priority;
} ArrowWidget;

void drawArrowWidget(ArrowWidget* arrowWidget);
static const WidgetClass kArrowWidgetClass = { (SRWIDGETDRAWPROC)drawArrowWidget };
ArrowWidget* createArrowWidget(drawArrowParams params, std::string name);

typedef struct {
    Widget widget;
    u8* materiaSlotsData;
} SlotsWidget;

void drawSlotsWidget(SlotsWidget* slotsWidget);
static const WidgetClass kSlotsWidgetClass = { (SRWIDGETDRAWPROC)drawSlotsWidget };
SlotsWidget* createSlotsWidget(drawSlotsParams params, std::string name);


void addChildWidget(Widget* widget, SlotsWidget* child_widget, std::string name);
void addChildWidget(Widget* widget, ArrowWidget* child_widget, std::string name);
void addChildWidget(Widget* widget, PortraitWidget* child_widget, std::string name);

#endif
