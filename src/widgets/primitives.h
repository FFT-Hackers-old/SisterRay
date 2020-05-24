#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "../EncodedString.h"
#include "base_widget.h"

struct TextWidget_ {
    Widget          widget;
    EncodedString   text;
    color           textColor;
    float           priority;
};

struct NumberWidget_ {
    Widget widget;
    u32 number;
    color numberColor;
    u8 digitCount;
    float priority;
};

struct BoxWidget_ {
    Widget widget;
    u16 x;
    u16 y;
    u16 width;
    u16 height;
    float priority;
    u8 isAlphaEnabled;
    u8 useBoxColors;
    BoxColors boxColors;
};

void drawTextWidget(TextWidget* textWidget);
static const WidgetClass kTextWidgetClass = { (SRWIDGETDRAWPROC)drawTextWidget };
const WidgetClass* TextWidgetKlass();
TextWidget* createTextWidget(DrawTextParams params, std::string name);
bool isTextWidget(Widget* widget);

void drawNumberWidget(NumberWidget* numberWidget);
static const WidgetClass kNumberWidgetClass = { (SRWIDGETDRAWPROC)drawNumberWidget };
const WidgetClass* NumberWidgetKlass();
NumberWidget* createNumberWidget(DrawNumberParams params, std::string name);
bool isNumberWidget(Widget* widget);

void drawBoxWidget(BoxWidget* boxWidget);
const WidgetClass* BoxWidgetKlass();
static const WidgetClass kBoxWidgetClass = { (SRWIDGETDRAWPROC)drawBoxWidget };
BoxWidget* createBoxWidget(DrawBoxParams params, std::string name);
bool isBoxWidget(Widget* widget);

#endif


