#include "impl.h"

static int PatchedGetInsertedCD(void)
{
    int requiredCD;

    requiredCD = *(u8*)(0xDC0BDC);
    if (requiredCD == 0)
        requiredCD = 1;
    return requiredCD;
}

SISTERRAY_API void enableNoCD(void)
{
    mogReplaceFunction((void*)0x404A7D, &PatchedGetInsertedCD);
}
