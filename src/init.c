#include <stdio.h>
#include <mog/mog.h>
#include <windows.h>
#include "damage_formulas.h"
#include "command_mechanics.h"
#include "elemental_modifiers.h"
#include "impl.h"
#include <zlib.h>

PFNCOMMANDMAINPROC* oldCommandMain;
PFNRUNANIMSCRIPT* oldRunAnimationScript;
PFNDECREMENTCOUNTERS* oldDecrementCounters;
PFNAPPLYDAMAGE*  oldApplyDamage;
// PFNANIMEFFECT* oldAnimEffectLookUp;

SrContext gContext;

static const SrKernelStreamHandler kKernelBinHandlers[9] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    InitMateria,
};

static void srLoadKernelBin(void)
{
    FILE* kernel;
    SrKernelStream stream;
    SrKernelStreamHandler handler;

    kernel = fopen(srGetGamePath("data/kernel/kernel.bin"), "rb");
    for (int i = 0; i < 9; ++i)
    {
        srKernelStreamOpen(&stream, kernel);
        handler = kKernelBinHandlers[i];
        if (handler)
            handler(&stream);
        else
            srKernelStreamSkip(&stream);
        srKernelStreamClose(&stream);
    }
    fclose(kernel);
}

__declspec(dllexport) void rayInit()
{
	MessageBoxA(NULL, "Sister ray at 100% power", "SisterRay", 0);
    InitLog();
    EnableNoCD();
    InitFunctionRegistry();
    srLoadKernelBin();
    LoadMods();
}
