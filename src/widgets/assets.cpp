#include "assets.h"

static void drawScrollerWidget(ScrollerWidget* scrollerPortrait) {
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

static void drawPortraitWidget(PortraitWidget* portraitWidget) {
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


void addChildWidget(Widget* widget, PortraitWidget* child_widget, std::string name) {
    addChildWidget(widget, (Widget*)child_widget, name);
}


static void drawHPBarWidget(HPBarWidget* hpBarWidget) {
    displayPortrait(
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

static void drawArrowWidget(ArrowWidget* arrowWidget) {
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

void addChildWidget(Widget* widget, ArrowWidget* child_widget, std::string name) {
    addChildWidget(widget, (Widget*)child_widget, name);
}

static void drawSlotsWidget(SlotsWidget* slotsWidget) {
    renderMateriaSlots(
        slotsWidget->widget.xCoordinate,
        slotsWidget->widget.yCoordinate,
        (i32)slotsWidget->materiaSlotsData
    );
}

SlotsWidget* createSlotsWidget(drawSlotsParams params, std::string name) {
    SlotsWidget* widget = (SlotsWidget*)createWidget(name, sizeof(SlotsWidget), &kSlotsWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->materiaSlotsData = params.materiaSlotData;
    return widget;
}

// This will not compile, need to fix underlying Widget data struct //
void addChildWidget(Widget* widget, SlotsWidget* child_widget, std::string name) {
    addChildWidget(widget, (Widget*)child_widget, name);
}

