#ifndef MATERIA_INVENTORY_H
#define MATERIA_INVENTORY_H

#define MATERIA_INVENTORY_SIZE (i16)512 //maximum allowable inventory size within the menu engine

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_registry_template.h"

/*This is the new Materia Inventory data structure,
  It cannot inherit from the base as it has per unique materia data*/
typedef struct {
    u8 item_id;
    u32 materia_ap; //u24 originally 
} MateriaInventoryEntry;

class SrMateriaInventory : public SrResourceRegistry<MateriaInventoryEntry> {
public:
    SrMateriaInventory(i16 allocation_size);
    SrMateriaInventory() : SrResourceRegistry<MateriaInventoryEntry>() {}
    void insertIntoMateriaInventory(MateriaInventoryEntry*);
    void removeFromMateriaInventory(u8* inventory_index);
};

SISTERRAY_API void InitMateriaInventory();

#endif
