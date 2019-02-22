#include "weapons.h"
#include "impl.h"

/*Patch references to items to reference to reference the new gContext objects*/
static void patch_weapons() {
    u8 a = 1;
}

SISTERRAY_API void init_weapons(SrKernelStream* stream)
{
    gContext.weapons = SrWeaponRegistry(stream);
    gContext.item_type_data.initialize_augmented_data((u8)1, gContext.weapons.resource_count());
    patch_weapons();
    srLogWrite("kernel.bin: Loaded %lu items", (unsigned long)gContext.weapons.resource_count());
}
