#include "characters.h"
#include "../impl.h"
//#include <google/protobuf/text_format.h>

#define CHARACTER_BLOCK_SIZE 3988
using namespace StatNames;

SISTERRAY_API void initCharacterData(SrKernelStream* stream) {
    srLogWrite("loading character AI Data");
    gContext.characters = SrCharacterRegistry();
    u8* sectionBuffer = (u8*)malloc(CHARACTER_BLOCK_SIZE);
    srKernelStreamRead(stream, (void*)sectionBuffer, CHARACTER_BLOCK_SIZE);
    KernelCharacterGrowth* growthDataPtr = (KernelCharacterGrowth*)sectionBuffer;
    auto characterAIPtr = sectionBuffer + 0x61C;

    for (auto characterIndex = 0; characterIndex < 12; characterIndex++) {
        srLogWrite("attemping to load AI scripts for character %i", characterIndex);
        auto character = SrCharacter();
        BattleAIData characterAIData = BattleAIData();
        initializeBattleAIData(characterAIPtr, characterIndex, characterAIData);
        character.gameCharacter = nullptr;
        character.characterAI = characterAIData;
        if (characterIndex < 9) {
            character.characterGrowth = growthDataPtr[characterIndex];
            character.gameCharacter = &(CHARACTER_RECORD_ARRAY[characterIndex]);
        }
        auto charName = getCharacterName(characterIndex);
        initializeStats<SrCharacter>(character);
        createDefaultEquipmentSlots(character);
        gContext.characters.addElement(charName, character);
    }

    free(sectionBuffer);
    srLogWrite("kernel.bin: Loaded %lu character AI scripts", (unsigned long)gContext.characters.resourceCount());
}

void initSummonCharacters() {
    gContext.summons = SrSummonRegistry();
    for (auto summonIdx = 0; summonIdx < 16; summonIdx++) {
        srLogWrite("attemping to load AI scripts for character %i", summonIdx);
        auto summon = SrSummon();
        BattleAIData summonAI = BattleAIData();
        summon.characterAI = summonAI;
        initializeStats<SrSummon>(summon);
        summon.summonDisplayName = EncodedString::from_unicode(getSummonName(summonIdx).c_str());
        gContext.summons.addElement(BASE_PREFIX + std::to_string(summonIdx), summon);
    }
}

std::string getSummonName(u16 summonIdx) {
    switch (summonIdx) {
    case 0: {
        return "Choco/Mog";
    case 1: {
        return "Shiva";
    }
    case 2: {
        return "Ifrit";
    }
    case 3: {
        return "Ramuh";
    }
    case 4: {
        return "Titan";
    }
    case 5: {
        return "Odin";
    }
    case 6: {
        return "Leviathan";
    }
    case 7: {
        return "Bahamut";
    }
    case 8: {
        return "Kujata";
    }
    case 9: {
        return "Alexander";
    }
    case 10: {
        return "Phoenix";
    }
    case 11: {
        return "Neo Bahamut";
    }
    case 12: {
        return "Hades";
    }
    case 13: {
        return "Typhon";
    }
    case 14: {
        return "Bahamut ZERO";
    }
    case 15:
        return "Knights of the Round";
    }
    }
    return "";
}


void createDefaultEquipmentSlots(SrCharacter& character) {
    for (u8 gearSlotIdx = 0; gearSlotIdx < 3; gearSlotIdx++) {
        GearSlot slot;
        switch (gearSlotIdx) {
        case 0: {
            slot.slotGearType = SR_GEAR_WEAPON;
            slot.slotName = EncodedString::from_unicode("WPN:");
            slot.equipped = gContext.weapons.getResource(0).sharedBase;
            break;
        }
        case 1: {
            slot.slotGearType = SR_GEAR_ARMOR;
            slot.slotName = EncodedString::from_unicode("ARM:");
            slot.equipped = gContext.armors.getResource(0).sharedBase;
            break;
        }
        case 2: {
            slot.slotGearType = SR_GEAR_ACCESSORY;
            slot.slotName = EncodedString::from_unicode("ACC:");
            break;
        }
        }
        slot.equippedIdx = 0;
        character.equipment.push_back(slot);
    }
}

SISTERRAY_API CharacterRecord* getPartyActorCharacterRecord(u8 characterIdx) {
    return gContext.characters.getResource(characterIdx).gameCharacter;
}

SISTERRAY_API SrCharacter* getSrCharacterRecord(u8 characterIdx) {
    return &(gContext.characters.getResource(characterIdx));
}

SISTERRAY_API CharacterRecord* getGameCharacter(SrCharacter* srCharacter) {
    return srCharacter->gameCharacter;
}


void finalizeCharacters() {
    finalizeRegistry<SrCharacter, InitCharacterEvent, SrCharacterRegistry>(gContext.characters, INIT_CHARACTERS);
}


void finalizeSummons() {
    finalizeRegistry<SrSummon, InitSummonCharacterEvent, SrSummonRegistry>(gContext.summons, INIT_SUMMONS);
}
