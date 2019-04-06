#include "primitives.h"

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
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->text = strdup(params.stringToDraw);
    widget->textColor = params.textColor;
    widget->priority = params.textThickness;
    return widget;
}

bool isTextWidget(Widget* widget) {
    return (widget->klass == &kTextWidgetClass);
}

void updateText(Widget* widgetToUpdate, char* text) {
    if (isTextWidget) {
        auto typedPtr = (TextWidget*)widgetToUpdate;
        typedPtr->text = text;
    }
    else {
        ///YA DONE MESSED UP
    }
}

void updateTextColor(Widget* widgetToUpdate, color textColor) {
    if (isTextWidget) {
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
        numberWidget->numberColor,
        numberWidget->digitCount,
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

bool isNumberWidget(Widget* widget) {
    return (widget->klass == &kNumberWidgetClass);
}

void updateNumber(Widget* widgetToUpdate, u32 number) {
    if (isNumberWidget) {
        auto typedPtr = (NumberWidget*)widgetToUpdate;
        typedPtr->number = number;
    }
    else {
        ///YA DONE MESSED UP
    }
}

void updateNumberColor(Widget* widgetToUpdate, color numberColor) {
    if (isNumberWidget) {
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

bool isBoxWidget(Widget* widget) {
    return (widget->klass == &kBoxWidgetClass);
}


