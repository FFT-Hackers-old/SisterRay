#ifndef MENU_UTILS_H
#define MENU_UTILS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "menu.h"


void renderMenuSideScroller(drawScrollerParams* arguments, float floatArg);
void renderGearMateriaSlots(u32 xbase, u32 ybase, u8* slotsToRender);
i32 displayTextAtLocation(i32 x_start, i32 y_start, const char* stringToDisplay, color textColor, float thickness); //The last argument might be a float, not sure yet
void setSideScrollerArguments(drawScrollerParams*, u16 viewSize, u16 maxRows, u16 baseRow, u16 pos1, u16 pos2, u16 pos3, u16 pos4);
void setContextCursorData(cursorContext* cursorContext, u32 relativeColumn, u32 relativeRow,
    u32 viewColumn, u32 viewRow, u32 baseColumn, u32 baseRow, u32 maxColumn, u32 maxRow,
    u32 unk_9, u32 unk_10, u32 unk_11, u32 unk_12, u32 unk_13, u32 unk_14);

#endif
