#ifndef CONTAINERS_H
#define CONTAINERS_H

#include "base_widget.h"
#include "container_table.h"

typedef struct {
    CollectionWidget widget;
    CursorContext* cursor;
    SRLISTUPDATERPROC updater;
    u16 rowSpacing;
    u16 columnSpacing;
} CursorGridWidget;

typedef struct {
    CursorContext* cursor;
    SRLISTUPDATERPROC updater;
    u32 xCoordinate;
    u32 yCoordinate;
    u16 rowSpacing;
    u16 columnSpacing;
} GridWidgetParams;

void drawGridWidget(CursorGridWidget* gridWidget); //draw visibleRow # of child widgets
const WidgetClass kGridWidgetClass = { (SRWIDGETDRAWPROC)drawGridWidget };
CursorGridWidget* createGridWidget(GridWidgetParams params, std::string name, const WidgetClass* childType);
bool isGridWidget(Widget* widget);

#endif
