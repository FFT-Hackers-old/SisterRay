#ifndef CONTAINERS_H
#define CONTAINERS_H

#include "base_widget.h"
#include "allocator.h"

struct CursorGridWidget_ {
    CollectionWidget widget;
    const char* cursorName;
    u32 menuState;
    u32 cursorIdx;
    SRLISTUPDATERPROC updater;
    SRLISTALLOCPROC allocator;
    bool transpose;
};

void drawGridWidget(CursorGridWidget* gridWidget); //draw visibleRow # of child widgets
static const WidgetClass kGridWidgetClass = { (SRWIDGETDRAWPROC)drawGridWidget };
const WidgetClass* GridWidgetClass();
CursorGridWidget* createGridWidget(drawGridParams params, std::string name, const WidgetClass* childType = nullptr);
bool isGridWidget(Widget* widget);

/*row/column widget for holding static texts which cannot rationally be bound to a cursor*/
struct StaticGridWidget_ {
    CollectionWidget widget;
    u16 columnCount;
    u16 rowCount;
    u16 columnSpacing;
    u16 rowSpacing;
    SRLISTUPDATERPROC updater;
    SRLISTALLOCPROC allocator;
};

void drawStaticGridWidget(StaticGridWidget* gridWidget); //draw visibleRow # of child widgets
static const WidgetClass kStaticGridWidgetClass = { (SRWIDGETDRAWPROC)drawStaticGridWidget };
const WidgetClass* StaticGridWidgetClass();
StaticGridWidget* createStaticGridWidget(DrawStaticGridParams params, std::string name, const WidgetClass* childType = nullptr);
bool isStaticGridWidget(Widget* widget);


const WidgetClass* getChildTypeFromID(u16 widgetTypeID);

#endif
