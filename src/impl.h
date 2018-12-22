#ifndef IMPL_H
#define IMPL_H

#include <SisterRay/SisterRay.h>

typedef void (PFNCOMMANDMAINPROC)(u8*);
SISTERRAY_GLOBAL PFNCOMMANDMAINPROC* oldCommandMain;

typedef void (PFNRUNANIMSCRIPT)(u16, u32, u32, u32);
SISTERRAY_GLOBAL PFNRUNANIMSCRIPT* oldRunAnimationScript;

typedef void (PFNANIMEFFECT)(u8);
SISTERRAY_GLOBAL PFNANIMEFFECT* oldAnimEffectLookUp;

#endif