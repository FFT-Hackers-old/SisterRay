#include "characters.h"
#include "../impl.h"

#define CHARACTER_BLOCK_SIZE 3988

SISTERRAY_API void initCharacterData(SrKernelStream* stream) {
    srLogWrite("loading character AI Data");
    gContext.characters = SrCharacterRegistry();
    u8* sectionBuffer = (u8*)malloc(CHARACTER_BLOCK_SIZE);
    srKernelStreamRead(stream, (void*)sectionBuffer, CHARACTER_BLOCK_SIZE);
    auto characterAIPtr = sectionBuffer + 0x61C;

    for (auto characterIndex = 0; characterIndex < 12; characterIndex++) {
        srLogWrite("attemping to load AI scripts for character %i", characterIndex);
        auto characterData = SrCharacterData();
        BattleAIData characterAIData = BattleAIData();
        initializeBattleAIData(characterAIPtr, characterIndex, characterAIData);
        characterData.characterAI = characterAIData;
        auto charName = getCharacterName(characterIndex);

        MateriaInventoryEntry defaultMat = MateriaInventoryEntry();
        defaultMat.item_id = 0xFFFF;
        defaultMat.materia_ap = 0;
        characterData.wpnMaterias.fill(defaultMat);
        characterData.armMaterias.fill(defaultMat);

        gContext.characters.addElement(charName, characterData);
    }

    free(sectionBuffer);
    srLogWrite("kernel.bin: Loaded %lu character AI scripts", (unsigned long)gContext.characters.resourceCount());
}
