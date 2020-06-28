#include "char_commands.h"
#include "../../battle/battle_engine_api.h"
#include "../../gamedata/game_data_interface.h"
#include "../../EncodedString.h"
#include "../../gamedata/element_names.h"
#include "../../menus/menu_interface.h"
#include "../../widgets/widgets_api.h"
#include "../../events/event_bus_interface.h"
#include "../../party/party_interface.h"
#include "../../menus/battle_menu/battle_menu_widget_names.h"
#include "../../gamedata/damage_callback_utils.h"
#include "../../battle/animation_script/animation_script_api.h"
#include "../../impl.h"

#define MOD_NAME "SR_NEW_COMMANDS"
#define SOLDIER_STATE_NAME "SOLDIER_CMD"
static ComboContext cloudComboContext;



void enableSoldier(const EnableDefaultAbilitiesEvent* const event) {
    if (event->characterIdx != ACT_IDX_CLOUD) {
        return;
    }
    enableCommand(event->characterIdx, 0, getInternalCommandID(1, MOD_NAME));
    if (gContext.party.getSrCharacter(event->characterIdx).gamePartyMember->enabledCommandArray[1].commandID == 0xFF) {
        enableCommand(event->characterIdx, 1, getInternalCommandID(0, MOD_NAME));
    }
    else {
        insertCommand(event->characterIdx, 1, getInternalCommandID(0, MOD_NAME));
    }
}

#define LACERATE_PATH  "mods\\cloud\\lacerate.srm"
#define IMPALE_PATH "mods\\cloud\\impale.srm"
#define SWORDPLAY_PATH "mods\\cloud\\swordplay.srm"
#define PUNISHER_PATH "mods\\cloud\\punisher.srm"
#define PUNISHER2_PATH "mods\\cloud\\punisher2.srm"
#define PUNISHER3_PATH "mods\\cloud\\punisher3.srm"
#define FTHRUST_PATH "mods\\cloud\\fthrust.srm"
#define SWORDPLAY_PATH1 "mods\\cloud\\swordplay1.srm"
#define SWORDPLAY_PATH2 "mods\\cloud\\swordplay-2.srm"
#define SWORDPLAY_PATH3 "mods\\cloud\\swordplay-3.srm"
#define SWORDPLAY_PATH2b "mods\\cloud\\swordplay-2b.srm"
#define SWORDPLAY_PATH3b "mods\\cloud\\swordplay-3b.srm"
#define INFINITY_PATH1 "mods\\cloud\\infinityend.srm"
#define INFINITY_PATH2 "mods\\cloud\\infinityendspin.srm"
#define STANCE_PATH "mods\\cloud\\stancechange.srm"
#define PUNISHER_PATHH1 "mods\\cloud\\punisherhit1.srm"

void loadCloudAnimations() {
    auto lacerateAnimation = readLGPArchive(srGetGamePath(LACERATE_PATH));
    auto lacerateAnims = loadModelAnimationFromDAFile("CLOUD.DAT", lacerateAnimation, true);
    auto idx = 0;

    for (auto animation : lacerateAnims) {
        if (idx > 3)
            break;
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
    }

    auto impaleAnimation = readLGPArchive(srGetGamePath(IMPALE_PATH));
    auto impaleAnims = loadModelAnimationFromDAFile("CLOUD.DAT", impaleAnimation, true);
    u8 localIdx = 0;
    for (auto animation : impaleAnims) {
        if (localIdx > 2)
            break;
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
        localIdx++;
    }


    auto swordPlayAnimation = readLGPArchive(srGetGamePath(SWORDPLAY_PATH));
    auto swordPlayAnims = loadModelAnimationFromDAFile("CLOUD.DAT", swordPlayAnimation, true);
    localIdx = 0;
    for (auto animation : swordPlayAnims) {
        if (localIdx > 2)
            break;
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
        localIdx++;
    }

    auto punisherAnimation = readLGPArchive(srGetGamePath(PUNISHER_PATH));
    auto punisherAnims = loadModelAnimationFromDAFile("CLOUD.DAT", punisherAnimation, true);
    localIdx = 0;
    for (auto animation : punisherAnims) {
        if (localIdx > 2)
            break;
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
        localIdx++;
    }

    auto punisher2Animation = readLGPArchive(srGetGamePath(PUNISHER2_PATH));
    auto punisher2Anims = loadModelAnimationFromDAFile("CLOUD.DAT", punisher2Animation, true);
    localIdx = 0;
    for (auto animation : punisher2Anims) {
        if (localIdx > 2)
            break;
        if (localIdx == 0) {
            localIdx++;
            continue;
        }
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
        localIdx++;
    }

    auto punisher3Animation = readLGPArchive(srGetGamePath(PUNISHER3_PATH));
    auto punisher3Anims = loadModelAnimationFromDAFile("CLOUD.DAT", punisher3Animation, true);
    localIdx = 0;
    for (auto animation : punisher3Anims) {
        if (localIdx != 2) {
            localIdx++;
            continue;
        }
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
        localIdx++;
    }

    auto fthrustAnimation = readLGPArchive(srGetGamePath(FTHRUST_PATH));
    auto fthrustAnims = loadModelAnimationFromDAFile("CLOUD.DAT", fthrustAnimation, true);
    localIdx = 0;
    for (auto animation : fthrustAnims) {
        if (localIdx > 2)
            break;
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
        localIdx++;
    }

    //Load 3 hits of sword-combo
    auto sp1 = readLGPArchive(srGetGamePath(SWORDPLAY_PATH1));
    auto sp1a = loadModelAnimationFromDAFile("CLOUD.DAT", sp1, true);
    localIdx = 0;
    for (auto animation : sp1a) {
        if (localIdx != 1) {
            localIdx++;
            continue;
        }
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
        localIdx++;
    }

    //Load 3 hits of sword-combo
    auto sp2 = readLGPArchive(srGetGamePath(SWORDPLAY_PATH2));
    auto sp2a = loadModelAnimationFromDAFile("CLOUD.DAT", sp2, true);
    localIdx = 0;
    for (auto animation : sp2a) {
        if (localIdx != 1) {
            localIdx++;
            continue;
        }
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
        localIdx++;
    }


    //Load 3 hits of sword-combo
    auto sp3 = readLGPArchive(srGetGamePath(SWORDPLAY_PATH3));
    auto sp3a = loadModelAnimationFromDAFile("CLOUD.DAT", sp3, true);
    localIdx = 0;
    for (auto animation : sp3a) {
        if (localIdx != 1) {
            localIdx++;
            continue;
        }
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
        localIdx++;
    }


    auto ie1= readLGPArchive(srGetGamePath(INFINITY_PATH1));
    auto ie1s = loadModelAnimationFromDAFile("CLOUD.DAT", ie1, true);
    localIdx = 0;
    for (auto animation : ie1s) {
        if (localIdx >= 2) {
            break;
        }
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
        localIdx++;
    }

    //Load 3 hits of sword-combo
    auto ie2 = readLGPArchive(srGetGamePath(INFINITY_PATH2));
    auto ie2s = loadModelAnimationFromDAFile("CLOUD.DAT", ie2, true);
    localIdx = 0;
    for (auto animation : ie2s) {
        if (localIdx != 1) {
            localIdx++;
            continue;
        }
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
        localIdx++;
    }

    auto psp1 = readLGPArchive(srGetGamePath(PUNISHER_PATHH1));
    auto psp1s = loadModelAnimationFromDAFile("CLOUD.DAT", psp1, true);
    localIdx = 0;
    for (auto animation : psp1s) {
        if (localIdx != 1) {
            localIdx++;
            continue;
        }
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
        localIdx++;
    }

    auto sc = readLGPArchive(srGetGamePath(STANCE_PATH));
    auto scs = loadModelAnimationFromDAFile("CLOUD.DAT", sc, true);
    localIdx = 0;
    for (auto animation : scs) {
        if (localIdx != 1) {
            localIdx++;
            continue;
        }
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
        localIdx++;
    }

    auto sp2b = readLGPArchive(srGetGamePath(SWORDPLAY_PATH2b));
    auto sp2ba = loadModelAnimationFromDAFile("CLOUD.DAT", sp2b, true);
    localIdx = 0;
    for (auto animation : sp2ba) {
        if (localIdx != 1) {
            localIdx++;
            continue;
        }
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
        localIdx++;
    }


    auto sp3b = readLGPArchive(srGetGamePath(SWORDPLAY_PATH3b));
    auto sp3ba = loadModelAnimationFromDAFile("CLOUD.DAT", sp3b, true);
    localIdx = 0;
    for (auto animation : sp3ba) {
        if (localIdx != 1) {
            localIdx++;
            continue;
        }
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
        localIdx++;
    }
}

bool punisherActive = false;
bool doSwitchStance = false;
void loadCloudActions() {
    u8 swordPlayScript[34] = { 0xFC, 0xEA, 0xF4, 0x7, 0xF3, 0xF0, 0xD8, 0, 0x1A, 0, 0x2C, 0xD1, 0xB0, 4, 0, 0, 8, 0xF0, 0xA1, 4, 9, 0x2E, 0xA8, 0, 8, 0xF0,
        0xDA, getOpCodeIdx(MOD_NAME, 0), 0, getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 3, "CLOUD.DAT"), getSrPlayerAnimationIdx(MOD_NAME, 13, "CLOUD.DAT"), 0xE5, 0xEE };
    swordPlayScript[10] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 0, "CLOUD.DAT");
    swordPlayScript[21] = getSrPlayerAnimationIdx(MOD_NAME, 8, "CLOUD.DAT");
    //swordPlayScript[24] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 3, "CLOUD.DAT");
    addAnimationScript(MOD_NAME, 0, "CLOUD.DAT", swordPlayScript, 34);

    auto swordPlay = SrActionData();
    swordPlay.name = "Swordplay";
    swordPlay.description = "Flurry of Rapid Strikes. High Chance to Bleed";
    swordPlay.baseData.attackPower = 6;
    swordPlay.baseData.additionalEffect = 0;
    swordPlay.baseData.additionalEffectModifier = 3;
    swordPlay.baseData.animationEffectID = 0xFF;
    swordPlay.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    swordPlay.baseData.abilityHitRate = 0x64;
    swordPlay.baseData.cameraMovementSingle = 0xFFFF;
    swordPlay.baseData.cameraMovementMultiple = 0xFFFF;
    swordPlay.baseData.impactEffectID = 0x13;
    swordPlay.baseData.impactSoundID = 18;
    swordPlay.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(swordPlay, 0, MOD_NAME);
    addElementToAction(MOD_NAME, 0, ElementNames::SLASH.c_str());
    setActionAnimationScript(MOD_NAME, 0, "CLOUD.DAT", MOD_NAME, 0);
    addActionToCommand(MOD_NAME, 0, MOD_NAME, 0);
    addHitModifier(MOD_NAME, 0, CHECK_ACCURACY);

    u8 leapingScript[39] = { 0xFC, 0xEA, 0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 13, 0xF0, 0xD8, 0, 0x1A, 0, 0x2C, 0xD1, 0xB0, 4, 0, 0, 4, 0xF0, 0x2D, 0xD8, 6, 0x30, 0, 0xF7, 15, 0x2E, 0xDA, getOpCodeIdx(MOD_NAME, 2), 0,  7, 0xDA, getOpCodeIdx(MOD_NAME, 1), 0, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE };
    leapingScript[11] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 0, "CLOUD.DAT");
    leapingScript[19] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 1, "CLOUD.DAT");
    leapingScript[26] = getSrPlayerAnimationIdx(BASE_PREFIX, 33, "CLOUD.DAT"); //Use Deathblow Animation
    leapingScript[36] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 3, "CLOUD.DAT");
    addAnimationScript(MOD_NAME, 1, "CLOUD.DAT", leapingScript, 39);

    auto leapSlash = SrActionData();
    leapSlash.name = "Leap Slash";
    leapSlash.description = "Powerful Jumping Slash. Good vs Defense/Block";
    leapSlash.baseData.attackPower = 48;
    leapSlash.baseData.additionalEffect = 0xFF;
    leapSlash.baseData.additionalEffectModifier = 0xFF;
    leapSlash.baseData.animationEffectID = 0xFF;
    leapSlash.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    leapSlash.baseData.abilityHitRate = 0x54;
    leapSlash.baseData.cameraMovementSingle = 0xFFFF;
    leapSlash.baseData.cameraMovementMultiple = 0xFFFF;
    leapSlash.baseData.impactEffectID = 0x13;
    leapSlash.baseData.impactSoundID = 27;
    leapSlash.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(leapSlash, 1, MOD_NAME);
    addElementToAction(MOD_NAME, 1, ElementNames::SLASH.c_str());
    setActionAnimationScript(MOD_NAME, 1, "CLOUD.DAT", MOD_NAME, 1);
    addActionToCommand(MOD_NAME, 0, MOD_NAME, 1);
    addHitModifier(MOD_NAME, 1, CHECK_ACCURACY);

    u8 lacerateScript[28] = { 0xFC, 0xEA, 0xF4, 0x7, 0xF3, 0xF0, 0xD8, 0, 0x1A, 0, 0x2C, 0xD1, 0xB0, 4, 0, 0, 4, 0xF0, 0x2D, 0xA1, 3, 10, 0x2E, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE };
    lacerateScript[10] = getSrPlayerAnimationIdx(MOD_NAME, 0, "CLOUD.DAT");
    lacerateScript[18] = getSrPlayerAnimationIdx(MOD_NAME, 1, "CLOUD.DAT");
    lacerateScript[22] = getSrPlayerAnimationIdx(MOD_NAME, 2, "CLOUD.DAT");
    lacerateScript[25] = getSrPlayerAnimationIdx(MOD_NAME, 3, "CLOUD.DAT");
    addAnimationScript(MOD_NAME, 2, "CLOUD.DAT", lacerateScript, 28);
    
    u8 fthrustScript[40] = { 0xFC, 0xEA, 0xF0, 0x2C, 0xD8, 0, 0x1A, 0, 0xD8, 6, 0x1A, 0, 0xD1, 0xB0, 4, 0, 0, 2, 0xF0, 0x2D, 0xF7, 3, 0x2E, 0xDA, getOpCodeIdx(MOD_NAME, 2), 0,  7, 0xDA, getOpCodeIdx(MOD_NAME, 1), 0, 0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 4, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE };
    fthrustScript[3] = getSrPlayerAnimationIdx(MOD_NAME, 16, "CLOUD.DAT");
    fthrustScript[19] = getSrPlayerAnimationIdx(MOD_NAME, 17, "CLOUD.DAT");
    fthrustScript[22] = getSrPlayerAnimationIdx(MOD_NAME, 18, "CLOUD.DAT");
    fthrustScript[36] = getSrPlayerAnimationIdx(MOD_NAME, 3, "CLOUD.DAT");
    u16* wordInserter = (u16*)&(fthrustScript[10]);
    *wordInserter = 342;
    addAnimationScript(MOD_NAME, 8, "CLOUD.DAT", fthrustScript, 40);

    auto fthrustData = SrActionData();
    fthrustData.name = "F. Thrust";
    fthrustData.description = "Charge an Enemy, dealing high stagger";
    fthrustData.baseData.attackPower = 24;
    fthrustData.baseData.additionalEffect = 0;
    fthrustData.baseData.additionalEffectModifier = 1;
    fthrustData.baseData.animationEffectID = 0xFF;
    fthrustData.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    fthrustData.baseData.abilityHitRate = 0x64;
    fthrustData.baseData.abilityHitRate = 0x64;
    fthrustData.baseData.cameraMovementSingle = 0xFFFF;
    fthrustData.baseData.cameraMovementMultiple = 0xFFFF;
    fthrustData.baseData.impactEffectID = 0x14;
    fthrustData.baseData.impactSoundID = 227;
    fthrustData.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(fthrustData, 8, MOD_NAME);
    addElementToAction(MOD_NAME, 8, ElementNames::PIERCE.c_str());
    setActionAnimationScript(MOD_NAME, 8, "CLOUD.DAT", MOD_NAME, 8);
    addActionToCommand(MOD_NAME, 0, MOD_NAME, 8);

    auto actionData = SrActionData();
    actionData.name = "Lacerate";
    actionData.description = "Flurry of Rapid Strikes. High Chance to Bleed";
    actionData.baseData.attackPower = 4;
    actionData.baseData.additionalEffect = 0;
    actionData.baseData.additionalEffectModifier = 4;
    actionData.baseData.animationEffectID = 0xFF;
    actionData.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    actionData.baseData.abilityHitRate = 0x64;
    actionData.baseData.cameraMovementSingle = 0x43;
    actionData.baseData.cameraMovementMultiple = 0xFFFF;
    actionData.baseData.impactEffectID = 0x13;
    actionData.baseData.impactSoundID = 18;
    actionData.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(actionData, 2, MOD_NAME);
    addElementToAction(MOD_NAME, 2, ElementNames::SLASH.c_str());
    setActionAnimationScript(MOD_NAME, 2, "CLOUD.DAT", MOD_NAME, 2);
    addActionToCommand(MOD_NAME, 0, MOD_NAME, 2);
    addHitModifier(MOD_NAME, 2, CHECK_ACCURACY);

    u8 impaleScript[34] = { 0xFC, 0xEA, 0xF0, 0x2C, 0xD8, 0, 0x1A, 0, 0xD8, 9, 0x1A, 0, 0xA1, 9, 0x1B, 0xD1, 0xB0, 4, 0, 0, 6, 0xF0, 0x2D, 0xD8, 0x19, 0x30, 0, 0x2E, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE };
    impaleScript[3] = getSrPlayerAnimationIdx(MOD_NAME, 4, "CLOUD.DAT");
    impaleScript[22] = getSrPlayerAnimationIdx(MOD_NAME, 5, "CLOUD.DAT");
    impaleScript[27] = getSrPlayerAnimationIdx(MOD_NAME, 6, "CLOUD.DAT");
    impaleScript[30] = getSrPlayerAnimationIdx(MOD_NAME, 3, "CLOUD.DAT");
    wordInserter = (u16*)&(impaleScript[11]);
    *wordInserter = 342;
    addAnimationScript(MOD_NAME, 3, "CLOUD.DAT", impaleScript, 34);

    auto impaleData = SrActionData();
    impaleData.name = "Impale";
    impaleData.description = "Gore an opponent for massive damage";
    impaleData.baseData.attackPower = 24;
    impaleData.baseData.additionalEffect = 0;
    impaleData.baseData.additionalEffectModifier = 2;
    impaleData.baseData.animationEffectID = 0xFF;
    impaleData.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    impaleData.baseData.abilityHitRate = 0x64;
    impaleData.baseData.abilityHitRate = 0x64;
    impaleData.baseData.cameraMovementSingle = 0x25;
    impaleData.baseData.cameraMovementMultiple = 0xFFFF;
    impaleData.baseData.impactEffectID = 0x14;
    impaleData.baseData.impactSoundID = 227;
    impaleData.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(impaleData, 3, MOD_NAME);
    addElementToAction(MOD_NAME, 3, ElementNames::PIERCE.c_str());
    setActionAnimationScript(MOD_NAME, 3, "CLOUD.DAT", MOD_NAME, 3);
    addActionToCommand(MOD_NAME, 0, MOD_NAME, 3);


    u8 ie1[45] = { 0xFC, 0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 1, 0xF0, 0xD8, 0, 0x1A, 0, 0x2C, 0xD1, 0xB0, 4, 0, 0, 7, 0xF0, 0x2D, 0xD8, 6, 0x30, 0, 0xF7, 15, 0x2E, 0xDA, getOpCodeIdx(MOD_NAME, 2), 0,  5, 0xDA, getOpCodeIdx(MOD_NAME, 1), 0, 0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 6, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE, 0xDA, getOpCodeIdx(MOD_NAME, 4), 0 };
    ie1[10] = getSrPlayerAnimationIdx(MOD_NAME, 22, "CLOUD.DAT");
    ie1[18] = getSrPlayerAnimationIdx(MOD_NAME, 23, "CLOUD.DAT");
    ie1[25] = getSrPlayerAnimationIdx(MOD_NAME, 24, "CLOUD.DAT");
    ie1[39] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 3, "CLOUD.DAT");
    addAnimationScript(MOD_NAME, 9, "CLOUD.DAT", ie1, 45);

    auto infinityEnd = SrActionData();
    infinityEnd.name = "Infinity's End";
    infinityEnd.description = "Powerful Jumping Slash. Good vs Defense/Block";
    infinityEnd.baseData.attackPower = 80;
    infinityEnd.baseData.additionalEffect = 0xFF;
    infinityEnd.baseData.additionalEffectModifier = 0xFF;
    infinityEnd.baseData.animationEffectID = 0xFF;
    infinityEnd.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    infinityEnd.baseData.abilityHitRate = 0x34;
    infinityEnd.baseData.cameraMovementSingle = 0xFFFF;
    infinityEnd.baseData.cameraMovementMultiple = 0xFFFF;
    infinityEnd.baseData.impactEffectID = 0x13;
    infinityEnd.baseData.impactSoundID = 27;
    infinityEnd.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(infinityEnd, 9, MOD_NAME);
    addElementToAction(MOD_NAME, 9, ElementNames::SLASH.c_str());
    setActionAnimationScript(MOD_NAME, 9, "CLOUD.DAT", MOD_NAME, 9);
    addActionToCommand(MOD_NAME, 0, MOD_NAME, 9);
    addHitModifier(MOD_NAME, 9, CHECK_ACCURACY);

  
}

void initSwordPlay() {
    u8 sp1s[45] = { 0xFC, 0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 13, 0xF0, 0xD8, 0, 0x1A, 0, 0x2C, 0xD1, 0xB0, 4, 0, 0, 4, 0xF0, 0x2D, 0xD8, 6, 0x30, 0, 0xF7, 4, 0x2E, 0xDA, getOpCodeIdx(MOD_NAME, 2), 0,  5, 0xDA, getOpCodeIdx(MOD_NAME, 1), 0, 0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 6, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE, 0xDA, getOpCodeIdx(MOD_NAME, 4), 0 };
    sp1s[10] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 0, "CLOUD.DAT");
    sp1s[18] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 1, "CLOUD.DAT");
    sp1s[25] = getSrPlayerAnimationIdx(MOD_NAME, 19, "CLOUD.DAT");
    sp1s[39] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 3, "CLOUD.DAT");
    addAnimationScript(MOD_NAME, 10, "CLOUD.DAT", sp1s, 45);

    auto sp1 = SrActionData();
    sp1.name = "Swordplay";
    sp1.description = "Basic Combo String, press square to continue the combo";
    sp1.baseData.attackPower = 7;
    sp1.baseData.additionalEffect = 0xFF;
    sp1.baseData.additionalEffectModifier = 0xFF;
    sp1.baseData.animationEffectID = 0xFF;
    sp1.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    sp1.baseData.abilityHitRate = 0xA4;
    sp1.baseData.cameraMovementSingle = 0xFFFF;
    sp1.baseData.cameraMovementMultiple = 0xFFFF;
    sp1.baseData.impactEffectID = 0x13;
    sp1.baseData.impactSoundID = 18;
    sp1.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(sp1, 10, MOD_NAME);
    addElementToAction(MOD_NAME, 10, ElementNames::SLASH.c_str());
    setActionAnimationScript(MOD_NAME, 10, "CLOUD.DAT", MOD_NAME, 10);
    addActionToCommand(MOD_NAME, 1, MOD_NAME, 10);
    addHitModifier(MOD_NAME, 10, CHECK_ACCURACY);
    addAttackStat(MOD_NAME, 10, "X_CLEAVE_RANGE", 1500);
    addAttackStat(MOD_NAME, 10, "Z_CLEAVE_RANGE", 1500);


    u8 sp2s[45] = { 0xFC, 0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 13, 0xF0, 0xD8, 0, 0x1A, 0, 0x2C, 0xD1, 0xB0, 4, 0, 0, 4, 0xF0, 0x2D, 0xD8, 6, 0x30, 0, 0xF7, 4, 0x2E, 0xDA, getOpCodeIdx(MOD_NAME, 2), 0,  6, 0xDA, getOpCodeIdx(MOD_NAME, 1), 0,  0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 6, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE, 0xDA, getOpCodeIdx(MOD_NAME, 4), 0 };
    //sp1[11] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 0, "CLOUD.DAT");
    sp2s[18] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 1, "CLOUD.DAT");
    sp2s[25] = getSrPlayerAnimationIdx(MOD_NAME, 20, "CLOUD.DAT");
    sp2s[39] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 3, "CLOUD.DAT");
    addAnimationScript(MOD_NAME, 11, "CLOUD.DAT", sp2s, 45);

    auto sp2 = SrActionData();
    sp2.name = "Swordplay";
    sp2.description = "Basic Combo String, press square to continue the combo";
    sp2.baseData.attackPower = 7;
    sp2.baseData.additionalEffect = 0xFF;
    sp2.baseData.additionalEffectModifier = 0xFF;
    sp2.baseData.animationEffectID = 0xFF;
    sp2.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    sp2.baseData.abilityHitRate = 0x94;
    sp2.baseData.cameraMovementSingle = 0xFFFF;
    sp2.baseData.cameraMovementMultiple = 0xFFFF;
    sp2.baseData.impactEffectID = 0x13;
    sp2.baseData.impactSoundID = 18;
    sp2.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(sp2, 11, MOD_NAME);
    addElementToAction(MOD_NAME, 11, ElementNames::SLASH.c_str());
    setActionAnimationScript(MOD_NAME, 11, "CLOUD.DAT", MOD_NAME, 11);
    addActionToCommand(MOD_NAME, 1, MOD_NAME, 11);
    addHitModifier(MOD_NAME, 11, CHECK_ACCURACY);


    u8 sp3s[45] = { 0xFC, 0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 13, 0xF0, 0xD8, 0, 0x1A, 0, 0x2C, 0xD1, 0xB0,
        4, 0, 0, 4, 0xF0, 0x2D, 0xD8, 6, 0x30, 0, 0xF7, 9, 0x2E, 0xDA, getOpCodeIdx(MOD_NAME, 2), 0,  6, 0xDA,
        getOpCodeIdx(MOD_NAME, 1), 0,  0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 6, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE,
        0xDA, getOpCodeIdx(MOD_NAME, 4), 0 };
    //sp1[11] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 0, "CLOUD.DAT");
    sp3s[18] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 1, "CLOUD.DAT");
    sp3s[25] = getSrPlayerAnimationIdx(MOD_NAME, 21, "CLOUD.DAT");
    sp3s[39] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 3, "CLOUD.DAT");
    addAnimationScript(MOD_NAME, 12, "CLOUD.DAT", sp3s, 45);

    auto sp3 = SrActionData();
    sp3.name = "Swordplay";
    sp3.description = "Basic Combo String, press square to continue the combo";
    sp3.baseData.attackPower = 15;
    sp3.baseData.additionalEffect = 0xFF;
    sp3.baseData.additionalEffectModifier = 0xFF;
    sp3.baseData.animationEffectID = 0xFF;
    sp3.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    sp3.baseData.abilityHitRate = 0x84;
    sp3.baseData.cameraMovementSingle = 0xFFFF;
    sp3.baseData.cameraMovementMultiple = 0xFFFF;
    sp3.baseData.impactEffectID = 0x13;
    sp3.baseData.impactSoundID = 18;
    sp3.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(sp3, 12, MOD_NAME);
    addElementToAction(MOD_NAME, 12, ElementNames::SLASH.c_str());
    setActionAnimationScript(MOD_NAME, 12, "CLOUD.DAT", MOD_NAME, 12);
    addActionToCommand(MOD_NAME, 1, MOD_NAME, 12);
    addHitModifier(MOD_NAME, 12, CHECK_ACCURACY);

    u8 sp2bs[45] = { 0xFC, 0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 13, 0xF0, 0xD8, 0, 0x1A, 0, 0x2C, 0xD1, 0xB0, 4, 0, 0, 4, 0xF0, 0x2D, 0xD8, 6, 0x30, 0, 0xF7, 4, 0x2E, 0xDA, getOpCodeIdx(MOD_NAME, 2), 0,  6, 0xDA, getOpCodeIdx(MOD_NAME, 1), 0,  0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 6, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE, 0xDA, getOpCodeIdx(MOD_NAME, 4), 0 };
    //sp1[11] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 0, "CLOUD.DAT");
    sp2bs[18] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 1, "CLOUD.DAT");
    sp2bs[25] = getSrPlayerAnimationIdx(MOD_NAME, 27, "CLOUD.DAT");
    sp2bs[39] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 3, "CLOUD.DAT");
    addAnimationScript(MOD_NAME, 15, "CLOUD.DAT", sp2bs, 45);

    auto sp2b = SrActionData();
    sp2b.name = "Swordplay";
    sp2b.description = "Basic Combo String, press square to continue the combo";
    sp2b.baseData.attackPower = 7;
    sp2b.baseData.additionalEffect = 0xFF;
    sp2b.baseData.additionalEffectModifier = 0xFF;
    sp2b.baseData.animationEffectID = 0xFF;
    sp2b.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    sp2b.baseData.abilityHitRate = 0x94;
    sp2b.baseData.cameraMovementSingle = 0xFFFF;
    sp2b.baseData.cameraMovementMultiple = 0xFFFF;
    sp2b.baseData.impactEffectID = 0x13;
    sp2b.baseData.impactSoundID = 18;
    sp2b.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(sp2b, 15, MOD_NAME);
    addElementToAction(MOD_NAME, 15, ElementNames::SLASH.c_str());
    setActionAnimationScript(MOD_NAME, 15, "CLOUD.DAT", MOD_NAME, 15);
    addActionToCommand(MOD_NAME, 1, MOD_NAME, 15);
    addHitModifier(MOD_NAME, 15, CHECK_ACCURACY);
    addAttackStat(MOD_NAME, 15, "X_CLEAVE_RANGE", 1500);
    addAttackStat(MOD_NAME, 15, "Z_CLEAVE_RANGE", 1500);

    u8 sp3bs[45] = { 0xFC, 0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 13, 0xF0, 0xD8, 0, 0x1A, 0, 0x2C, 0xD1, 0xB0, 4, 0, 0, 4, 0xF0, 0x2D, 0xD8, 6, 0x30, 0, 0xF7, 4, 0x2E, 0xDA, getOpCodeIdx(MOD_NAME, 2), 0,  6, 0xDA, getOpCodeIdx(MOD_NAME, 1), 0,  0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 6, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE, 0xDA, getOpCodeIdx(MOD_NAME, 4), 0 };
    //sp1[11] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 0, "CLOUD.DAT");
    sp3bs[18] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 1, "CLOUD.DAT");
    sp3bs[25] = getSrPlayerAnimationIdx(MOD_NAME, 28, "CLOUD.DAT");
    sp3bs[39] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 3, "CLOUD.DAT");
    addAnimationScript(MOD_NAME, 16, "CLOUD.DAT", sp3bs, 45);

    auto sp3b = SrActionData();
    sp3b.name = "Swordplay";
    sp3b.description = "Basic Combo String, press square to continue the combo";
    sp3b.baseData.attackPower = 7;
    sp3b.baseData.additionalEffect = 0xFF;
    sp3b.baseData.additionalEffectModifier = 0xFF;
    sp3b.baseData.animationEffectID = 0xFF;
    sp3b.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    sp3b.baseData.abilityHitRate = 0x94;
    sp3b.baseData.cameraMovementSingle = 0xFFFF;
    sp3b.baseData.cameraMovementMultiple = 0xFFFF;
    sp3b.baseData.impactEffectID = 0x13;
    sp3b.baseData.impactSoundID = 18;
    sp3b.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(sp3b, 16, MOD_NAME);
    addElementToAction(MOD_NAME, 16, ElementNames::SLASH.c_str());
    setActionAnimationScript(MOD_NAME, 16, "CLOUD.DAT", MOD_NAME, 16);
    addActionToCommand(MOD_NAME, 1, MOD_NAME, 16);
    addHitModifier(MOD_NAME, 16, CHECK_ACCURACY);


    u8 psp1s[47] = { 0xFC, 0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 12, 0xF0, 0xD8, 0, 0x1A, 0, 0xD1, 0xB0, 4, 0, 0, 4,
        0xF0, 0x2D, 0xA1, 4, 9, 0x2E, 0xDA, getOpCodeIdx(MOD_NAME, 2), 0,  6, 0xDA,
        getOpCodeIdx(MOD_NAME, 1), 0,  0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 7, 0xA8, 0, 4,
        0xF0, 0x2F, 0xE5, 0xEE, 0xDA, getOpCodeIdx(MOD_NAME, 4), 0 };
    psp1s[17] = getSrPlayerAnimationIdx(MOD_NAME, 9, "CLOUD.DAT");
    psp1s[21] = getSrPlayerAnimationIdx(MOD_NAME, 25, "CLOUD.DAT");
    psp1s[37] = getSrPlayerAnimationIdx(MOD_NAME, 13, "CLOUD.DAT");

    addAnimationScript(MOD_NAME, 13, "CLOUD.DAT", psp1s, 47);


    auto psp1 = SrActionData();
    psp1.name = "Swordplay";
    psp1.description = "Basic Combo String, press square to continue the combo";
    psp1.baseData.attackPower = 4;
    psp1.baseData.additionalEffect = 0;
    psp1.baseData.additionalEffectModifier = 4;
    psp1.baseData.animationEffectID = 0xFF;
    psp1.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    psp1.baseData.abilityHitRate = 0x84;
    psp1.baseData.cameraMovementSingle = 0xFFFF;
    psp1.baseData.cameraMovementMultiple = 0xFFFF;
    psp1.baseData.impactEffectID = 0x13;
    psp1.baseData.impactSoundID = 18;
    psp1.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(psp1, 13, MOD_NAME);
    addElementToAction(MOD_NAME, 13, ElementNames::SLASH.c_str());
    setActionAnimationScript(MOD_NAME, 13, "CLOUD.DAT", MOD_NAME, 13);
    addActionToCommand(MOD_NAME, 1, MOD_NAME, 13);
    addHitModifier(MOD_NAME, 13, CHECK_ACCURACY);

    //addAttackStat(MOD_NAME, 13, "X_CLEAVE_RANGE", 1800);
    //addAttackStat(MOD_NAME, 13, "Z_CLEAVE_RANGE", 1500);
}

void loadCharMod() {
    loadCloudAnimations();
    registerOpCode(playStanceAnimation, MOD_NAME, 0);
    registerOpCode(OpCodeCombo, MOD_NAME, 1);
    registerOpCode(OpCodeSetComboWaitFrames, MOD_NAME, 2);
    registerOpCode(ComboJumpOpcode, MOD_NAME, 3);
    registerOpCode(ComboIdle, MOD_NAME, 4);
    registerOpCode(handlePunisherSwap, MOD_NAME, 5);
    u8 punisherHurt[8] = { 0xDA, getOpCodeIdx(MOD_NAME, 0), 0, 0xF, getSrPlayerAnimationIdx(MOD_NAME, 14, "CLOUD.DAT"), 0xF2, 0xE5, 0xEE };
    addAnimationScript(MOD_NAME, 6, "CLOUD.DAT", punisherHurt, 8);

    auto commandData = SrCommandData();
    commandData.baseData.singleCameraID = 0xFFFF;
    commandData.baseData.multipleCameraID = 0xFFFF;
    commandData.auxData.animationEffectID = 0xFF;
    commandData.name = "SOLDIER";
    commandData.description = "Use SOLDIER skills";
    addSrCommand(commandData, 0, MOD_NAME);
    u8 punisherIdle[3] = { getSrPlayerAnimationIdx(MOD_NAME, 12, "CLOUD.DAT"), 0xFE, 0xC0 };
    addAnimationScript(MOD_NAME, 5, "CLOUD.DAT", punisherIdle, 3);
    registerSelectCallback(MOD_NAME, 0, cmdSoldierelectHandler);
    registerSetupCallback(MOD_NAME, 0, loadAbility);
    registerSetupCallback(MOD_NAME, 0, applyDamage);

    auto commandMisc = SrCommandData();
    commandMisc.baseData.singleCameraID = 0xFFFF;
    commandMisc.baseData.multipleCameraID = 0xFFFF;
    commandMisc.auxData.animationEffectID = 0xFF;
    commandMisc.name = "Swordplay";
    commandMisc.description = "Attack with equipped weapon";
    addSrCommand(commandMisc, 1, MOD_NAME);

    auto commandStance = SrCommandData();
    commandStance.baseData.singleCameraID = 0xFFFF;
    commandStance.baseData.multipleCameraID = 0xFFFF;
    commandStance.auxData.animationEffectID = 0xFF;
    commandStance.name = "Stance";
    commandStance.description = "switch between character Stances";
    addSrCommand(commandStance, 2, MOD_NAME);
    registerSetupCallback(MOD_NAME, 2, swapStance);


    u8 switchStance[34] = { 0xFC, getSrPlayerAnimationIdx(MOD_NAME, 26, "CLOUD.DAT"), 0xDA, getOpCodeIdx(MOD_NAME, 5), 0, 0xDA, getOpCodeIdx(MOD_NAME, 2), 0,  5, 0xDA, getOpCodeIdx(MOD_NAME, 1), 0,  0xDA, getOpCodeIdx(MOD_NAME, 3), 0, 13,
        0xF0, 0xA8, 0, 4, 0xF0, 0xDA, getOpCodeIdx(MOD_NAME, 0), 0, getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 3, "CLOUD.DAT"), getSrPlayerAnimationIdx(MOD_NAME, 13, "CLOUD.DAT") , 0xE5, 0xEE, 0xDA, getOpCodeIdx(MOD_NAME, 4), 0 };
    addAnimationScript(MOD_NAME, CloudCommands::STANCE_SWITCH, "CLOUD.DAT", switchStance, 34);


    u8 punishCounter[7] = { 0xFC, 0xF7, 4, getSrPlayerAnimationIdx(MOD_NAME, 15, "CLOUD.DAT") , 0xFA, 0xE5, 0xEE };
    addAnimationScript(MOD_NAME, 7, "CLOUD.DAT", punishCounter, 7);
    registerSelectCallback(MOD_NAME, 1, cmdSwordplayelectHandler);
    registerSetupCallback(MOD_NAME, 1, loadAbility);
    registerSetupCallback(MOD_NAME, 1, applyDamage);

    auto punish = SrActionData();
    punish.name = "Punish";
    punish.description = "Quick Punisher Counter";
    punish.baseData.attackPower = 16;
    punish.baseData.additionalEffect = 0xFF;
    punish.baseData.additionalEffectModifier = 0xFF;
    punish.baseData.animationEffectID = 0xFF;
    punish.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    punish.baseData.abilityHitRate = 0x64;
    punish.baseData.cameraMovementSingle = 0xFFFF;
    punish.baseData.cameraMovementMultiple = 0xFFFF;
    punish.baseData.impactEffectID = 0x13;
    punish.baseData.impactSoundID = 18;
    punish.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(punish, 6, MOD_NAME);
    addElementToAction(MOD_NAME, 6, ElementNames::SLASH.c_str());
    setActionAnimationScript(MOD_NAME, 6, "CLOUD.DAT", MOD_NAME, 7);
    addActionToCommand(MOD_NAME, 1, MOD_NAME, 6);
    initSwordPlay();

    loadCloudActions();
    srAddListener(ENABLE_ACTIONS, (SrEventCallback)enableSoldier, MOD_NAME, ENABLE_ACTION_NO_MATERIA);
    //srAddListener(MENU_INPUT_SQUARE, (SrEventCallback)stanceSwapCallback, MOD_NAME, PLAYING_ANIMATION);
    srAddListener(MENU_INPUT_SQUARE, (SrEventCallback)triggerCounter, MOD_NAME, PLAYING_ANIMATION);
    //srAddListener(POST_MODEL_ANIMATION, (SrEventCallback)handlePunisherSwap, MOD_NAME);
    srAddListener(POST_MODEL_ANIMATION, (SrEventCallback)executeCounter, MOD_NAME);
    srAddListener(SET_TARGET_REACTION, (SrEventCallback)useStanceHurtScript, MOD_NAME);
    srAddListener(RUN_ANIM_SCRIPT_OPCODE, (SrEventCallback)setActorWait, MOD_NAME);
    srAddListener(POST_HIT_FORMULA, (SrEventCallback)handlePunisherNoEvade, MOD_NAME);
    srAddListener(SR_COMBO_OPCODE, (SrEventCallback)HandleCloudCombo, MOD_NAME);
    srAddListener(SETUP_MODEL_ANIMATION, (SrEventCallback)smoothCombo, MOD_NAME);
    srAddListener(INIT_BATTLE_ACTORS, (SrEventCallback)cloudInitActors, MOD_NAME);
    srAddListener(ON_TARGETS_SELECTED, (SrEventCallback)setActionInitialTargets, MOD_NAME);
    srAddListener(RETURN_TO_IDLE, (SrEventCallback)unsetCombOnIdle, MOD_NAME);
    initializeLimits();
    initCloudMenu();
}

void initializeLimits() {
    u8 swordPlayScript[31] = { 0xFC, 0xEA, 0xF4, 0, 0xF3, 0xF0, 0xD8, 0, 0x1A, 0, 0xD1, 0xB0, 4, 0, 0, 8, 0x2C, 0xF0, 0xA1, 6, 16,  0x2E, 0xA8, 0, 8, 0xF0, 0x2F, 0xFA, 0xE5, 0xEE };
    swordPlayScript[16] = getSrPlayerAnimationIdx(MOD_NAME, 9, "CLOUD.DAT");
    swordPlayScript[21] = getSrPlayerAnimationIdx(MOD_NAME, 11, "CLOUD.DAT");
    swordPlayScript[26] = getSrPlayerAnimationIdx(MOD_NAME, 13, "CLOUD.DAT");
    addAnimationScript(MOD_NAME, 4, "CLOUD.DAT", swordPlayScript, 31);

    auto swordPlay = SrActionData();
    swordPlay.name = "Swordplay";
    swordPlay.description = "Simple Three Hit Combo. Press Square to Change Stances";
    swordPlay.baseData.attackPower = 8;
    swordPlay.baseData.additionalEffect = 0;
    swordPlay.baseData.additionalEffectModifier = 3;
    swordPlay.baseData.animationEffectID = 0xFF;
    swordPlay.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    swordPlay.baseData.abilityHitRate = 0x64;
    swordPlay.baseData.cameraMovementSingle = 0xFFFF;
    swordPlay.baseData.cameraMovementMultiple = 0xFFFF;
    swordPlay.baseData.impactEffectID = 0x13;
    swordPlay.baseData.impactSoundID = 18;
    swordPlay.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(swordPlay, 4, MOD_NAME);
    addElementToAction(MOD_NAME, 4, ElementNames::SLASH.c_str());
    setActionAnimationScript(MOD_NAME, 4, "CLOUD.DAT", MOD_NAME, 4);

    //addSwapActionToCommand(MOD_NAME, 0, MOD_NAME, 4); // This is the "Punisher" Swordplay Combo
    addSwapActionToCommand(MOD_NAME, 0, MOD_NAME, 4);
    addSwapActionToCommand(MOD_NAME, 0, CLOUD_LIMIT_MOD_NAME, 0);
    addSwapActionToCommand(MOD_NAME, 0, CLOUD_LIMIT_MOD_NAME, 1);
    addSwapActionToCommand(MOD_NAME, 0, CLOUD_LIMIT_MOD_NAME, 2);
}


void initCloudMenu() {
    auto battleMenu = getMenu(BattleMenuWidgetNames::BATTLE_MENU_NAME.c_str());

    CursorContext soldierSelection = { 0, 0, 2, 2, 0, 0, 2, 3, 0, 0, 0, 0, 0, 1 };
    Cursor* soldierCursor = createCursor(&soldierSelection, 8, 364, 45, 190);
    addState(battleMenu, soldierCursor, SOLDIER_STATE_NAME);
    srLogWrite("VALUE OF SOLDIER MENU STATE: %i", getStateByName(battleMenu, SOLDIER_STATE_NAME));
    setStateCursor(battleMenu, getStateByName(battleMenu, SOLDIER_STATE_NAME), *soldierCursor, 0);
    setStateCursor(battleMenu, getStateByName(battleMenu, SOLDIER_STATE_NAME), *soldierCursor, 1);
    setStateCursor(battleMenu, getStateByName(battleMenu, SOLDIER_STATE_NAME), *soldierCursor, 2);
    const auto& contextKeys = std::unordered_set<SrEventContext>({ BATTLE_MENU });
    srAddListener(INIT_BATTLE_MENU, (SrEventCallback)initSoldierCommandViewWidget, MOD_NAME);
    srAddListener(DRAW_BATTLE_MENU, (SrEventCallback)drawSoldierViewWidget, MOD_NAME);
    srAddListener(MENU_INPUT_OK, (SrEventCallback)handleSelectSoldierAction, MOD_NAME, BATTLE_MENU);
    srAddListener(MENU_INPUT_SQUARE, (SrEventCallback)limitSwapUpdater, MOD_NAME, BATTLE_MENU);
}

void initSoldierCommandViewWidget(const MenuInitEvent* menuEvent) {
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menu = menuEvent->menu;
    auto mainWidget = getWidget(menu);
    auto soldierCommandView = srNewWidget(mainWidget, SoldierWidgetNames::SOLDIER_WIDGET_NAME.c_str());

    boxParams = {
        0,
        340,
        640,
        140,
        0.4f
    };
    srNewBoxWidget(soldierCommandView, boxParams, SoldierWidgetNames::SOLDIER_BOX_NAME.c_str());

    std::vector<std::string> names = { SoldierWidgetNames::SOLDIER_GRID_NAME };
    for (u32 idx = 0; idx < names.size(); idx++) {
        auto characterChoice = getStateCursor(menu, getStateByName(menu, SOLDIER_STATE_NAME), idx);
        DrawCursorGridParams gridParams = { BattleMenuWidgetNames::BATTLE_MENU_NAME.c_str(), getStateByName(menu, SOLDIER_STATE_NAME), soldierCommandUpdater, 50, 364, allocateSoldierRow, idx };
        srNewGridWidget(soldierCommandView, gridParams, names[idx].c_str());
    }
}


Widget* allocateSoldierRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto charWidget = srGetAllocatorBase(name);
    moveWidget(charWidget, xCoordinate, yCoordinate);
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.4f };
    srNewTextWidget(charWidget, textParams, "TXT");
    return charWidget;
}

void soldierCommandUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    /*if (self->collectionType != GridWidgetClass()) {
        return;
    }*/
    auto typedPtr = (CursorGridWidget*)self;
    u8 cloudLevel = getGameCharacter(getSrCharacterRecord(ACT_IDX_CLOUD))->level;
    if (flatIndex >= getCommandActionCount(MOD_NAME, 0)) {
        disableWidget(srGetChild(widget, "TXT"));
    }

    updateText(srGetChild(widget, "TXT"), getSrCommandAction(MOD_NAME, 0, flatIndex).name);
    enableWidget(srGetChild(widget, "TXT"));
    updateTextColor(srGetChild(widget, "TXT"), COLOR_WHITE);
}

void limitSwapUpdater(const MenuInputEvent* event) {
    if (event->menuState != getStateByName(event->menu, SOLDIER_STATE_NAME))
        return;

    srLogWrite("Inside LIMIT Toggler");
    /*if (!getActivePartyMember(*BATTLE_ACTIVE_ACTOR_ID).srPartyMember->isLimitActive)
        return;*/

    auto soldierCmdChoice = getStateCursor(event->menu, event->menuState, 0)->context;
    auto flatIndex = (soldierCmdChoice.maxColumnBound * (soldierCmdChoice.relativeRowIndex + soldierCmdChoice.baseRowIndex)) + soldierCmdChoice.relativeColumnIndex;
    toggleSwapActions(MOD_NAME, 0, flatIndex);
}

void drawSoldierViewWidget(const MenuDrawEvent* event) {
    auto menuWidget = getWidget(event->menu);
    if (event->menuState != getStateByName(event->menu, SOLDIER_STATE_NAME)) {
        disableWidget(srGetChild(menuWidget, SoldierWidgetNames::SOLDIER_WIDGET_NAME.c_str()));
        return;
    }
    enableWidget(srGetChild(menuWidget, SoldierWidgetNames::SOLDIER_WIDGET_NAME.c_str()));
    setActiveCursorIndex(event->menu, getStateByName(event->menu, SOLDIER_STATE_NAME), 0);
}


void cmdSoldierelectHandler(SelectCommandEvent* menuEvent) {
    setMenuState(menuEvent->menuObect, getStateByName(menuEvent->menuObect, SOLDIER_STATE_NAME));
    //srLogWrite("Battle Menu State After SOLDIER CALLBACK: %i", menuEvent->menuObect->currentState);
}

void setActionInitialTargets(void* nullEvent) {
    if (getChosenCommandID() == getInternalCommandID(1, MOD_NAME)) {
        cloudComboContext.initialTargetMask = (1 << getChosenActionTargetIndex());
        srLogWrite("SETTING INITIAL CHOSEN TARGETS TO %x", (1 << getChosenActionTargetIndex()));
    }
}


void cmdSwordplayelectHandler(SelectCommandEvent* menuEvent) {
    if (!punisherActive) {
        setChosenActionID(1);
    }
    else {
        setChosenActionID(6);
    }
    setChosenActionMenuIndex(1);
    srLogWrite("SETTING INITIAL SWORDPLAY TARGET MASK TO: %x ", cloudComboContext.initialTargetMask);
    setTargetingFromFlags(TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES, false);
    setMenuState(menuEvent->menuObect, BATTLE_TARGETING_STATE);
}

bool mashinNoob;
void handleSelectSoldierAction(const MenuInputEvent* event) {
    //if (!checkHandlingInput())
        //return;
    if (event->menuState != getStateByName(event->menu, SOLDIER_STATE_NAME))
        return;

    auto soldierCmdChoice = getStateCursor(event->menu, event->menuState, 0)->context;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (soldierCmdChoice.maxColumnBound * (soldierCmdChoice.relativeRowIndex + soldierCmdChoice.baseRowIndex)) + soldierCmdChoice.relativeColumnIndex;
    auto action = getSrCommandAction(MOD_NAME, 0, flatIndex);

    playMenuSound(1);
    setChosenActionID(flatIndex);
    setChosenActionMenuIndex(flatIndex);
    setTargetingFromFlags(action.baseData.targetingFlags, false);
    setMenuState(event->menu, BATTLE_TARGETING_STATE);
    *PREVIOUS_BATTLE_MENU_STATE = getStateByName(event->menu, SOLDIER_STATE_NAME);
}

void swapStance(CommandSetupEvent& event) {
    if (!punisherActive) {
        srLogWrite("RUNNING STANCE CHANGE SETUP CALLBACK");
        auto animEvent = createAnimEvent(event.damageContext->attackerID, 1, getSrPlayerAnimScriptID(MOD_NAME, 14, "CLOUD.DAT"),
            event.damageContext->commandIndex,
            event.damageContext->absAttackIndex,
            0, 0, 0xFFFF);
        animEvent->damageEventQueueIdx = 0xFFFF;
        auto damageEvent = newDamageEvent();
        damageEvent->targetID = event.damageContext->attackerID;
        damageEvent->attackerID = event.damageContext->attackerID;
        damageEvent->damagedAnimScriptIdx = 0;
        damageEvent->actionFlags = 0;
        damageEvent->targetStatusMask = 0;
        auto terminatorEvent = newDamageEvent();
        terminatorEvent->targetID = 0xFF;
        srLogWrite("Created Stance Change Anim Event");
    }
}

OpCodeControlSequence handlePunisherSwap(AnimScriptEvent* modelAnimEvent) {
    if (!punisherActive) {
        srLogWrite("SWITCHING TO PUNISHER STANCE");
        setActorIdleAnim(modelAnimEvent->actorID, getSrPlayerAnimScriptID(MOD_NAME, 5, "CLOUD.DAT"));
        punisherActive = true;
    }
    else {
        setActorIdleAnim(modelAnimEvent->actorID, getSrPlayerAnimScriptID(MOD_NAME, 5, "CLOUD.DAT"));
        punisherActive = false;
    }
    return RUN_NEXT;
}

OpCodeControlSequence playStanceAnimation(AnimScriptEvent* srEvent) {
    u8* byte_C05F80 = (u8*)0xC05F80;
    u32* off_C06008 = (u32*)0xC06008;
    auto& scriptCtx = *(srEvent->scriptContext);
    auto& actorModelState = *(srEvent->battleModelState);
    auto& actorModelState74 = *getBattleModelState74(srEvent->actorID);
    auto& actorRotationData = *getBattleModelRotationData(srEvent->actorID);
    u8 nonPunisherIdx = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    u8 punisherIdx = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    if (punisherActive) {
        actorModelState.runningAnimIdx = punisherIdx;
        srLogWrite("Playing punisher animation");
    }
    else {
        actorModelState.runningAnimIdx = nonPunisherIdx;
        srLogWrite("Playing non-punisher animation");
    }
    actorModelState.field_74 = 0;
    actorModelState74.field_36 = 0;
    actorRotationData.field_0 = 0;
    scriptCtx.isScriptActive = 0;
    srLogWrite("Playing punisher animation");
    return PLAY_ANIM;
}

void useStanceHurtScript(DamageCalculationEvent* srDamageEvent) {
    srLogWrite("SETTING HURT ANIM SCRIPT ID for PUNISHER COUNTER");
    auto& aiContext = *srDamageEvent->aiContext;
    if (srDamageEvent->damageContext->targetID > 3) {
        srLogWrite("invalid cloud actor id");
        return;
    }
    if (aiContext.actorAIStates[srDamageEvent->damageContext->targetID].characterID != ACT_CLOUD) {
        srLogWrite("target is not cloud");
        return;
    }
    if (!punisherActive) {
        srLogWrite("Punisher is not active");
        return;
    }
    if (!(srDamageEvent->damageContext->hitTypeFlags & 1)) {
        srDamageEvent->damageContext->damagedAnimScriptIdx = getSrPlayerAnimScriptID(MOD_NAME, 6, "CLOUD.DAT");
        srLogWrite("Set reaction animation to punisher");
    }
}


bool isCombohandled = false;
bool doCounter = false;
u8 punisherCounterTarget;
void executeCounter(const ModelAnimationEvent* modelAnimEvent) {
    auto animationIdx = modelAnimEvent->ownerModelState->runningAnimIdx;
    if (animationIdx == getSrPlayerAnimationIdx(MOD_NAME, 14, "CLOUD.DAT")) {
        u16 currentFrame = modelAnimEvent->ownerModelState->currentPlayingFrame;
        if (currentFrame == 6) {
            if (!doCounter) {
                srLogWrite("No Counter triggered, stall deactivated");
                deactivateActorStall(punisherCounterTarget);
                return;
            }
            auto targetModelState = getBattleModelState(punisherCounterTarget);
            targetModelState->actorIsNotActing = 1;
            BattleQueueEntry queueEntry = { 0, 0, modelAnimEvent->actorIdx, getInternalCommandID(1, MOD_NAME), 0, (1 << punisherCounterTarget) };
            doCounter = false;
            isCombohandled = false;
            srEnqueueBattleAction(queueEntry);
        }
    }
}

void triggerCounter(const ModelAnimationEvent* modelAnimEvent) {
    srLogWrite("IN TRIGGER COUNTER CALLBACK");
    auto animationIdx = modelAnimEvent->ownerModelState->runningAnimIdx;
    if (animationIdx == getSrPlayerAnimationIdx(MOD_NAME, 14, "CLOUD.DAT")) {
        u16 currentFrame = modelAnimEvent->ownerModelState->currentPlayingFrame;
        //srLogWrite("Current Model Animation Frame: %i", currentFrame);
        if ((getPreviousInputMask() & 8)) {
            srLogWrite("Button held down prior to window. Returning");
            return;
        }

        if (punisherActive && !isCombohandled) {
            if (currentFrame >= 0 && currentFrame < 6) {
                doCounter = true;
            }
        }
    }
}



void cloudInitActors(InitBattleActorEvent* initActorEvent) {
    punisherActive = false;
}

void setActorWait(const AnimScriptEvent* modelAnimEvent) {
    srLogWrite("DEBUG: In Set Punisher Counter Wait");
    if (modelAnimEvent->scriptContext->currentOpCode != 0xD1)
        return;


    punisherCounterTarget = modelAnimEvent->actorID;
    u16 actionTargetMask = getAnimatingActionTargetMask();
    if (maskIsSingleTarget(actionTargetMask)) {
        u8 targetID = getFirstMaskBitIdx(actionTargetMask);
        const auto& target = gContext.battleActors.getActiveBattleActor(targetID);
        if (target.party10 == nullptr)
            return;

        if (punisherActive && target.party10->characterID == ACT_IDX_CLOUD) {
            activateActorStall(modelAnimEvent->actorID, 75);
        }
    }
    srLogWrite("DEBUG: Handle Set Punisher Counter Wait");
}

void handlePunisherNoEvade(DamageCalculationEvent* srDmgCalcEvent) {
    auto& aiContext = *srDmgCalcEvent->aiContext;
    if (srDmgCalcEvent->damageContext->targetID > 3) {
        srLogWrite("invalid cloud actor id");
        return;
    }
    if (aiContext.actorAIStates[srDmgCalcEvent->damageContext->targetID].characterID != ACT_CLOUD) {
        srLogWrite("target is not cloud");
        return;
    }
    if (!punisherActive) {
        srLogWrite("Punisher is not active");
        return;
    }
    if ((srDmgCalcEvent->damageContext->hitTypeFlags & 1)) {
        srDmgCalcEvent->damageContext->hitTypeFlags &= ~1;
        srLogWrite("Cloud cannot dodge in punisher stance, unflagging");
    }
}

// new context for dispatching input
static u16 ComboWaitFrames[10];
static bool ComboModeActive[10];
static InputBuffer inputBuffer;
OpCodeControlSequence OpCodeCombo(AnimScriptEvent* srEvent) {
    srLogWrite("RUNNING COMBO OPCODE, LISTENING FOR INPUT");
    auto& ownerModelState = *srEvent->battleModelState;
    u32* capturedInputMask = ((u32*)0x9A85D4);
    srLogWrite("CAPTURED: %x", *capturedInputMask);
    inputBuffer.inputBuffer.push_back(*capturedInputMask);
    srLogWrite("INPUT BUFFER LOCATION: %x", &inputBuffer);
    if (!ComboWaitFrames[srEvent->actorID]) {
        for (auto inputmask : inputBuffer.inputBuffer) {
            srLogWrite("INPUT BUFFER: %x", inputmask);
        }
        srLogWrite("DISPATCHING COMBO LISTENER");
        ComboEvent comboEvent{ srEvent, &inputBuffer };
        dispatchEvent(SR_COMBO_OPCODE, &comboEvent);
        inputBuffer.inputBuffer.clear();
        return RUN_NEXT;
    }
    --ComboWaitFrames[srEvent->actorID];
    ownerModelState.currentScriptPosition -= 3;
    srLogWrite("LISTENER FRAMES LEFT: %i", ComboWaitFrames[srEvent->actorID]);
    srEvent->scriptContext->isScriptActive = 0;
    return BREAK;
}


OpCodeControlSequence OpCodeSetComboWaitFrames(AnimScriptEvent* srEvent) {
    auto framesToWait = readOpCodeArg8(srEvent->scriptPtr, srEvent->scriptContext, srEvent->battleModelState);
    ComboWaitFrames[srEvent->actorID] = framesToWait;
    return RUN_NEXT;
}

// TODO should be a base utility in the engine, along with general input buffer parsing tools 
bool srParseInputComboInBuffer(InputBuffer* buffer, u32 comboMask) {
    auto& input = buffer->inputBuffer;
    auto first = input[0];
    srLogWrite("PRESS HANDLER");
    for (auto inputmask : input) {
        srLogWrite("INPUT BUFFER: %x", inputmask);
    }
    srLogWrite("Combo Mask: %x, First: %x", comboMask, first);
    if (first == comboMask) {
        srLogWrite("INPUT COMBO HELD DOWN PRIOR TO WINDOW, GIT GUD");
        return false;
    }
    for (auto inputMask : input) {
        srLogWrite("PRESS Comparison result: %x", (inputMask & comboMask) == comboMask);
        if ((inputMask & comboMask) == comboMask) {
            return true;
        }
    }
    return false;
}

bool srParseInputHoldInBuffer(InputBuffer* buffer, u32 comboMask) {
    srLogWrite("HOLD HANDLER");
    auto& input = buffer->inputBuffer;
    auto first = input[0];
    for (auto inputmask : input) {
        srLogWrite("INPUT BUFFER: %x", inputmask);
    }
    if (!(first == comboMask)) {
        srLogWrite("INPUT NOT HELD DOWN PRIOR TO WINDOW, GIT GUD");
        return false;
    }
    for (auto inputMask : input) {
        srLogWrite("HELD Comparison result: %x", (inputMask & comboMask) == comboMask);
        if (!((inputMask & comboMask) == comboMask)) {
            return false;
        }
    }
    return true;
}


void HandleCloudCombo(ComboEvent* srComboEvent) {
    bool CounterHandled = false;

    auto targetMask = cloudComboContext.initialTargetMask;
    srLogWrite("CLOUD COMBO INITIAL TARGET MASK: %x", cloudComboContext.initialTargetMask);
    if (srParseInputComboInBuffer(srComboEvent->inputBuffer, SR_INPUT_L1 | SR_INPUT_SQUARE)) {
        BattleQueueEntry queueEntry = { 0, 0, srComboEvent->animScriptEvent->actorID, getInternalCommandID(0, MOD_NAME), 1, targetMask };
        srEnqueueBattleAction(queueEntry);
        ComboModeActive[srComboEvent->animScriptEvent->actorID] = true;
        return;
    }
    if (srParseInputHoldInBuffer(srComboEvent->inputBuffer, SR_INPUT_L1 | SR_INPUT_SQUARE)) {
        BattleQueueEntry queueEntry = { 0, 0, srComboEvent->animScriptEvent->actorID, CMD_LIMIT, 0, targetMask };
        srEnqueueBattleAction(queueEntry);
        ComboModeActive[srComboEvent->animScriptEvent->actorID] = true;
        return;
    }
    if (srParseInputComboInBuffer(srComboEvent->inputBuffer, SR_INPUT_L1 | SR_INPUT_TRIANGLE)) {
        BattleQueueEntry queueEntry = { 0, 0, srComboEvent->animScriptEvent->actorID, getInternalCommandID(0, MOD_NAME), 2, targetMask };
        srEnqueueBattleAction(queueEntry);
        ComboModeActive[srComboEvent->animScriptEvent->actorID] = true;
        return;
    }
    if (srParseInputHoldInBuffer(srComboEvent->inputBuffer, SR_INPUT_L1 | SR_INPUT_TRIANGLE)) {
        BattleQueueEntry queueEntry = { 0, 0, srComboEvent->animScriptEvent->actorID, CMD_LIMIT, 2, targetMask };
        srEnqueueBattleAction(queueEntry);
        ComboModeActive[srComboEvent->animScriptEvent->actorID] = true;
        return;
    }
    if (srParseInputComboInBuffer(srComboEvent->inputBuffer, SR_INPUT_L1 | SR_INPUT_OK)) {
        BattleQueueEntry queueEntry = { 0, 0, srComboEvent->animScriptEvent->actorID, getInternalCommandID(0, MOD_NAME), 5, targetMask };
        srEnqueueBattleAction(queueEntry);
        ComboModeActive[srComboEvent->animScriptEvent->actorID] = true;
        return;
    }
    if (srParseInputHoldInBuffer(srComboEvent->inputBuffer, SR_INPUT_L1 | SR_INPUT_CANCEL)) {
        BattleQueueEntry queueEntry = { 0, 0, srComboEvent->animScriptEvent->actorID, CMD_LIMIT, 1, targetMask };
        srEnqueueBattleAction(queueEntry);
        ComboModeActive[srComboEvent->animScriptEvent->actorID] = true;
        return;
    }
    if (srParseInputComboInBuffer(srComboEvent->inputBuffer, SR_INPUT_SQUARE)) {
        if (srComboEvent->animScriptEvent->battleModelState->commandID == getInternalCommandID(1, MOD_NAME)) {
            if (!punisherActive) {
                isCombohandled = handleNormalSwordplay(srComboEvent);
                return;
            }
        }
        if (srComboEvent->animScriptEvent->battleModelState->commandID == getInternalCommandID(2, MOD_NAME)) {
            srLogWrite("TARGET MASK ON STANCE CHANCE ATTACK: %x", targetMask);
            if (!punisherActive) {
                BattleQueueEntry queueEntry = { 0, 0, srComboEvent->animScriptEvent->actorID, getInternalCommandID(1, MOD_NAME), 1, targetMask };
                srEnqueueBattleAction(queueEntry);
                ComboModeActive[srComboEvent->animScriptEvent->actorID] = true;
                return;
            }
            BattleQueueEntry queueEntry = { 0, 0, srComboEvent->animScriptEvent->actorID, getInternalCommandID(1, MOD_NAME), 6, targetMask };
            srEnqueueBattleAction(queueEntry);
            ComboModeActive[srComboEvent->animScriptEvent->actorID] = true;
            return;
        }
        ComboModeActive[srComboEvent->animScriptEvent->actorID] = false;
        return;
    }
    else if (srParseInputHoldInBuffer(srComboEvent->inputBuffer, SR_INPUT_SQUARE)) {
        if (srComboEvent->animScriptEvent->battleModelState->commandID == getInternalCommandID(1, MOD_NAME)) {
            if (!punisherActive) {
                isCombohandled = handleNormalSwordplay(srComboEvent, true);
                return;
            }
        }
        ComboModeActive[srComboEvent->animScriptEvent->actorID] = false;
        return;
    }
    if (srParseInputComboInBuffer(srComboEvent->inputBuffer, SR_INPUT_TRIANGLE)) {
        isCombohandled = handleStanceChange(srComboEvent);
        return;
    }
    srLogWrite("NO COMBO DETECTED, Disabling Combo Mode");
    ComboModeActive[srComboEvent->animScriptEvent->actorID] = false;
}

bool handleStanceChange(ComboEvent* srComboEvent) {
    if (srComboEvent->animScriptEvent->battleModelState->commandID == getInternalCommandID(1, MOD_NAME)) {
        if (!punisherActive) {
            srLogWrite("Handling Stance Change");
            BattleQueueEntry queueEntry = { 0, 0, srComboEvent->animScriptEvent->actorID, getInternalCommandID(2, MOD_NAME), 5, 0 };
            srEnqueueBattleAction(queueEntry);
            ComboModeActive[srComboEvent->animScriptEvent->actorID] = true;
            cloudComboContext.lastCommandIdx = getInternalCommandID(2, MOD_NAME);
            return true;
        }
    }
    return false;
}

bool handleNormalSwordplay(ComboEvent* srComboEvent, bool wasHeld) {
    auto targetMask = cloudComboContext.initialTargetMask;
    switch (getBattleModelState74(srComboEvent->animScriptEvent->actorID)->actionIdx) {
    case 1: {
        if (wasHeld) {
            BattleQueueEntry queueEntry = { 0, 0, srComboEvent->animScriptEvent->actorID, getInternalCommandID(1, MOD_NAME), 4, targetMask };
            srEnqueueBattleAction(queueEntry);
        }
        else {
            BattleQueueEntry queueEntry = { 0, 0, srComboEvent->animScriptEvent->actorID, getInternalCommandID(1, MOD_NAME), 2, targetMask };
            srEnqueueBattleAction(queueEntry);
        }
        ComboModeActive[srComboEvent->animScriptEvent->actorID] = true;
        cloudComboContext.lastTargetMask = targetMask;
        return true;
        break;
    }
    case 4:
    case 2: {
        if (wasHeld) {
            BattleQueueEntry queueEntry = { 0, 0, srComboEvent->animScriptEvent->actorID, getInternalCommandID(1, MOD_NAME), 3, targetMask };
            srEnqueueBattleAction(queueEntry);
        }
        else {
            BattleQueueEntry queueEntry = { 0, 0, srComboEvent->animScriptEvent->actorID, getInternalCommandID(1, MOD_NAME), 5, targetMask };
            srEnqueueBattleAction(queueEntry);
        }
        ComboModeActive[srComboEvent->animScriptEvent->actorID] = true;
        cloudComboContext.lastTargetMask = targetMask;
        return true;;
        break;
    }
    }
    ComboModeActive[srComboEvent->animScriptEvent->actorID] = false;
    return false;
}

OpCodeControlSequence ComboJumpOpcode(AnimScriptEvent* srAnimEvent) {
    auto jumpDistance = readOpCodeArg8(srAnimEvent->scriptPtr, srAnimEvent->scriptContext, srAnimEvent->battleModelState);
    if (!ComboModeActive[srAnimEvent->actorID]) {
        return RUN_NEXT;
    }
    srAnimEvent->battleModelState->currentScriptPosition -= 1;
    srAnimEvent->battleModelState->currentScriptPosition += jumpDistance;
    return RUN_NEXT;
}

void unsetCombOnIdle(AnimScriptEvent* srAnimEvent) {
    ComboModeActive[srAnimEvent->actorID] = false;
}

OpCodeControlSequence ComboIdle(AnimScriptEvent* srEvent) {
    u8* G_ACTOR_IDLE_SCRIPTS = (u8*)0xBF2DF8;
    auto& scriptCtx = *srEvent->scriptContext;
    auto& actorModelState74 = *getBattleModelState74(srEvent->actorID);
    auto& actorModelState = *getBattleModelState(srEvent->actorID);
    actorModelState74.field_C &= 0xFFFBu;
    actorModelState.actorIsNotActing = 1;
    actorModelState74.field_C &= 0xFFF7u;
    scriptCtx.scriptPtr = srEvent->animationScriptTable[actorModelState.animScriptIndex];
    setLingeringAnims(srEvent);
    actorModelState.isScriptExecuting = 0;
    actorModelState.currentScriptPosition = 0;
    actorModelState.waitFrames = 0;
    return RUN_NEXT;
}

static u16 lingerAnims[10];
static u16 lingerFrames[10];
void setLingeringAnims(AnimScriptEvent* animEvent) {
    if (animEvent->battleModelState->commandID == getInternalCommandID(1, MOD_NAME)) {
        switch (getBattleModelState74(animEvent->actorID)->actionIdx) {
        case 1: {
            lingerAnims[animEvent->actorID] = getSrPlayerAnimationIdx(MOD_NAME, 19, "CLOUD.DAT");
            lingerFrames[animEvent->actorID] = 8;
            break;
        }
        case 2: {
            lingerAnims[animEvent->actorID] = getSrPlayerAnimationIdx(MOD_NAME, 20, "CLOUD.DAT");
            lingerFrames[animEvent->actorID] = 7;
            break;
        }
        case 4: {
            lingerAnims[animEvent->actorID] = getSrPlayerAnimationIdx(MOD_NAME, 27, "CLOUD.DAT");
            lingerFrames[animEvent->actorID] = 6;
            break;
        }
        case 5: {
            lingerAnims[animEvent->actorID] = getSrPlayerAnimationIdx(MOD_NAME, 28, "CLOUD.DAT");
            lingerFrames[animEvent->actorID] = 11;
            break;
        }
        case 6: {
            lingerAnims[animEvent->actorID] = getSrPlayerAnimationIdx(MOD_NAME, 25, "CLOUD.DAT");
            lingerFrames[animEvent->actorID] = 30;
            break;
        }
        }
    }

    if (animEvent->battleModelState->commandID == getInternalCommandID(2, MOD_NAME)) {
        lingerAnims[animEvent->actorID] = getSrPlayerAnimationIdx(MOD_NAME, 26, "CLOUD.DAT");
        lingerFrames[animEvent->actorID] = 10;
    }
}

void smoothCombo(ModelAnimationEvent* animEvent) {
    if (ComboModeActive[animEvent->actorIdx] && (animEvent->ownerModelState->actorIsNotActing == 1)) {
        animEvent->ownerModelState->runningAnimIdx = lingerAnims[animEvent->actorIdx];
        animEvent->ownerModelState->currentPlayingFrame = lingerFrames[animEvent->actorIdx];
        animEvent->ownerModelState->playedAnimFrames = lingerFrames[animEvent->actorIdx] - 1;
    }
}
