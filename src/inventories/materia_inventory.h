#ifndef MATERIA_INVENTORY_H
#define MATERIA_INVENTORY_H

#define MATERIA_INVENTORY_SIZE (i16)512 //maximum allowable inventory size within the menu engine

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_registry_template.h"
#include "../gamedata/materia.h"

class SrMateriaInventory : public SrResourceRegistry<MateriaInventoryEntry> {
public:
    SrMateriaInventory(i16 allocation_size);
    SrMateriaInventory() : SrResourceRegistry<MateriaInventoryEntry>() {}
    void insertIntoMateriaInventory(const MateriaInventoryEntry& materia);
    void removeFromMateriaInventory(u8* inventory_index);
    bool isFull();
    void sortInventory();
};

SISTERRAY_API void initMateriaInventory();


#endif
