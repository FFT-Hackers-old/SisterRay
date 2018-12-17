#ifndef IMPL_H
#define IMPL_H

#include <SisterRay/SisterRay.h>

typedef void (PFNCOMMANDMAINPROC)(u8*);
SISTERRAY_GLOBAL PFNCOMMANDMAINPROC* oldCommandMain;

#endif