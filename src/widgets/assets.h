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

static void drawScrollerWidget(ScrollerWidget* scrollerWidget);
static const WidgetClass kScrollerWidgetClass = { (SRWIDGETDRAWPROC)drawScrollerWidget };
ScrollerWidget* createScrollerWidget(drawScrollerParams params, std::string name);

typedef struct {
    Widget widget;
    u8 partyIndex;
    float priority;
} PortraitWidget;

static void drawPortraitWidget(PortraitWidget* portraitWidget);
static const WidgetClass kPortraitWidgetClass = { (SRWIDGETDRAWPROC)drawPortraitWidget };
PortraitWidget* createPortraitWidget(drawPortraitParams params, std::string name);

typedef struct {
    Widget widget;
    u8 partyIndex;
    float priority;
} HPBarWidget;

static void drawHPBarWidget(HPBarWidget* hpBarWidget);
static const WidgetClass kHPBarWidgetClass = { (SRWIDGETDRAWPROC)drawHPBarWidget };
HPBarWidget* createPortraitWidget(drawHPBarParams params, std::string name);


#endif
