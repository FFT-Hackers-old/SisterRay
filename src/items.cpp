#include "items.h"
#include "kernel_utils.h"
#include "impl.h"

/*This will initialize the global_item_id -> relative_id map for kernel items
  requires the registry to have already been initialized*/
static void InitializeAugmentedItemData() {
    for (int i = 0; i < gContext.items.count; i++) {
        ItemTypeData* object = allocKernelObject<ItemTypeData, SrItemTypeData>(&gContext.item_type_data);
        object->item_type = 0;
        object->type_relative_id = gContext.item_type_data.count - 1; //relative IDs are the same as indexes into the array for the original 128 items
    }
}

/*Patch references to items to reference to reference the new gContext objects*/
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
    InitializeAugmetedData();
    PatchItems();
    srLogWrite("kernel.bin: Loaded %lu items", (unsigned long)gContext.items.count);
}
