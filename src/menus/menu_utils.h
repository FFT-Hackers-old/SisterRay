#ifndef MENU_UTILS_H
#define MENU_UTILS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>

enum color { COLOR_GRAY, COLOR_UNKNOWN_1, COLOR_RED, COLOR_UNKNOWN_2, COLOR_UNKNOWN_3, COLOR_TEAL, COLOR_GREEN, COLOR_WHITE };

typedef struct {
    u16 viewSize;
    u16 maxRows;
    u16 baseRow;
    u16 position1;
    u16 position2;
    u16 position3;
    u16 position4;
} sideScrollerArguments;

void renderMenuSideScroller(sideScrollerArguments* arguments, float floatArg);
void renderGearMateriaSlots(u32 xbase, u32 ybase, u8* slotsToRender);
void setSideScrollerArguments(sideScrollerArguments*, u16 viewSize, u16 maxRows, u16 baseRow, u16 pos1, u16 pos2, u16 pos3, u16 pos4);
void setContextCursorData(cursorContext* cursorContext, u32 relativeColumn, u32 relativeRow,
    u32 viewColumn, u32 viewRow, u32 baseColumn, u32 baseRow, u32 maxColumn, u32 maxRow,
    u32 unk_9, u32 unk_10, u32 unk_11, u32 unk_12, u32 unk_13, u32 unk_14);

#endif
