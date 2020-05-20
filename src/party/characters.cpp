#include "characters.h"
#include "../impl.h"

#define CHARACTER_BLOCK_SIZE 3988

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
        createDefaultEquipmentSlots(character);
        gContext.characters.addElement(charName, character);
    }

    free(sectionBuffer);
    srLogWrite("kernel.bin: Loaded %lu character AI scripts", (unsigned long)gContext.characters.resourceCount());
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
