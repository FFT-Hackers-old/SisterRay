#ifndef CONTAINERS_H
#define CONTAINERS_H

#include "base_widget.h"
#include "../menus/menu.h"

typedef struct {
    CollectionWidget widget;
    cursorContext* cursorContext;
    SRLISTUPDATERPROC updater;
    u16 rowSpacing;
    u16 columnSpacing;
} CursorGridWidget;

typedef struct {
    cursorContext* cursorContext;
    SRLISTUPDATERPROC updater;
    u32 xCoordinate;
    u32 yCoordinate;
    u16 rowSpacing;
    u16 columnSpacing;
} GridWidgetParams;

void drawGridWidget(CursorGridWidget* listWidget); //draw visibleRow # of child widgets
static const WidgetClass kGridWidgetClass = { (SRWIDGETDRAWPROC)drawGridWidget };
CursorGridWidget* createGridWidget(GridWidgetParams params, std::string name, WidgetClass* childType);
bool isGridWidget(Widget* widget);
#endif
