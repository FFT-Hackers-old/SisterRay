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
    std::vector<u32> limitBreaks; //Array of Indexes into the Actions Table
    StatTables statTable;
    std::unordered_map<std::string, u32> appliedPoints;
    std::unordered_map<std::string, u32> statPointMultipliers;
    u16 unspentPoints;
    std::vector<GearSlot> equipment;
};

struct _SrSummon {
    StatTables statTable;
    std::unordered_map<std::string, u32> appliedPoints;
    std::unordered_map<std::string, u32> statPointMultipliers;
    std::vector<u32> limitBreaks; //Array of Indexes into the Actions Table
    BattleAIData characterAI;
    EncodedString summonDisplayName;
};

/*The following registries contain enemy data and AI scripts indexed by the absolute ID of the enemy*/
class SrCharacterRegistry : public SrNamedResourceRegistry<SrCharacter, std::string> {
public:
    SrCharacterRegistry() : SrNamedResourceRegistry<SrCharacter, std::string>() {}
};

class SrSummonRegistry : public SrNamedResourceRegistry<SrSummon, std::string> {
public:
    SrSummonRegistry() : SrNamedResourceRegistry<SrSummon, std::string>() {}
};

SISTERRAY_API void initCharacterData(SrKernelStream* stream);
void initSummonCharacters();
std::string getSummonName(u16 summonIdx);
void initializeCharacterStats(SrCharacter& character);
void createDefaultEquipmentSlots(SrCharacter& character);
void finalizeCharacters();
void finalizeSummons();

template <class T> void initializeStats(T& character) {
    std::vector<std::string> primaryStats{ STRENGTH, VITALITY, DEXTERITY, AGILITY, StatNames::MAGIC, SPIRIT, FOCUS, INSIGHT, LUCK };
    for (auto statName : primaryStats) {
        character.statTable[statName] = StatTable();
        character.statTable[statName].fill(75);
        character.statPointMultipliers[statName] = 1;
        character.appliedPoints[statName] = 0;
    }

    std::vector<std::string> resourceStats{ HP, MP };
    for (auto statName : resourceStats) {
        character.statTable[statName] = StatTable();
        if (statName == MP) {
            character.statTable[statName].fill(100);
        }
        else {
            character.statTable[statName].fill(1000);
        }
        character.statPointMultipliers[statName] = 1;
        character.appliedPoints[statName] = 0;
    }
}
#endif
