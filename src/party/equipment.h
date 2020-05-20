#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <SisterRay/SisterRay.h>
#include "../gamedata/battle_stats.h"
#include "../gamedata/base_item.h"
#include <array>

typedef struct GearSlot {
    u16 equippedIdx;
    SrGameGearType slotGearType;
    std::array<MateriaInventoryEntry, 8> materia;
    u16 gearSubType;
    Equippable equipped;
    EncodedString slotName;

    GearSlot();
} GearSlot;


u8* getMateriaSlots(u16 relativeEquipIdx, SrGameGearType gearType);

#endif
