#include <stdlib.h>
#include <string.h>
#include "impl.h"
#include "kernel_utils.h"
#include "accessory.h"


SISTERRAY_API void InitAccessory(SrKernelStream* stream)
{
    SrAccessoryRegistry* registry = &gContext.accessories;
    initRegistry<AccessoryData>(
        stream,
        registry,
        allocKernelObject<AccessoryData, SrAccessoryRegistry>,
        initObjectRegistry<AccessoryData, SrAccessoryRegistry>);

    srLogWrite("kernel.bin: Loaded %lu accessories", (unsigned long)gContext.accessories.count);
}
