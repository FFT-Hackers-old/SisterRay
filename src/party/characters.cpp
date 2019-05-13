#include "characters.h"
#include "../impl.h"

#define CHARACTER_BLOCK_SIZE 3988

SISTERRAY_API void initCharacterData(SrKernelStream* stream) {
    gContext.characters = SrCharacterRegistry();
    u8* sectionBuffer = (u8*)malloc(CHARACTER_BLOCK_SIZE);
    srKernelStreamRead(stream, (void*)sectionBuffer, CHARACTER_BLOCK_SIZE);
    auto characterAIPtr = sectionBuffer + 0x61C;

    for (auto characterIndex = 0; characterIndex < 12; characterIndex++) {
        auto characterData = SrCharacterData();
        BattleAIData characterAIData = BattleAIData();
        initializeBattleAIData(characterAIPtr, characterIndex, characterAIData);
        characterData.characterAI = characterAIData;
        auto charName = getCharacterName(characterIndex);
        gContext.characters.add_element(charName, characterData);
    }
    srLogWrite("kernel.bin: Loaded %lu character AI scripts", (unsigned long)gContext.characters.resource_count());
}
