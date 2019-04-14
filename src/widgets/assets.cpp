#include "assets.h"
#include "../impl.h"

void drawScrollerWidget(ScrollerWidget* scrollerPortrait) {
    drawScrollerParams params = {
        scrollerPortrait->viewSize,
        scrollerPortrait->maxRows,
        scrollerPortrait->baseRows,
        scrollerPortrait->position1,
        scrollerPortrait->position2,
        scrollerPortrait->position3,
        scrollerPortrait->position4
    };
    renderSideScroller((i32)&params, scrollerPortrait->priority);
}

ScrollerWidget* createScrollerWidget(drawScrollerParams params, std::string name) {
    ScrollerWidget* widget = (ScrollerWidget*)createWidget(name, sizeof(ScrollerWidget), &kScrollerWidgetClass);
    widget->viewSize = params.viewSize;
    widget->baseRows = params.baseRow;
    widget->maxRows = params.maxRows;
    widget->position1 = params.position1;
    widget->position2 = params.position2;
    widget->position3 = params.position3;
    widget->position4 = params.position4;
    return widget;
}

bool isScrollerWidget(Widget* widget) {
    return ((widget->klass == &kScrollerWidgetClass));
}

const WidgetClass* ScrollerWidgetKlass() {
    return &kScrollerWidgetClass;
}

void drawPortraitWidget(PortraitWidget* portraitWidget) {
    displayPortrait(
        portraitWidget->widget.xCoordinate,
        portraitWidget->widget.yCoordinate,
        portraitWidget->partyIndex,
        portraitWidget->priority
    );
}

PortraitWidget* createPortraitWidget(drawPortraitParams params, std::string name) {
    PortraitWidget* widget = (PortraitWidget*)createWidget(name, sizeof(PortraitWidget), &kPortraitWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->partyIndex = params.partyIndex;
    widget->priority = params.priority;
    return widget;
}

bool isPortraitWidget(Widget* widget) {
    return ((widget->klass == &kPortraitWidgetClass));
}

const WidgetClass* PortraitWidgetKlass() {
    return &kPortraitWidgetClass;
}

void updatePortraitPartyIndex(Widget* widget, u8 portraitIndex) {
    if (isPortraitWidget(widget)) {
        auto typedPtr = (PortraitWidget*)widget;
        typedPtr->partyIndex = portraitIndex;
    }
    else {
        //YA DONE MESSED UP
    }
}


void drawHPBarWidget(HPBarWidget* hpBarWidget) {
    renderHPAndStatus(
        hpBarWidget->widget.xCoordinate,
        hpBarWidget->widget.yCoordinate,
        hpBarWidget->partyIndex,
        hpBarWidget->priority
    );
}

HPBarWidget* createHPBarWidget(drawHPBarParams params, std::string name) {
    HPBarWidget* widget = (HPBarWidget*)createWidget(name, sizeof(HPBarWidget), &kHPBarWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->partyIndex = params.partyIndex;
    widget->priority = params.priority;
    return widget;
}

const WidgetClass* HPBarWidgetKlass() {
    return &kHPBarWidgetClass;
}

bool isHPBarWidget(Widget* widget) {
    return ((widget->klass == &kHPBarWidgetClass));
}

void updateHPBarPartyIndex(Widget* widget, u8 partyIndex) {
    if (isHPBarWidget(widget)) {
        auto typedPtr = (HPBarWidget*)widget;
        typedPtr->partyIndex = partyIndex;
    }
    else {
        //YA DONE MESSED UP
    }
}

void drawArrowWidget(ArrowWidget* arrowWidget) {
    gameDrawAsset(
        arrowWidget->widget.xCoordinate,
        arrowWidget->widget.yCoordinate,
        arrowWidget->code,
        arrowWidget->arrowColor,
        arrowWidget->priority
    );
}

ArrowWidget* createArrowWidget(drawArrowParams params, std::string name) {
    ArrowWidget* widget = (ArrowWidget*)createWidget(name, sizeof(ArrowWidget), &kArrowWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->code = params.arrowCode;
    widget->priority = params.arrowPriority;
    return widget;
}

bool isArrowWidget(Widget* widget) {
    return ((widget->klass == &kArrowWidgetClass));
}

const WidgetClass* ArrowWidgetKlass() {
    return &kArrowWidgetClass;
}

void drawSlotsWidget(SlotsWidget* slotsWidget) {
    if (slotsWidget->materiaSlotsData) {
        renderMateriaSlots(
            slotsWidget->widget.xCoordinate,
            slotsWidget->widget.yCoordinate,
            (i32)(slotsWidget->materiaSlotsData)
        );
    }
}

SlotsWidget* createSlotsWidget(drawSlotsParams params, std::string name) {
    SlotsWidget* widget = (SlotsWidget*)createWidget(name, sizeof(SlotsWidget), &kSlotsWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->materiaSlotsData = params.materiaSlotData;
    return widget;
}

bool isSlotsWidget(Widget* widget) {
    return ((widget->klass == &kSlotsWidgetClass));
}

const WidgetClass* SlotsWidgetKlass() {
    return &kSlotsWidgetClass;
}

void updateMateriaSlots(Widget* widget, u8* materiaSlotsData) {
    if (isSlotsWidget(widget)) {
        auto typedPtr = (SlotsWidget*)widget;
        typedPtr->materiaSlotsData = materiaSlotsData;
    }
    else {
        //YA DONE MESSED UP
    }
}


