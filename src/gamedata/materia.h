#ifndef MATERIA_H
#define MATERIA_H

#include "../sr_named_registry.h"
#include "game_data_interface.h"
#include "base_item.h"
#include "../EncodedString.h"
#include <map>

#define KERNEL_MATERIA_COUNT 96

typedef struct {
    MateriaData gameMateria;
    AuxMateriaData auxData;
    EncodedString materiaName;
    EncodedString materiaDescription;
    EquipmentStatBoosts equipEffects;
} SrMateria;

class SrMateriaRegistry : public SrNamedResourceRegistry<SrMateria, std::string> {
public:
    SrMateriaRegistry(SrKernelStream* stream);
    SrMateriaRegistry() : SrNamedResourceRegistry<SrMateria, std::string>() {}
};



void setEquipEffects(u16 equipEffect, SrMateria& srMateria, u16 materiaIdx);
u8 getMateriaTopType(u16 materiaID);
u8 getMateriaSubType(u16 materiaID);
u32 getMateriaColorType(u16 materiaID);
u8 getMateriaLevel(const MateriaInventoryEntry& materia, u8* maxLevelPtr);
SrEventContext getTopKey(u8 materiaType);
SrEventContext getSubKey(u8 materiaType);

#endif
