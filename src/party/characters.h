#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "equipment.h"
#include "../sr_named_registry.h"
#include "../battle/ai_script/ai_scripts.h"
#include "party_utils.h"
#include "../gamedata/materia.h"
#include <array>

/*Holds character AI, as well as currently equipped materias*/
struct _SrCharacter {
    CharacterRecord* gameCharacter;
    BattleAIData characterAI;
    KernelCharacterGrowth characterGrowth;
    std::vector<GearSlot> equipment;
};

/*The following registries contain enemy data and AI scripts indexed by the absolute ID of the enemy*/
class SrCharacterRegistry : public SrNamedResourceRegistry<SrCharacter, std::string> {
public:
    SrCharacterRegistry() : SrNamedResourceRegistry<SrCharacter, std::string>() {}
};

SISTERRAY_API void initCharacterData(SrKernelStream* stream);
void createDefaultEquipmentSlots(SrCharacter& character);

#endif
