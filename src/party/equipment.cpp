#include "equipment.h"
#include "../impl.h"

GearSlot::GearSlot() {
    MateriaInventoryEntry defaultMat = MateriaInventoryEntry();
    defaultMat.materiaID = 0xFFFF;
    defaultMat.materiaAP = 0;
    equippedIdx = 0xFFFF;
    slotGearType = SR_INVALID_GEAR_TYPE;
    materia.fill(defaultMat);
}

u8* getMateriaSlots(u16 relativeEquipIdx, SrGameGearType gearType) {

    if (gearType == SR_GEAR_WEAPON) {
        auto materiaSlots = &(gContext.weapons.getResource(relativeEquipIdx).gameWeapon.materiaSlots[0]);
        return materiaSlots;
    }
    else if (gearType == SR_GEAR_ARMOR) {
        auto materiaSlots = &(gContext.armors.getResource(relativeEquipIdx).gameArmor.materiaSlots[0]);
        return materiaSlots;
    }
    return nullptr;
}
