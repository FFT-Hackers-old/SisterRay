#ifndef IMPL_H
#define IMPL_H

#include <mog/mog.h>
#include <SisterRay/SisterRay.h>

#include "kernel.h"
#include "materia.h"
#include "armor.h"
#include "accessory.h"

typedef struct {
    uint32_t            windowWidth;
    uint32_t            windowHeight;
    FILE*               logFile;
    SrArmorRegistry     armors;
    SrAccessoryRegistry accessories;
    SrMateriaRegistry   materias;
} SrContext;

SISTERRAY_GLOBAL SrContext gContext;

SISTERRAY_API void srPatchAddresses(void** patchList, size_t patchCount, void* src, void* dst, size_t offset);

SISTERRAY_API void InitRenderer(void);

SISTERRAY_API void InitMateria(SrKernelStream* stream);
SISTERRAY_API void InitArmor(SrKernelStream* stream);
SISTERRAY_API void InitAccessory(SrKernelStream* stream);

SISTERRAY_API void InitLog(void);
SISTERRAY_API void srLogWrite(const char* format, ...);
SISTERRAY_API void EnableNoCD(void);
SISTERRAY_API void InitFunctionRegistry(void);
SISTERRAY_API void LoadMods(void);
SISTERRAY_API void LoadKernelFile(void** dst, size_t sectionCount, const char* path);

SISTERRAY_API const void* srLoadFunction(const char* name);
SISTERRAY_API const void* srRegisterFunction(const char* name, const void* func);
SISTERRAY_API const char* srGetGamePath(const char* suffix);

#endif
