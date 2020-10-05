#ifndef SR_GRAPHICS_H
#define SR_GRAPHICS_H

#include <SisterRay/SisterRay.h>


struct ViewPort {
    u32 x;
    u32 y;
    u32 w;
    u32 h;
};

void srSetViewport(const ViewPort& port);

#endif
