#include "menu_utils.h"

void setContextCursorData(cursorContext* cursorContext, u32 relativeColumn, u32 relativeRow,
    u32 viewColumn, u32 viewRow, u32 baseColumn, u32 baseRow, u32 maxColumn, u32 maxRow,
    u32 unk_9, u32 unk_10, u32 unk_11, u32 unk_12, u32 unk_13, u32 unk_14) {
    cursorContext->relativeRowIndex = relativeRow;
    cursorContext->relativeColumnIndex = relativeColumn;
    cursorContext->viewColumnBound = viewColumn;
    cursorContext->viewRowBound = viewRow;
    cursorContext->baseColumnIndex = baseColumn;
    cursorContext->baseRowIndex = baseRow;
    cursorContext->maxColumnBound = maxColumn;
    cursorContext->maxRowBound = maxRow;
    cursorContext->ninth_dword = unk_9;
    cursorContext->tenth_dword = unk_10;
    cursorContext->eleventh_dword = unk_11;
    cursorContext->twelth_dword = unk_12;
    cursorContext->thirteenth_dword = unk_13;
    cursorContext->fourteenh_dword = unk_14;
}

void setSideScrollerArguments(sideScrollerArguments* arguments, u16 viewSize, u16 maxRows, u16 baseRow, u16 pos1, u16 pos2, u16 pos3, u16 pos4){
    arguments->viewSize = viewSize;
    arguments->maxRows = maxRows;
    arguments->baseRow = baseRow;
    arguments->position1 = pos1;
    arguments->position2 = pos2;
    arguments->position3 = pos3;
    arguments->position4 = pos4;
}

void renderMenuSideScroller(sideScrollerArguments* arguments, float floatArg) {
    renderSideScroller((i32)arguments, floatArg);
}

void renderGearMateriaSlots(u32 xbase, u32 ybase, u8* slotsToRender) {
    renderMateriaSlots(xbase, ybase, (i32)slotsToRender);
};
