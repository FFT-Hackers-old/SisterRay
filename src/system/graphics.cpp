#include "graphics.h"


typedef void(*pfnsub66067A)(u32, u32, u32, u32, void*);
#define gameSetViewport               ((pfnsub66067A)0x66067A)
void srSetViewport(const ViewPort& port) {
    auto ctx = getGraphicsCtx();
    gameSetViewport(port.x, port.y, port.w, port.h, ctx);
}
