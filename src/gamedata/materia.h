#ifndef MATERIA_H
#define MATERIA_H

#include <SisterRay/types.h>
#include <SisterRay/data_addresses.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"

#define KERNEL_MATERIA_COUNT 96

class SrMateriaRegistry : public SrNamedResourceRegistry<MateriaData, std::string> {
public:
    SrMateriaRegistry(SrKernelStream* stream);
    SrMateriaRegistry() : SrNamedResourceRegistry<MateriaData, std::string>() {}
};

class SrAuxMateriaRegistry : public SrNamedResourceRegistry<AuxMateriaData, std::string> {
public:
    SrAuxMateriaRegistry() : SrNamedResourceRegistry<AuxMateriaData, std::string>() {}
};


void initializeAuxMateriaRegistry();
void populateEquipEffects(const MateriaData& kernelMateria, ActorStatBoosts& boosts);

u8 getMateriaTopType(u16 materiaID);
u8 getMateriaSubType(u16 materiaID);
u32 getMateriaColorType(u16 materiaID);
u8 getMateriaLevel(const MateriaInventoryEntry& materia, u8* maxLevelPtr);
SrEventContext getTopKey(u8 materiaType);
SrEventContext getSubKey(u8 materiaType);

#endif
