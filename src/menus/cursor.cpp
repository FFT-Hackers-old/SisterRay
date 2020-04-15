#include "cursor.h"
#include "../impl.h"

SISTERRAY_API Cursor* createCursor(CursorContext* context, i32 xCoordinate, i32 yCoordinate, i32 rowSpacing, i32 columnSpacing) {
    Cursor* cursor = new Cursor();
    cursor->xCoordinate = xCoordinate;
    cursor->yCoordinate = yCoordinate;
    cursor->rowSpacing = rowSpacing;
    cursor->columnSpacing = columnSpacing;
    cursor->context = *context;
    return cursor;
}

void deleteCursor(Cursor* cursor) {
    delete cursor;
}


SISTERRAY_API CursorContext* getCursorContext(Cursor* cursor){
    return &(cursor->context);
}

SISTERRAY_API void drawCursor(Cursor* cursor, float priority) {
    if (cursor == nullptr) {
        return;
    }
    auto xCoordinate = (cursor->columnSpacing * cursor->context.relativeColumnIndex) + cursor->xCoordinate;
    auto yCoordinate = (cursor->rowSpacing * cursor->context.relativeRowIndex) + cursor->yCoordinate;
    displayCursor(xCoordinate, yCoordinate, priority);
}

SISTERRAY_API void drawFlashingCursor(Cursor* cursor, u32 stateMask, float priority) {
    if (stateMask & 2)
        drawCursor(cursor, priority);
}

SISTERRAY_API void moveCursor(Cursor* cursor, i32 xCoordinate, i32 yCoordinate) {
    cursor->xCoordinate = xCoordinate;
    cursor->yCoordinate = yCoordinate;
}

SISTERRAY_API void scaleCursor(Cursor* cursor, i32 rowSpacing, i32 columnSpacing) {
    cursor->rowSpacing = rowSpacing;
    cursor->columnSpacing = columnSpacing;
}
