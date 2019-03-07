#include "materia_inventory.h"
#include "../impl.h"
#include <memory>

SrMateriaInventory::SrMateriaInventory(i16 allocation_size) : SrResourceRegistry<MateriaInventoryEntry>(allocation_size) {
    for (i32 i = 0; i < MATERIA_INVENTORY_SIZE; i++) {
        MateriaInventoryEntry entry = {
            0xFF,
            0
        };
        add_resource(entry);
    }
}


void SrMateriaInventory::insertIntoMateriaInventory(MateriaInventoryEntry* entry) {
    u16 inventoryIndex;
    for (auto it = begin(resource_registry); it != end(resource_registry); ++it) {
        if ((*it).item_id == 0xFFFF) {
            inventoryIndex = distance(resource_registry.begin(), it);
            update_resource(inventoryIndex, *entry);
        }
    }
}

void SrMateriaInventory::removeFromMateriaInventory(u8* inventory_index) {
}

SISTERRAY_API void InitMateriaInventory() {
    gContext.materia_inventory = std::make_unique<SrMateriaInventory>(MATERIA_INVENTORY_SIZE);
    srLogWrite("sister ray: inventory initialized with capacity: %lu", (unsigned long)gContext.inventory->current_capacity());
}
