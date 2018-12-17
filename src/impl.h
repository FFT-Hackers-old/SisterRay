#ifndef IMPL_H
#define IMPL_H

#include <SisterRay/SisterRay.h>

typedef void (PFNCOMMANDMAINPROC)(u32);
SISTERRAY_GLOBAL PFNCOMMANDMAINPROC* oldCommandMain;

#endif