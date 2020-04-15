#include "primitives.h"
#include "../impl.h"

void drawTextWidget(TextWidget* textWidget){
    const char* text = textWidget->text.str();
    gameDrawString(
        textWidget->widget.xCoordinate,
        textWidget->widget.yCoordinate,
        text,
        textWidget->textColor,
        textWidget->priority
    );
}

SISTERRAY_API void srNewTextWidget(Widget* parent, DrawTextParams params, const char* name) {
    auto strName = std::string(name);
    auto TextWidget = createTextWidget(params, strName);
    addChildWidget(parent, (Widget*)TextWidget, strName);
}

TextWidget* createTextWidget(DrawTextParams params, std::string name) {
    TextWidget* widget = (TextWidget*)createWidget(name, sizeof(TextWidget), &kTextWidgetClass);
    if(params.stringToDraw)
        new (&widget->text) EncodedString(params.stringToDraw);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->textColor = params.textColor;
    widget->priority = params.priority;
    return widget;
}

SISTERRAY_API void setTextParams(DrawTextParams* params, i32 xCoordinate, i32 yCoordinate, const char* stringToDraw, color textColor, float priority) {
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

SISTERRAY_API void updateText(Widget* widgetToUpdate, const char* text) {
    if (isTextWidget(widgetToUpdate)) {
        auto typedPtr = (TextWidget*)widgetToUpdate;
        typedPtr->text = EncodedString(text);
    }
    else {
        srLogWrite("attempting to update TextWidget text property of an invalid Widget type");
    }
}

SISTERRAY_API void updateTextColor(Widget* widgetToUpdate, color textColor) {
    if (isTextWidget(widgetToUpdate)) {
        auto typedPtr = (TextWidget*)widgetToUpdate;
        typedPtr->textColor = textColor;
    }
    else {
        //srLogWrite("attempting to update TextWidget color property of an invalid Widget type");
    }
}

SISTERRAY_API void updateTextPriority(Widget* widgetToUpdate, float priority) {
    if (isTextWidget(widgetToUpdate)) {
        auto typedPtr = (TextWidget*)widgetToUpdate;
        typedPtr->priority = priority;
    }
    else {
        srLogWrite("attempting to update TextWidget priority property of an invalid Widget type");
    }
}

SISTERRAY_API const char* getText(Widget* widgetToUpdate) {
    if (isTextWidget(widgetToUpdate)) {
        auto typedPtr = (TextWidget*)widgetToUpdate;
        return typedPtr->text.str();
    }
    else {
        srLogWrite("attempting to fetch TextWidget text property of an invalid Widget type");
        return nullptr;
    }
}

SISTERRAY_API color getTextColor(Widget* widgetToUpdate) {
    if (isTextWidget(widgetToUpdate)) {
        auto typedPtr = (TextWidget*)widgetToUpdate;
        return typedPtr->textColor;
    }
    else {
        srLogWrite("attempting to fetch TextWidget color property of an invalid Widget type");
        return COLOR_WHITE;
    }
}

SISTERRAY_API float getTextPriority(Widget* widgetToUpdate) {
    if (isTextWidget(widgetToUpdate)) {
        auto typedPtr = (TextWidget*)widgetToUpdate;
        return typedPtr->priority;
    }
    else {
        srLogWrite("attempting to fetch TextWidget priority property of an invalid Widget type");
        return 0.0f;
    }
}

//Begin Number Widget API Implementations
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

SISTERRAY_API void srNewNumberWidget(Widget* parent, DrawNumberParams params, const char* name) {
    auto strName = std::string(name);
    auto NumberWidget = createNumberWidget(params, strName);
    addChildWidget(parent, (Widget*)NumberWidget, strName);
}

NumberWidget* createNumberWidget(DrawNumberParams params, std::string name) {
    NumberWidget* widget = (NumberWidget*)createWidget(name, sizeof(NumberWidget), &kNumberWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->number = params.numberToDraw;
    widget->numberColor = params.numberColor;
    widget->digitCount = params.charCount;
    widget->priority = params.numberThickness;
    return widget;
}

SISTERRAY_API void setNumberParams(DrawNumberParams* params, i32 xCoord, i32 yCoord, u32 number, u8 numChars, color numColor, float priority) {
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

SISTERRAY_API void updateNumber(Widget* widgetToUpdate, u32 number) {
    if (isNumberWidget(widgetToUpdate)) {
        auto typedPtr = (NumberWidget*)widgetToUpdate;
        typedPtr->number = number;
    }
    else {
        srLogWrite("attempting to update NumberWidget number property of an invalid Widget type");
    }
}

SISTERRAY_API void updateNumberColor(Widget* widgetToUpdate, color numberColor) {
    if (isNumberWidget(widgetToUpdate)) {
        auto typedPtr = (NumberWidget*)widgetToUpdate;
        typedPtr->numberColor = numberColor;
    }
    else {
        srLogWrite("attempting to update NumberWidget color property of an invalid Widget type");
    }
}


SISTERRAY_API void updateNumberPriority(Widget* widgetToUpdate, float priority) {
    if (isNumberWidget(widgetToUpdate)) {
        auto typedPtr = (NumberWidget*)widgetToUpdate;
        typedPtr->priority = priority;
    }
    else {
        srLogWrite("attempting to update NumberWidget priority property of an invalid Widget type");
    }
}

SISTERRAY_API u32 getNumber(Widget* widgetToUpdate) {
    if (isNumberWidget(widgetToUpdate)) {
        auto typedPtr = (NumberWidget*)widgetToUpdate;
        return typedPtr->number;
    }
    else {
        srLogWrite("attempting to fetch NumberWidget number property from an invalid Widget type");
        return 0;
    }
}

SISTERRAY_API color getNumberColor(Widget* widgetToUpdate) {
    if (isNumberWidget(widgetToUpdate)) {
        auto typedPtr = (NumberWidget*)widgetToUpdate;
        return typedPtr->numberColor;
    }
    else {

        srLogWrite("attempting to fetch NumberWidget color property from an invalid Widget type");
        return COLOR_WHITE;
    }
}

SISTERRAY_API float getNumberPriority(Widget* widgetToUpdate) {
    if (isNumberWidget(widgetToUpdate)) {
        auto typedPtr = (NumberWidget*)widgetToUpdate;
        return typedPtr->priority;
    }
    else {
        srLogWrite("attempting to fetch NumberWidget priority property from an invalid Widget type");
        return 0.0f;
    }
}


typedef void(*pfnsub6E7D20)(DrawBoxParams*, float);
#define gameDrawBox                      ((pfnsub6E7D20)0x6E7D20)
void drawBoxWidget(BoxWidget* boxWidget) {
    u32* dword_DC3CD8 = (u32*)0xDC3CD8;
    u32 swap = 0;
    u32* dword_91EFC8 = (u32*)0x91EFC8;
    u32* dword_91EFCC = (u32*)0x91EFCC;
    u32* dword_91EFD0 = (u32*)0x91EFD0;
    u32* dword_91EFD4 = (u32*)0x91EFD4;
    BoxColors swapColors;

    DrawBoxParams params = {
        boxWidget->drawDistanceXa,
        boxWidget->drawDistanceXb,
        boxWidget->drawDistanceYa,
        boxWidget->drawDistanceYb
    };
    if (boxWidget->isAlphaEnabled) {
        swap = *dword_DC3CD8;
        *dword_DC3CD8 = 1;
    }
    if (boxWidget->useBoxColors) {
        swapColors = { *dword_91EFC8, *dword_91EFCC, *dword_91EFD0, *dword_91EFD4 };
        *dword_91EFC8 = boxWidget->boxColors.rgba1;
        *dword_91EFCC = boxWidget->boxColors.rgba2;
        *dword_91EFD0 = boxWidget->boxColors.rgba3;
        *dword_91EFD4 = boxWidget->boxColors.rgba4;
    }
    gameDrawBox(&params, boxWidget->priority);
    if (boxWidget->useBoxColors) {
        *dword_91EFC8 = swapColors.rgba1;
        *dword_91EFCC = swapColors.rgba2;
        *dword_91EFD0 = swapColors.rgba3;
        *dword_91EFD4 = swapColors.rgba4;
    }
    if (boxWidget->isAlphaEnabled) {
        *dword_DC3CD8 = swap;
    }
    
}

SISTERRAY_API void srNewBoxWidget(Widget* parent, DrawBoxParams params, const char* name) {
    auto strName = std::string(name);
    auto boxWidget = createBoxWidget(params, strName);
    addChildWidget(parent, (Widget*)boxWidget, strName);
}

BoxWidget* createBoxWidget(DrawBoxParams params, std::string name) {
    BoxWidget* widget = (BoxWidget*)createWidget(name, sizeof(BoxWidget), &kBoxWidgetClass);
    widget->drawDistanceXa = params.drawDistance1;
    widget->drawDistanceXb = params.drawDistance2;
    widget->drawDistanceYa = params.drawDistance3;
    widget->drawDistanceYb = params.drawDistance4;
    widget->priority = params.boxFloat;
    widget->isAlphaEnabled = params.isAlphaEnabled;
    widget->useBoxColors = params.useBoxColors;
    widget->boxColors = params.boxColors;
    return widget;
}


SISTERRAY_API void setBoxParams(DrawBoxParams* params, i16 xPosition, i16 yPosition, u16 width, u16 height, float priority, u8 isAlphaEnabled, u8 useBoxColors, BoxColors boxColors) {
    params->drawDistance1 = xPosition;
    params->drawDistance2 = yPosition;
    params->drawDistance3 = width;
    params->drawDistance4 = height;
    params->boxFloat = priority;
    params->isAlphaEnabled = isAlphaEnabled;
}

bool isBoxWidget(Widget* widget) {
    return (widget->klass == &kBoxWidgetClass);
}

const WidgetClass* BoxWidgetKlass() {
    return &kBoxWidgetClass;
}

SISTERRAY_API void resizeBox(Widget* widgetToUpdate, i16 drawDistance1, i16 drawDistance2, i16 drawDistance3, i16 drawdistance4) {
    if (isBoxWidget(widgetToUpdate)) {
        auto typedPtr = (BoxWidget*)widgetToUpdate;
        typedPtr->drawDistanceXa = drawDistance1;
        typedPtr->drawDistanceXb = drawDistance2;
        typedPtr->drawDistanceYa = drawDistance3;
        typedPtr->drawDistanceYb = drawdistance4;
    }
    else {
        //srLogWrite("attempting to resize a box widget which is not a Box Widget");
    }
}

SISTERRAY_API void updateBoxPriority(Widget* widgetToUpdate, float priority) {
    if (isBoxWidget(widgetToUpdate)) {
        auto typedPtr = (BoxWidget*)widgetToUpdate;
        typedPtr->priority = priority;
    }
    else {
        srLogWrite("attempting update the Box priority property of not a Box Widget");
    }
}

SISTERRAY_API u16 getBoxDrawDistance(Widget* widgetToUpdate, i32 dimension) {
    if (isBoxWidget(widgetToUpdate)) {
        auto typedPtr = (BoxWidget*)widgetToUpdate;
        switch (dimension) {
            case 0:
                return typedPtr->drawDistanceXa;
            case 1:
                return typedPtr->drawDistanceXb;
            case 2:
                return typedPtr->drawDistanceYa;
            case 3:
                return typedPtr->drawDistanceYb;
            default:{
                srLogWrite("attempting to fetch an invalid box dimension");
                return 0xFFFF;
            }
        }
    }
    else {
        srLogWrite("attempting to fetch a Box widget draw distance property of not a Box Widget");
        return 0xFFFF;
    }
}

SISTERRAY_API float getBoxPriority(Widget* widgetToUpdate) {
    if (isBoxWidget(widgetToUpdate)) {
        auto typedPtr = (BoxWidget*)widgetToUpdate;
        return typedPtr->priority;
    }
    else {
        srLogWrite("attempting to fetch the Box priority property of not a Box Widget");
        return 0.0f;
    }
}


