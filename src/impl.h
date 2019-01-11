#ifndef IMPL_H
#define IMPL_H

#include <mog/mog.h>
#include <SisterRay/SisterRay.h>

typedef void (PFNCOMMANDMAINPROC)(u8*);
SISTERRAY_GLOBAL PFNCOMMANDMAINPROC* oldCommandMain;

typedef void (PFNRUNANIMSCRIPT)(u16, u32, u32, u32);
SISTERRAY_GLOBAL PFNRUNANIMSCRIPT* oldRunAnimationScript;

typedef void (PFNDECREMENTCOUNTERS)();
SISTERRAY_GLOBAL PFNDECREMENTCOUNTERS* oldDecrementCounters;

typedef void (PFNAPPLYDAMAGE)();
SISTERRAY_GLOBAL PFNAPPLYDAMAGE* oldApplyDamage;

//typedef void (PFNANIMEFFECT)(u8);
//SISTERRAY_GLOBAL PFNANIMEFFECT* oldAnimEffectLookUp;

SISTERRAY_API void EnableNoCD(void);
SISTERRAY_API void InitFunctionRegistry(void);
SISTERRAY_API void LoadMods(void);

SISTERRAY_API const void* srLoadFunction(const char* name);
SISTERRAY_API const void* srRegisterFunction(const char* name, const void* func);

#endif