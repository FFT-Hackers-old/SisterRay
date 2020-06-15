#include "limits.h"
#include "../battle/battle_models/battle_models_api.h"
#include "game_data_interface.h"
#include "element_names.h"
#include "../impl.h"

void initLimits(u8* magicLGP) {
    loadCloudLimitAnimations(magicLGP);
    loadCloudAnimScripts();
    createCloudLimitActions();
}

void loadCloudAnimScripts() {
    u8 braverScript[33] = { 0xE8, 0xFC, 0, 0xE0, 0xEA, 0xF4, 0x19, 0xF3, 0xEC, 0xF0, 0xD8, 0, 0x1A, 0, 0x2C, 0xD1, 0xB0, 4, 0, 0, 4, 0xF0, 0x2D, 0xD8, 6, 0x30, 0, 0x2E, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE };
    braverScript[14] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 0, "CLOUD.DAT");
    braverScript[22] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 1, "CLOUD.DAT");
    braverScript[27] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 2, "CLOUD.DAT");
    braverScript[30] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 3, "CLOUD.DAT");
    addAnimationScript(CLOUD_LIMIT_MOD_NAME, 0, "CLOUD.DAT", braverScript, 33);


    u8 crossScript[30] = { 0xE8, 0xFC, 0, 0xE0, 0xEA, 0xF4, 0x19, 0xF3, 0xEC, 0xF0, 0xD8, 0, 0x1A, 0, 0x2C, 0xD1, 0xB0, 4, 0, 0, 4, 0xF0, 0x2D, 0x2E, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE };
    crossScript[14] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 4, "CLOUD.DAT");
    crossScript[22] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 5, "CLOUD.DAT");
    crossScript[23] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 6, "CLOUD.DAT");
    crossScript[26] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 7, "CLOUD.DAT");
    addAnimationScript(CLOUD_LIMIT_MOD_NAME, 1, "CLOUD.DAT", crossScript, 30);

    u8 climScript[35] = { 0xE8, 0xFC, 0, 0xE0, 0xEA, 0xF4, 0x19, 0xF3, 0xEC, 0xF0, 0x2C, 0xD8, 0, 0x1A, 0, 0xD1, 0xB0, 4, 0, 0, 4, 0xF0, 0x2D, 0xD8, 0x19, 0x30, 0, 0xA8, 0x26, 8, 0x2E, 0xFA, 0xF0, 0xE5, 0xEE };
    climScript[10] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 10, "CLOUD.DAT");
    climScript[22] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 11, "CLOUD.DAT");
    climScript[30] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 12, "CLOUD.DAT");
    addAnimationScript(CLOUD_LIMIT_MOD_NAME, 2, "CLOUD.DAT", climScript, 35);
}

void createCloudLimitActions() {
    auto braver = SrActionData();
    braver.attackName = "Braver";
    braver.attackDesc = "Powerful Jumping Slash, Capable of Shattering Defenses";
    braver.baseData.attackPower = 48;
    braver.baseData.additionalEffect = 0xFF;
    braver.baseData.additionalEffectModifier = 0xFF;
    braver.baseData.animationEffectID = 0;
    braver.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    braver.baseData.abilityHitRate = 0x64;
    braver.baseData.cameraMovementSingle = 0x32;
    braver.baseData.cameraMovementMultiple = 0xFFFF;
    braver.baseData.impactEffectID = 0xFF;
    braver.baseData.impactSoundID = 0xFFFF;
    braver.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(braver, 0, CLOUD_LIMIT_MOD_NAME);
    addElementToAction(CLOUD_LIMIT_MOD_NAME, 0, ElementNames::SLASH.c_str());
    setActionAnimationScript(CLOUD_LIMIT_MOD_NAME, 0, "CLOUD.DAT", CLOUD_LIMIT_MOD_NAME, 0);
    setActionEffectType(CLOUD_LIMIT_MOD_NAME, 0, LIMIT);
    addActionToCommand(BASE_PREFIX, CMD_LIMIT, CLOUD_LIMIT_MOD_NAME, 0);


    auto cross = SrActionData();
    cross.attackName = "Cross-Slash";
    cross.attackDesc = "Powerful Jumping Slash, Capable of Shattering Defenses";
    cross.baseData.attackPower = 30;
    cross.baseData.additionalEffect = 0;
    cross.baseData.additionalEffectModifier = 3;
    cross.baseData.animationEffectID = 1;
    cross.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    cross.baseData.abilityHitRate = 0x64;
    cross.baseData.cameraMovementSingle = 0x34;
    cross.baseData.cameraMovementMultiple = 0xFFFF;
    cross.baseData.impactEffectID = 0xFF;
    cross.baseData.impactSoundID = 0xFFFF;
    cross.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(cross, 1, CLOUD_LIMIT_MOD_NAME);
    addElementToAction(CLOUD_LIMIT_MOD_NAME, 1, ElementNames::SLASH.c_str());
    setActionAnimationScript(CLOUD_LIMIT_MOD_NAME, 1, "CLOUD.DAT", CLOUD_LIMIT_MOD_NAME, 1);
    setActionEffectType(CLOUD_LIMIT_MOD_NAME, 1, LIMIT);
    addActionToCommand(BASE_PREFIX, CMD_LIMIT, CLOUD_LIMIT_MOD_NAME, 1);


    auto clim = SrActionData();
    clim.attackName = "Climhazzard";
    clim.attackDesc = "Stab your foe, and cleave them in two";
    clim.baseData.attackPower = 60;
    clim.baseData.additionalEffect = 0xFF;
    clim.baseData.additionalEffectModifier = 0;
    clim.baseData.animationEffectID = 3;
    clim.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    clim.baseData.abilityHitRate = 0x64;
    clim.baseData.cameraMovementSingle = 0x35;
    clim.baseData.cameraMovementMultiple = 0xFFFF;
    clim.baseData.impactEffectID = 0xFF;
    clim.baseData.impactSoundID = 0xFFFF;
    clim.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(clim, 2, CLOUD_LIMIT_MOD_NAME);
    addElementToAction(CLOUD_LIMIT_MOD_NAME, 2, ElementNames::PIERCE.c_str());
    setActionAnimationScript(CLOUD_LIMIT_MOD_NAME, 2, "CLOUD.DAT", CLOUD_LIMIT_MOD_NAME, 2);
    setActionEffectType(CLOUD_LIMIT_MOD_NAME, 2, LIMIT);
    addActionToCommand(BASE_PREFIX, CMD_LIMIT, CLOUD_LIMIT_MOD_NAME, 2);
}


void loadCloudLimitAnimations(u8* magicLGPBuffer) {
    std::vector<u8> trueLoadCounts = { 4, 4, 2, 3, 1, 1, 3 };
    auto idx = 0;
    auto limitIdx = 0;
    srLogWrite("Loading Cloud's Limit Breaks");
    for (auto archiveName : cloudLimitNames) {
        LGPContext lgpContext = { 0, 1, 2, (PFNFF7MANGLER)0x5E2908 };
        srLogWrite("Attempting to load Animations from Limit Archive: %s", archiveName.c_str());
        LGPArchiveFile archiveFile = srOpenDAFile(&lgpContext, archiveName.c_str(), (void*)magicLGPBuffer, false);
        auto animations = loadModelAnimationFromDAFile("CLOUD.DAT", (u8*)archiveFile.buffer, true);
        //Load the lacerate animations
        u16 loadedAnimCount = 0;
        for (auto animation : animations) {
            if (loadedAnimCount >= trueLoadCounts[limitIdx]) {
                srLogWrite("Break triggered, limit idx: %i, load_count: %i", limitIdx, trueLoadCounts[limitIdx]);
                break;
            }
            addPlayerModelAnimation(CLOUD_LIMIT_MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
            loadedAnimCount++;
            idx++;
        }
        limitIdx++;
    }
    srLogWrite("Loading Cloud's Limit Breaks");
}
