#include "items.h"
#include "kernel_utils.h"
#include "impl.h"

static void PatchItems(void) {

}

SISTERRAY_API void InitItem(SrKernelStream* stream)
{
    SrItemRegistry* registry = &gContext.items;
    initRegistry<ItemData>(
        stream,
        registry,
        allocKernelObject<ItemData, SrItemRegistry>,
        initObjectRegistry<ItemData, SrItemRegistry>);
    PatchItems();
    srLogWrite("kernel.bin: Loaded %lu items", (unsigned long)gContext.items.count);
}
