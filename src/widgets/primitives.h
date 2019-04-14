#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "base_widget.h"

// Todo: Pixel count to figure out which draw distance is which
#pragma pack(push, 1)
typedef struct {
    i16 drawDistance1;
    i16 drawDistance2;
    i16 drawDistance3;
    i16 drawDistance4;
    float boxFloat;
    void set(i16 drawDistance1Arg, i16 drawDistance2Arg, i16 drawDistance3Arg, i16 drawDistance4Arg, float priorityArg) {
        drawDistance1 = drawDistance1Arg;
        drawDistance2 = drawDistance2Arg;
        drawDistance3 = drawDistance3Arg;
        drawDistance4 = drawDistance4Arg;
        boxFloat = priorityArg;
    }
} drawBoxParams;
#pragma pack(pop)

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    char* stringToDraw;
    color textColor;
    float textThickness;
    void set(i32 xCoordinateArg, i32 yCoordinateArg, char* stringToDrawArg, color textColorArg, float priorityArg) {
        xCoordinate = xCoordinateArg;
        yCoordinate = yCoordinateArg;
        stringToDraw = stringToDrawArg;
        textColor = textColorArg;
        textThickness = priorityArg;
    }
} drawTextParams;

typedef struct {
    i32 xCoordinate;
    i32 yCoordinate;
    u32 numberToDraw;
    u8 charCount;
    color numberColor;
    float numberThickness;
    void set(i32 xCoordinateArg, i32 yCoordinateArg, u32 numberToDrawArg, u8 charCountArg, color numberColorArg, float priorityArg) {
        xCoordinate = xCoordinateArg;
        yCoordinate = yCoordinateArg;
        numberToDraw = numberToDrawArg;
        charCount = charCountArg;
        numberColor = numberColorArg;
        numberThickness = priorityArg;
    }
} drawNumberParams;

typedef struct {
    Widget widget;
    char* text; 
    color textColor;
    float priority;
} TextWidget;

void drawTextWidget(TextWidget* textWidget);
static const WidgetClass kTextWidgetClass = { (SRWIDGETDRAWPROC)drawTextWidget };
const WidgetClass* TextClass();
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


