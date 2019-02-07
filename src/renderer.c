#include <windows.h>
#include "impl.h"

typedef void     (*FuncType_401810)(void);
typedef void     (*FuncType_41B577)(void*);
typedef void     (*FuncType_661966)(int);
typedef void     (*FuncType_6619AA)(int, float);
typedef void     (*FuncType_66064A)(int, int, int, double*);
typedef void     (*FuncType_67453A)(int);
typedef void     (*FuncType_67CCDE)(float, float, float, float, float, float, float, void*);
typedef void     (*FuncType_6833DE)(int*);
typedef void     (*FuncType_6CEA17)(void);

static FuncType_401810 Func_401810 = (void*)0x401810;
static FuncType_41B577 Func_41B577 = (void*)0x41B577;
static FuncType_661966 Func_661966 = (void*)0x661966;
static FuncType_6619AA Func_6619AA = (void*)0x6619AA;
static FuncType_66064A Func_66064A = (void*)0x66064A;
static FuncType_67453A Func_67453A = (void*)0x67453A;
static FuncType_67CCDE Func_67CCDE = (void*)0x67CCDE;
static FuncType_6833DE Func_6833DE = (void*)0x6833DE;
static FuncType_6CEA17 Func_6CEA17 = (void*)0x6CEA17;

static int GetConfigGraphicsMode(void)
{
    return 2;
}

static void FrameEnterSwirl(double* arg)
{
    float scale = gContext.windowWidth / 160.f;

    *((uint32_t*)0x99F330) = gContext.windowWidth;
    *((uint32_t*)0x99F334) = gContext.windowHeight;
    *((uint32_t*)0x9A04B0) = (uint32_t)scale;
    *((uint32_t*)0x9A04D4) = 0;
    *((uint32_t*)0x9A04D8) = 0;
    *((uint32_t*)0x9A04DC) = (uint32_t)(scale * 32);

    *((uint16_t*)0x9A04C8) = 0;
    *((uint16_t*)0x9A04CA) = 0;
    *((uint16_t*)0x9A04B8) = gContext.windowWidth / 2;
    *((uint16_t*)0x9A04BA) = 0;
    *((uint16_t*)0x9A04BC) = 160;
    *((uint16_t*)0x9A04CC) = 160;
    *((uint16_t*)0x9A04BE) = 240;
    *((uint16_t*)0x9A04CE) = 240;

    *((uint32_t*)0x9A04D0) = 32;
    Func_401810();
    *((uint32_t*)0x9A04E0) = 0;
    *((uint32_t*)0x9A04E4) = 0;
    *((double*)0x9A04A0) = arg[6] / 30.0;
    Func_661966(1);
    Func_66064A(0, 1, 0, arg);
    *((uint32_t*)0x99F338) = 0;
}

static void FrameEnterBattle(void* arg)
{
    float scale = gContext.windowWidth / 160.f;

    int tmp;
    float v1;
    float v2;

    Func_41B577(arg);
    if (*((uint32_t*)0x7C10DC))
        Func_67453A(0);
    else
        Func_67453A(1);
    Func_661966(1);
    Func_6619AA(0, 3.71875);
    tmp = 32849;
    Func_6833DE(&tmp);
    ((float*)arg)[615] = 90.f;
    ((float*)arg)[616] = 250.f;
    ((float*)arg)[617] = 64000.f;
    ((float*)arg)[618] = 0.f;
    ((float*)arg)[619] = 0.f;
    v1 = (float)(double)(((int*)arg)[532]);
    v2 = (float)(double)(((int*)arg)[533]);
    Func_67CCDE(90.f, 250.f, 64000.f, 0.f, 0.f, v1, v2, arg);
    *((uint32_t*)0x9ACB5C) = 2;

    *((uint32_t*)0x9A89E0) = 0;
    *((uint32_t*)0x9A89E4) = (uint32_t)(scale * 37);
    *((uint32_t*)0x9AAD4C) = 0;
    *((uint32_t*)0x9AAD50) = 0;
    *((uint32_t*)0x9AAD5C) = gContext.windowWidth;
    *((uint32_t*)0x9ACB88) = gContext.windowHeight;
    *((uint32_t*)0x9AAD68) = (uint32_t)(scale * 166);
    *((uint32_t*)0x9AD1A8) = (uint32_t)scale;

    *((uint32_t*)0x9AC108) = gContext.windowWidth - 1;
    *((uint32_t*)0x9AD198) = (uint32_t)(scale * 166) - 1;
    *((void**)0x9ACB90) = (void*)((int*)arg + 603);
    Func_6CEA17();
}

static int ConfigureGraphicsDriver(uint32_t* arg)
{
    *((uint32_t*)0x9A054C) = 2;

    arg[525] = 0;
    arg[526] = 0;
    arg[594] = 0;
    arg[595] = 1;
    arg[597] = gContext.windowWidth;
    arg[598] = gContext.windowHeight;
    arg[601] = 1;
    arg[602] = 1;
    arg[603] = 0;
    arg[609] = 1;
    arg[610] = 0;
    arg[652] = 1;
    arg[664] = 0;
    arg[676] = 0;
    arg[677] = 0;

    return 1;
}

SISTERRAY_API void InitRenderer(void)
{
    mogReplaceFunction((void*)0x404D80, &GetConfigGraphicsMode);
    mogReplaceFunction((void*)0x40164E, &FrameEnterSwirl);
    mogReplaceFunction((void*)0x41B300, &FrameEnterBattle);
    mogReplaceFunction((void*)0x4069FD, &ConfigureGraphicsDriver);
}
