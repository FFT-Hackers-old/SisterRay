#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "widget.h"
#include "../menus/menu.h"

typedef struct {
    Widget widget;
    char* text; 
    color textColor;
    float priority;
} TextWidget;

static void drawTextWidget(TextWidget* textWidget);
static const WidgetClass kTextWidgetClass = { (SRWIDGETDRAWPROC)drawTextWidget };
TextWidget* createTextWidget(drawTextParams params, std::string name);


typedef struct {
    Widget widget;
    u32 number;
    color numberColor;
    u8 digitCount;
    float priority;
} NumberWidget;

static void drawNumberWidget(NumberWidget* numberWidget);
static const WidgetClass kNumberWidgetClass = { (SRWIDGETDRAWPROC)drawNumberWidget };
NumberWidget* createNumberWidget(drawNumberParams params, std::string name);

typedef struct {
    Widget widget;
    u16 drawDistanceXa;
    u16 drawDistanceXb;
    u16 drawDistanceYa;
    u16 drawDistanceYb;
    float priority;
} BoxWidget;

static void drawBoxWidget(BoxWidget* boxWidget);
static const WidgetClass kBoxWidgetClass = { (SRWIDGETDRAWPROC)drawBoxWidget };
BoxWidget* createBoxWidget(drawBoxParams params, std::string name);
#endif
