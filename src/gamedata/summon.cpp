#include "summons.h"
#include "../battle/battle_models/battle_models_api.h"
#include "game_data_interface.h"
#include "element_names.h"
#include "../impl.h"

void initSummons(u8* magicLGP) {
    loadSummonAnimations(magicLGP);
    loadSummonAnimScripts();
}

void loadSummonAnimScripts() {
    u8 summonAnimScript[4] = { 0, 1, 0xF1, 0 };
    addAnimationScript(SUMMON_MOD_NAME, 0, summonModelNames[2].c_str(), &(summonAnimScript[0]), 4);
}


void loadSummonAnimations(u8* magicLGPBuffer) {
    auto idx = 0;
    auto summonIdx = 0;
    srLogWrite("Loading Summon Model Animations");
    for (auto archiveName : summonModelNames) {
        LGPContext lgpContext = { 0, 1, 2, (PFNFF7MANGLER)0x5E2908 };
        srLogWrite("Attempting to load Animations from Summon Archive: %s", archiveName.c_str());
        LGPArchiveFile archiveFile = srOpenDAFile(&lgpContext, summonLGPSeekNames[summonIdx].c_str(), (void*)magicLGPBuffer, false);
        auto animations = loadModelAnimationFromDAFile(summonLGPSeekNames[summonIdx].c_str(), (u8*)archiveFile.buffer, true);
        for (auto animation : animations) {
            addModelAnimation(SUMMON_MOD_NAME, idx, archiveName.c_str(), animation["BASE"]);
            idx++;
        }
        srLogWrite("loaded %i animations for summon :%s", gContext.battleAnimations.getElement(archiveName).totalAnimationCount, archiveName.c_str());
        summonIdx++;
    }

    srLogWrite("loaded %i animations for summon :%s", gContext.battleAnimations.getElement(summonModelNames[2]).totalAnimationCount, summonModelNames[2].c_str());
    srLogWrite("Loading Summon Model Animations");
}
