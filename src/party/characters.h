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
#include <unordered_map>
#include <vector>

using StatTable = std::array<i32, 100 >;
using StatTables = std::unordered_map<std::string, StatTable>;
/*Holds character AI, as well as currently equipped materias*/
struct _SrCharacter {
    CharacterRecord* gameCharacter;
    BattleAIData characterAI;
    KernelCharacterGrowth characterGrowth;
    std::vector<GearSlot> equipment;
    StatTables statTable;
    std::unordered_map<std::string, u32> appliedPoints;
    std::unordered_map<std::string, u32> statPointMultipliers;
    u16 unspentPoints;
};

/*The following registries contain enemy data and AI scripts indexed by the absolute ID of the enemy*/
class SrCharacterRegistry : public SrNamedResourceRegistry<SrCharacter, std::string> {
public:
    SrCharacterRegistry() : SrNamedResourceRegistry<SrCharacter, std::string>() {}
};

SISTERRAY_API void initCharacterData(SrKernelStream* stream);
void initializeCharacterStats(SrCharacter& character);
void createDefaultEquipmentSlots(SrCharacter& character);
void finalizeCharacters();

#endif
