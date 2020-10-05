#include "menu_utils.h"
#include "../module/module_api.h"
#include "../impl.h"

void setContextCursorData(CursorContext* CursorContext, u32 relativeColumn, u32 relativeRow,
    u32 viewColumn, u32 viewRow, u32 baseColumn, u32 baseRow, u32 maxColumn, u32 maxRow,
    u32 unk_9, u32 unk_10, u32 unk_11, u32 unk_12, u32 unk_13, u32 unk_14) {
    CursorContext->relativeRowIndex = relativeRow;
    CursorContext->relativeColumnIndex = relativeColumn;
    CursorContext->viewColumnBound = viewColumn;
    CursorContext->viewRowBound = viewRow;
    CursorContext->baseColumnIndex = baseColumn;
    CursorContext->baseRowIndex = baseRow;
    CursorContext->maxColumnBound = maxColumn;
    CursorContext->maxRowBound = maxRow;
    CursorContext->ninth_dword = unk_9;
    CursorContext->tenth_dword = unk_10;
    CursorContext->eleventh_dword = unk_11;
    CursorContext->twelth_dword = unk_12;
    CursorContext->thirteenth_dword = unk_13;
    CursorContext->fourteenh_dword = unk_14;
}

void setSideScrollerArguments(drawScrollerParams* arguments, u16 viewSize, u16 maxRows, u16 baseRow, u16 pos1, u16 pos2, u16 pos3, u16 pos4){
    arguments->viewSize = viewSize;
    arguments->maxRows = maxRows;
    arguments->baseRow = baseRow;
    arguments->position1 = pos1;
    arguments->position2 = pos2;
    arguments->position3 = pos3;
    arguments->position4 = pos4;
}

i32 displayTextAtLocation(i32 x_start, i32 y_start, const char* stringToDisplay, color textColor, float thickness) {
    gameDrawString(x_start, y_start, stringToDisplay, textColor, thickness);
    return 0;
}

void renderMenuSideScroller(drawScrollerParams* arguments, float floatArg) {
    renderSideScroller((i32)arguments, floatArg);
}

void renderGearMateriaSlots(u32 xbase, u32 ybase, u8* slotsToRender) {
    renderMateriaSlots(xbase, ybase, (i32)slotsToRender);
};


const auto gameDispatchBattleMenuDraw = (PFNSR_VOIDSUB)0x6DC1EB;
const auto gameDispatchMenuDraw = (PFNSR_VOIDSUB)0x6FA347;
void dispatchMenuDraw() {
    auto activeModule = getActiveGameModule();
    if (activeModule == BATTLE) {
        gameDispatchBattleMenuDraw();
    }
    else if (activeModule == MENU) {
        gameDispatchMenuDraw();
    }
}
