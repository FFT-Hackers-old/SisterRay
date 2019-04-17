#include "weapons.h"
#include "../impl.h"

SISTERRAY_API WeaponData getWeapon(u16 itemID) {
    return gContext.weapons.get_resource(itemID);
}

SISTERRAY_API void setWeaponData(WeaponData data, u16 itemID) {
    gContext.weapons.update_resource(itemID, data);
}

SISTERRAY_API void addWeapon(WeaponData data, char* name) {
    gContext.weapons.add_element(std::string(name), data);
}

/*Patch references to items to reference to reference the new gContext objects*/
static void patch_weapons() {
    u8 a = 1;
}

SISTERRAY_API void init_weapon(SrKernelStream* stream)
{
    gContext.weapons = SrWeaponRegistry(stream);
    gContext.itemTypeData.initialize_augmented_data((u8)1, gContext.weapons.resource_count());
    patch_weapons();
    srLogWrite("kernel.bin: Loaded %lu weapons", (unsigned long)gContext.weapons.resource_count());
}
