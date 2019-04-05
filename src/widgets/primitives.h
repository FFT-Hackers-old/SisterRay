#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "base_widget.h"
#include "../menus/menu.h"

typedef struct {
    Widget widget;
    char* text; 
    color textColor;
    float priority;
} TextWidget;

void drawTextWidget(TextWidget* textWidget);
const WidgetClass kTextWidgetClass = { (SRWIDGETDRAWPROC)drawTextWidget };
TextWidget* createTextWidget(drawTextParams params, std::string name);
bool isTextWidget(Widget* widget);
void updateText(Widget* widget, char* text);
void updateTextColor(Widget* widget, color textColor);


typedef struct {
    Widget widget;
    u32 number;
    color numberColor;
    u8 digitCount;
    float priority;
} NumberWidget;

void drawNumberWidget(NumberWidget* numberWidget);
const WidgetClass kNumberWidgetClass = { (SRWIDGETDRAWPROC)drawNumberWidget };
NumberWidget* createNumberWidget(drawNumberParams params, std::string name);
bool isNumberWidget(Widget* widget);
void updateNumber(Widget* widget, u32 number);
void updateNumberColor(Widget* widget, color numberColor);

typedef struct {
    Widget widget;
    u16 drawDistanceXa;
    u16 drawDistanceXb;
    u16 drawDistanceYa;
    u16 drawDistanceYb;
    float priority;
} BoxWidget;

void drawBoxWidget(BoxWidget* boxWidget);
const WidgetClass kBoxWidgetClass = { (SRWIDGETDRAWPROC)drawBoxWidget };
BoxWidget* createBoxWidget(drawBoxParams params, std::string name);
bool isBoxWidget(Widget* widget);
#endif


