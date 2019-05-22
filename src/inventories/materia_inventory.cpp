#include "materia_inventory.h"
#include "../impl.h"
#include <memory>

SrMateriaInventory::SrMateriaInventory(i16 allocation_size) : SrResourceRegistry<MateriaInventoryEntry>(allocation_size) {
    for (i32 i = 0; i < MATERIA_INVENTORY_SIZE; i++) {
        if (i < 100) {
            MateriaInventoryEntry entry = {
                i,
                17350
            };
            add_resource(entry);
        }
        else {
            MateriaInventoryEntry entry = {
                0xFFFF,
                0
            };
            add_resource(entry);
        }
    }
}

/*there's some other things that normally happen when this is called*/
void SrMateriaInventory::insertIntoMateriaInventory(const MateriaInventoryEntry& entry) {
    u16 inventoryIndex;
    for (auto it = begin(_resource_registry); it != end(_resource_registry); ++it) {
        if ((*it).item_id == 0xFFFF) {
            inventoryIndex = distance(_resource_registry.begin(), it);
            update_resource(inventoryIndex, entry);

            if (getMateriaColorType(entry.item_id) == 0xA) // if Summon Materia
                *enabledSpellTypeMask = *enabledSpellTypeMask | 1u; //enable summons on magic menu
            if (entry.item_id == 44)   // Enemy Skill Case
                *enabledSpellTypeMask = *enabledSpellTypeMask | 2u; //enable eskill on magic menu
            //sub_6CC2C9(entry.item_id);
            return;
        }
    }
}

void SrMateriaInventory::removeFromMateriaInventory(u16 inventory_index) {
}

/*TODO: implement these two methods*/
bool SrMateriaInventory::isFull() {
    return false;
}

void SrMateriaInventory::sortInventory() {
    return;
}

SISTERRAY_API void initMateriaInventory() {
    gContext.materiaInventory = std::make_unique<SrMateriaInventory>(MATERIA_INVENTORY_SIZE);
    srLogWrite("sister ray: inventory initialized with capacity: %lu", (unsigned long)gContext.inventory->current_capacity());
}
