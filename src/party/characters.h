#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "../battle/ai_scripts.h"
#include "party_utils.h"
#include "../gamedata/materia.h"
#include <array>

/*Holds character AI, as well as currently equipped materias*/
typedef struct {
    CharacterRecord* gameCharacter;
    BattleAIData characterAI;
    KernelCharacterGrowth characterGrowth;
    std::array<MateriaInventoryEntry, 8> wpnMaterias;
    std::array<MateriaInventoryEntry, 8> armMaterias;
    u16 equippedWeapon;
    u16 equippedArmor;
    u16 equippedAccessory;
} SrCharacterData;

/*The following registries contain enemy data and AI scripts indexed by the absolute ID of the enemy*/
class SrCharacterRegistry : public SrNamedResourceRegistry<SrCharacterData, std::string> {
public:
    SrCharacterRegistry() : SrNamedResourceRegistry<SrCharacterData, std::string>() {}
};

SISTERRAY_API void initCharacterData(SrKernelStream* stream);
#endif
