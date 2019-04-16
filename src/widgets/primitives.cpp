#include "primitives.h"
#include "../impl.h"

void drawTextWidget(TextWidget* textWidget){
    gameDrawString(
        textWidget->widget.xCoordinate,
        textWidget->widget.yCoordinate,
        textWidget->text,
        textWidget->textColor,
        textWidget->priority
    );
}

TextWidget* createTextWidget(drawTextParams params, std::string name) {
    TextWidget* widget = (TextWidget*)createWidget(name, sizeof(TextWidget), &kTextWidgetClass);
    srLogWrite("text widget class: %p", &kTextWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->text = params.stringToDraw;
    widget->textColor = params.textColor;
    widget->priority = params.priority;
    return widget;
}

SISTERRAY_API void setTextParams(drawTextParams* params, i32 xCoordinate, i32 yCoordinate, char* stringToDraw, color textColor, float priority) {
    params->xCoordinate = xCoordinate;
    params->yCoordinate = yCoordinate;
    params->stringToDraw = stringToDraw;
    params->textColor = textColor;
    params->priority = priority;
}

bool isTextWidget(Widget* widget) {
    return (widget->klass == &kTextWidgetClass);
}

const WidgetClass* TextWidgetKlass() {
    return &kTextWidgetClass;
}

void updateText(Widget* widgetToUpdate, char* text) {
    if (isTextWidget(widgetToUpdate)) {
        auto typedPtr = (TextWidget*)widgetToUpdate;
        typedPtr->text = text;
    }
    else {
        ///YA DONE MESSED UP
    }
}

void updateTextColor(Widget* widgetToUpdate, color textColor) {
    if (isTextWidget(widgetToUpdate)) {
        auto typedPtr = (TextWidget*)widgetToUpdate;
        typedPtr->textColor = textColor;
    }
    else {
        //YA DONE MESSED UP
    }
}

void drawNumberWidget(NumberWidget* numberWidget) {
    gameDrawNumbers(
        numberWidget->widget.xCoordinate,
        numberWidget->widget.yCoordinate,
        numberWidget->number,
        numberWidget->digitCount,
        numberWidget->numberColor,
        numberWidget->priority
    );
}

NumberWidget* createNumberWidget(drawNumberParams params, std::string name) {
    NumberWidget* widget = (NumberWidget*)createWidget(name, sizeof(NumberWidget), &kNumberWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->number = params.numberToDraw;
    widget->numberColor = params.numberColor;
    widget->digitCount = params.charCount;
    widget->priority = params.numberThickness;
    return widget;
}

SISTERRAY_API void setNumberParams(drawNumberParams* params, i32 xCoord, i32 yCoord, u32 number, u8 numChars, color numColor, float priority) {
    params->xCoordinate = xCoord;
    params->yCoordinate = yCoord;
    params->numberToDraw = number;
    params->charCount = numChars;
    params->numberColor = numColor;
    params->numberThickness = priority;
}

bool isNumberWidget(Widget* widget) {
    return (widget->klass == &kNumberWidgetClass);
}

const WidgetClass* NumberWidgetKlass() {
    return &kNumberWidgetClass;
}

void updateNumber(Widget* widgetToUpdate, u32 number) {
    if (isNumberWidget(widgetToUpdate)) {
        auto typedPtr = (NumberWidget*)widgetToUpdate;
        typedPtr->number = number;
    }
    else {
        ///YA DONE MESSED UP
    }
}

void updateNumberColor(Widget* widgetToUpdate, color numberColor) {
    if (isNumberWidget(widgetToUpdate)) {
        auto typedPtr = (NumberWidget*)widgetToUpdate;
        typedPtr->numberColor = numberColor;
    }
    else {
        //YA DONE MESSED UP
    }
}

void drawBoxWidget(BoxWidget* boxWidget) {
    drawBoxParams params = {
        boxWidget->drawDistanceXa,
        boxWidget->drawDistanceXb,
        boxWidget->drawDistanceYa,
        boxWidget->drawDistanceYb
    };
    gameDrawBox((i16*)&params, boxWidget->priority);
}

BoxWidget* createBoxWidget(drawBoxParams params, std::string name) {
    BoxWidget* widget = (BoxWidget*)createWidget(name, sizeof(BoxWidget), &kBoxWidgetClass);
    widget->drawDistanceXa = params.drawDistance1;
    widget->drawDistanceXb = params.drawDistance2;
    widget->drawDistanceYa = params.drawDistance3;
    widget->drawDistanceYb = params.drawDistance4;
    widget->priority = params.boxFloat;
    return widget;
}


SISTERRAY_API void setBoxParams(drawBoxParams* params, i16 drawDistance1, i16 drawDistance2, u16 drawDistance3, u16 drawDistance4, float priority) {
    params->drawDistance1 = drawDistance1;
    params->drawDistance2 = drawDistance2;
    params->drawDistance3 = drawDistance3;
    params->drawDistance4 = drawDistance4;
    params->boxFloat = priority;
}

bool isBoxWidget(Widget* widget) {
    return (widget->klass == &kBoxWidgetClass);
}

const WidgetClass* BoxWidgetKlass() {
    return &kBoxWidgetClass;
}


