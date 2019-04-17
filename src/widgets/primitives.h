#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "base_widget.h"

struct TextWidget_ {
    Widget widget;
    std::string text;
    color textColor;
    float priority;
};

struct NumberWidget_ {
    Widget widget;
    u32 number;
    color numberColor;
    u8 digitCount;
    float priority;
};

typedef struct BoxWidget_ {
    Widget widget;
    u16 drawDistanceXa;
    u16 drawDistanceXb;
    u16 drawDistanceYa;
    u16 drawDistanceYb;
    float priority;
};

void drawTextWidget(TextWidget* textWidget);
static const WidgetClass kTextWidgetClass = { (SRWIDGETDRAWPROC)drawTextWidget };
const WidgetClass* TextWidgetKlass();
TextWidget* createTextWidget(drawTextParams params, std::string name);
bool isTextWidget(Widget* widget);

void drawNumberWidget(NumberWidget* numberWidget);
static const WidgetClass kNumberWidgetClass = { (SRWIDGETDRAWPROC)drawNumberWidget };
const WidgetClass* NumberWidgetKlass();
NumberWidget* createNumberWidget(drawNumberParams params, std::string name);
bool isNumberWidget(Widget* widget);

void drawBoxWidget(BoxWidget* boxWidget);
const WidgetClass* BoxWidgetKlass();
static const WidgetClass kBoxWidgetClass = { (SRWIDGETDRAWPROC)drawBoxWidget };
BoxWidget* createBoxWidget(drawBoxParams params, std::string name);
bool isBoxWidget(Widget* widget);

#endif


