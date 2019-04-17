#ifndef MATERIA_H
#define MATERIA_H

#include <SisterRay/types.h>
#include <SisterRay/data_addresses.h>
#include "../sr_named_registry.h"
#include "game_data_interface.h"

class SrMateriaRegistry : public SrNamedResourceRegistry<MateriaData, std::string> {
public:
    SrMateriaRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<MateriaData, std::string>(stream) {}
    SrMateriaRegistry() : SrNamedResourceRegistry<MateriaData, std::string>() {}
};

#endif
