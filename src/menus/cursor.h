#ifndef CURSOR_H
#define CURSOR_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "../widgets/widget.h"
#include "../events/event.h"
#include "menu_interface.h"

struct _Cursor {
    CursorContext context; 
    i32 xCoordinate;
    i32 yCoordinate;
    i32 rowSpacing;
    i32 columnSpacing;
};

Cursor* createCursor(CursorContext* context, i32 xCoordinate, i32 yCoordinate, i32 rowSpacing, i32 columnSpacing);
void destroyCursor(Cursor* cursor);


#endif
