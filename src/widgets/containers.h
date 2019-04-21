#ifndef CONTAINERS_H
#define CONTAINERS_H

#include "base_widget.h"
#include "allocator.h"

struct CursorGridWidget_ {
    CollectionWidget widget;
    Cursor* cursor;
    SRLISTUPDATERPROC updater;
};

void drawGridWidget(CursorGridWidget* gridWidget); //draw visibleRow # of child widgets
static const WidgetClass kGridWidgetClass = { (SRWIDGETDRAWPROC)drawGridWidget };
const WidgetClass* GridWidgetClass();
CursorGridWidget* createGridWidget(drawGridParams params, std::string name, const WidgetClass* childType);
bool isGridWidget(Widget* widget);


const WidgetClass* getChildTypeFromID(u16 widgetTypeID);

#endif
