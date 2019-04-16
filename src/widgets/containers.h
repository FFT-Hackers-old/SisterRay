#ifndef CONTAINERS_H
#define CONTAINERS_H

#include "base_widget.h"
#include "allocator.h"

struct CursorGridWidget_ {
    CollectionWidget widget;
    CursorContext* cursor;
    SRLISTUPDATERPROC updater;
    u16 rowSpacing;
    u16 columnSpacing;
};

void drawGridWidget(CursorGridWidget* gridWidget); //draw visibleRow # of child widgets
const WidgetClass kGridWidgetClass = { (SRWIDGETDRAWPROC)drawGridWidget };
CursorGridWidget* createGridWidget(drawGridParams params, std::string name, const WidgetClass* childType);
bool isGridWidget(Widget* widget);


const WidgetClass* getChildTypeFromID(u16 widgetTypeID);

#endif
