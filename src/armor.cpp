#include <stdlib.h>
#include <string.h>
#include "impl.h"
#include "kernel_utils.h"
#include "armor.h"


SISTERRAY_API void InitArmor(SrKernelStream* stream)
{
    SrArmorRegistry* registry = &gContext.armors;
    initRegistry<ArmorData, SrArmorRegistry>(
        stream,
        registry,
        allocKernelObject<ArmorData, SrArmorRegistry>,
        initObjectRegistry<SrArmorRegistry>);
    srLogWrite("kernel.bin: Loaded %lu Armors", (unsigned long)gContext.armors.count);
}
