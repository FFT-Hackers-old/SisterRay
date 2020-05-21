#include "char_commands.h"
#include "../battle/battle_engine_api.h"
#include "../gamedata/game_data_interface.h"
#include "../EncodedString.h"
#include "../gamedata/element_names.h"
#include "../menus/menu_interface.h"
#include "../widgets/widgets_api.h"
#include "../events/event_bus_interface.h"
#include "../party/party_interface.h"
#include "../menus/battle_menu/battle_menu_widget_names.h"
#include "../impl.h"

#define MOD_NAME "SR_NEW_COMMANDS"
#define SOLDIER_STATE_NAME "SOLDIER_CMD"


void enableSoldier(const EnableDefaultAbilitiesEvent* const event) {
    if (event->characterIdx != ACT_IDX_CLOUD) {
        return;
    }
    enableCommand(event->characterIdx, 0, getInternalCommandID(0, MOD_NAME));
}

#define LACERATE_PATH  "mods\\cloud\\lacerate.srm"
#define IMPALE_PATH "mods\\cloud\\impale.srm"
#define SWORDPLAY_PATH "mods\\cloud\\swordplay.srm"
#define PUNISHER_PATH "mods\\cloud\\punisher.srm"
#define PUNISHER2_PATH "mods\\cloud\\punisher2.srm"
#define PUNISHER3_PATH "mods\\cloud\\punisher3.srm"

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
}

bool punisherActive = false;
bool doSwitchStance = false;
void loadCloudActions() {
    u8 swordPlayScript[31] = { 0xFC, 0xEA, 0xF4, 0x7, 0xF3, 0xF0, 0xD8, 0, 0x1A, 0, 0x2C, 0xD1, 0xB0, 4, 0, 0, 8, 0xF0, 0xA1, 4, 9, 0x2E, 0xFA, 0xF0,
        0xDA, getOpCodeIdx(MOD_NAME, 0), 0, getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 3, "CLOUD.DAT"), getSrPlayerAnimationIdx(MOD_NAME, 13, "CLOUD.DAT"), 0xE5, 0xEE };
    swordPlayScript[10] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 0, "CLOUD.DAT");
    swordPlayScript[21] = getSrPlayerAnimationIdx(MOD_NAME, 8, "CLOUD.DAT");
    //swordPlayScript[24] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 3, "CLOUD.DAT");
    addAnimationScript(MOD_NAME, 0, "CLOUD.DAT", swordPlayScript, 31);

    auto swordPlay = SrActionData();
    swordPlay.attackName = "Swordplay";
    swordPlay.attackDesc = "Flurry of Rapid Strikes. High Chance to Bleed";
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

    u8 leapingScript[32] = { 0xFC, 0, 0xEA, 0xF4, 7, 0xF3, 0xF0, 0xD8, 0, 0x1A, 0, 0x2C, 0xD1, 0xB0, 4, 0, 0, 4, 0xF0, 0x2D, 0xD8, 6, 0x30, 0, 0xF7, 15, 0x2E, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE };
    leapingScript[11] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 0, "CLOUD.DAT");
    leapingScript[19] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 1, "CLOUD.DAT");
    leapingScript[26] = getSrPlayerAnimationIdx(BASE_PREFIX, 33, "CLOUD.DAT"); //Use Deathblow Animation
    leapingScript[29] = getSrPlayerAnimationIdx(CLOUD_LIMIT_MOD_NAME, 3, "CLOUD.DAT");
    addAnimationScript(MOD_NAME, 1, "CLOUD.DAT", leapingScript, 32);

    auto leapSlash = SrActionData();
    leapSlash.attackName = "Leap Slash";
    leapSlash.attackDesc = "Powerful Jumping Slash. Good vs Defense/Block";
    leapSlash.baseData.attackPower = 48;
    leapSlash.baseData.additionalEffect = 0xFF;
    leapSlash.baseData.additionalEffectModifier = 0xFF;
    leapSlash.baseData.animationEffectID = 0xFF;
    leapSlash.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT | TGT_FLAG_START_ENEMIES;
    leapSlash.baseData.abilityHitRate = 0x64;
    leapSlash.baseData.cameraMovementSingle = 0xFFFF;
    leapSlash.baseData.cameraMovementMultiple = 0xFFFF;
    leapSlash.baseData.impactEffectID = 0x13;
    leapSlash.baseData.impactSoundID = 27;
    leapSlash.baseData.specialAttackFlags = 0xFFFF;
    addSrAction(leapSlash, 1, MOD_NAME);
    addElementToAction(MOD_NAME, 1, ElementNames::SLASH.c_str());
    setActionAnimationScript(MOD_NAME, 1, "CLOUD.DAT", MOD_NAME, 1);
    addActionToCommand(MOD_NAME, 0, MOD_NAME, 1);


    u8 lacerateScript[28] = { 0xFC, 0xEA, 0xF4, 0x7, 0xF3, 0xF0, 0xD8, 0, 0x1A, 0, 0x2C, 0xD1, 0xB0, 4, 0, 0, 4, 0xF0, 0x2D, 0xA1, 3, 10, 0x2E, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE };
    lacerateScript[10] = getSrPlayerAnimationIdx(MOD_NAME, 0, "CLOUD.DAT");
    lacerateScript[18] = getSrPlayerAnimationIdx(MOD_NAME, 1, "CLOUD.DAT");
    lacerateScript[22] = getSrPlayerAnimationIdx(MOD_NAME, 2, "CLOUD.DAT");
    lacerateScript[25] = getSrPlayerAnimationIdx(MOD_NAME, 3, "CLOUD.DAT");
    addAnimationScript(MOD_NAME, 2, "CLOUD.DAT", lacerateScript, 28);

    auto actionData = SrActionData();
    actionData.attackName = "Lacerate";
    actionData.attackDesc = "Flurry of Rapid Strikes. High Chance to Bleed";
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

    u8 impaleScript[34] = { 0xFC, 0, 0xEA, 0xF0, 0x2C, 0xD8, 0, 0x1A, 0, 0xD8, 9, 0x1A, 0, 0xA1, 9, 0x1B, 0xD1, 0xB0, 4, 0, 0, 6, 0xF0, 0x2D, 0xD8, 0x19, 0x30, 0, 0x2E, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE };
    impaleScript[4] = getSrPlayerAnimationIdx(MOD_NAME, 4, "CLOUD.DAT");
    impaleScript[23] = getSrPlayerAnimationIdx(MOD_NAME, 5, "CLOUD.DAT");
    impaleScript[28] = getSrPlayerAnimationIdx(MOD_NAME, 6, "CLOUD.DAT");
    impaleScript[31] = getSrPlayerAnimationIdx(MOD_NAME, 3, "CLOUD.DAT");
    u16* wordInserter = (u16*)&(impaleScript[11]);
    *wordInserter = 342;
    addAnimationScript(MOD_NAME, 3, "CLOUD.DAT", impaleScript, 34);

    auto impaleData = SrActionData();
    impaleData.attackName = "Impale";
    impaleData.attackDesc = "Gore an opponent for massive damage";
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
}

void loadCharMod() {
    loadCloudAnimations();
    registerOpCode(playStanceAnimation, MOD_NAME, 0);
    u8 punisherHurt[8] = { 0xDA, getOpCodeIdx(MOD_NAME, 0), 0, 0xF, getSrPlayerAnimationIdx(MOD_NAME, 14, "CLOUD.DAT"), 0xF2, 0xE5, 0xEE };
    addAnimationScript(MOD_NAME, 6, "CLOUD.DAT", punisherHurt, 8);

    auto commandData = SrCommandData();
    commandData.baseData.singleCameraID = 0xFFFF;
    commandData.baseData.multipleCameraID = 0xFFFF;
    commandData.auxData.animationEffectID = 0xFF;
    commandData.commandName = "SOLDIER";
    commandData.commandDesc = "Use SOLDIER skills";
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
    commandMisc.commandName = "MISC";
    commandMisc.commandDesc = "MISC";
    addSrCommand(commandMisc, 1, MOD_NAME);
    u8 punishCounter[7] = { 0xFC, 0xF7, 4, getSrPlayerAnimationIdx(MOD_NAME, 15, "CLOUD.DAT") , 0xFA, 0xE5, 0xEE };
    addAnimationScript(MOD_NAME, 7, "CLOUD.DAT", punishCounter, 7);
    registerSetupCallback(MOD_NAME, 1, loadAbility);
    registerSetupCallback(MOD_NAME, 1, applyDamage);

    auto punish = SrActionData();
    punish.attackName = "Punish";
    punish.attackDesc = "Quick Punisher Counter";
    punish.baseData.attackPower = 12;
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

    loadCloudActions();
    srAddListener(ENABLE_ACTIONS, (SrEventCallback)enableSoldier, MOD_NAME, ENABLE_ACTION_NO_MATERIA);
    srAddListener(MENU_INPUT_SQUARE, (SrEventCallback)stanceSwapCallback, MOD_NAME, PLAYING_ANIMATION);
    srAddListener(MENU_INPUT_SQUARE, (SrEventCallback)triggerCounter, MOD_NAME, PLAYING_ANIMATION);
    srAddListener(POST_MODEL_ANIMATION, (SrEventCallback)handlePunisherSwap, MOD_NAME);
    srAddListener(POST_MODEL_ANIMATION, (SrEventCallback)executeCounter, MOD_NAME);
    srAddListener(INIT_PLAYER_BATTLE_ACTOR, (SrEventCallback)useStanceHurtScript, MOD_NAME);
    srAddListener(RUN_ANIM_SCRIPT_OPCODE, (SrEventCallback)setActorWait, MOD_NAME);
    initializeLimits();
    initCloudMenu();
}

void initializeLimits() {
    u8 swordPlayScript[28] = { 0xFC,  getSrPlayerAnimationIdx(MOD_NAME, 12, "CLOUD.DAT"), 0xEA, 0xF4, 0, 0xF3, 0xF0, 0xD8, 0, 0x1A, 0, 0xD1, 0xB0, 4, 0, 0, 8, 0x2C, 0xF0, 0xA1, 6, 16, 0x2E, 0xFA, 0xF0, 0x2F, 0xE5, 0xEE };
    swordPlayScript[17] = getSrPlayerAnimationIdx(MOD_NAME, 9, "CLOUD.DAT");
    swordPlayScript[22] = getSrPlayerAnimationIdx(MOD_NAME, 11, "CLOUD.DAT");
    swordPlayScript[25] = getSrPlayerAnimationIdx(MOD_NAME, 13, "CLOUD.DAT");
    addAnimationScript(MOD_NAME, 4, "CLOUD.DAT", swordPlayScript, 28);

    auto swordPlay = SrActionData();
    swordPlay.attackName = "Swordplay";
    swordPlay.attackDesc = "Simple Three Hit Combo. Press Square to Change Stances";
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

    CursorContext soldierSelection = { 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 1 };
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

    updateText(srGetChild(widget, "TXT"), getSrCommandAction(MOD_NAME, 0, flatIndex).attackName);
    enableWidget(srGetChild(widget, "TXT"));
    updateTextColor(srGetChild(widget, "TXT"), COLOR_WHITE);
}

void limitSwapUpdater(const MenuInputEvent* event) {
    if (event->menuState != getStateByName(event->menu, SOLDIER_STATE_NAME))
        return;

    srLogWrite("Inside LIMIT Toggler");
    if (!getActivePartyMember(*BATTLE_ACTIVE_ACTOR_ID).srPartyMember->isLimitActive)
        return;

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
    /* if (enabledSummons[flatIndex].propertiesMask & 2 || enabledSummons[flatIndex].magicIndex == 0xFF) {
        playMenuSound(3);
        return;
    } */
    //DebugBreak();
    playMenuSound(1);
    setChosenActionID(flatIndex);
    setChosenActionMenuIndex(flatIndex);
    setTargetingFromFlags(action.baseData.targetingFlags, false);
    setMenuState(event->menu, BATTLE_TARGETING_STATE);
    *PREVIOUS_BATTLE_MENU_STATE = getStateByName(event->menu, SOLDIER_STATE_NAME);
}

void stanceSwapCallback(const ModelAnimationEvent* modelAnimEvent) {
    u16 frameTolerance = 10;
    if (modelAnimEvent->ownerModelState->commandID == getInternalCommandID(0, MOD_NAME)) {
        //srLogWrite("Inside Stance Input Callback, INPUT RECEIVED");
        auto actionIdx = modelAnimEvent->ownerModelState74->actionIdx;
        if (actionIdx == 0) {
            u16 currentFrame = modelAnimEvent->ownerModelState->currentPlayingFrame;
            //srLogWrite("Current Model Animation Frame: %i", currentFrame);
            if ((getPreviousInputMask() & 8)) {
                //srLogWrite("Button held down prior to window. Returning");
                return;
            }
            if (!punisherActive) {
                //srLogWrite("PUNISHER INACTIVE, DISPATCHING STANCE SWITCH");
                if (currentFrame >= 22 && currentFrame <= 30) {
                    if (mashinNoob)
                        return;

                    doSwitchStance = true;
                    
                    //srLogWrite("DISPATCHING STANCE SWITCH");

                    if (!(getPreviousInputMask() & 8)) {
                        if (doSwitchStance) {
                            //srLogWrite("Mashing Noob, disabling switch");
                            mashinNoob = true;
                            return;
                        }
                    }
                }
            }
        }
    }
}

void handlePunisherSwap(const ModelAnimationEvent* modelAnimEvent) {
    if (modelAnimEvent->ownerModelState->commandID == getInternalCommandID(0, MOD_NAME)) {
        srLogWrite("Inside Stance Input Callback");
        auto actionIdx = modelAnimEvent->ownerModelState74->actionIdx;
        auto battleModel = modelAnimEvent->ownerModelState->modelDataPtr;
        auto playingAnimation = battleModel->animationsTable[modelAnimEvent->ownerModelState->runningAnimIdx];
        if (actionIdx == 0) {
            u16 currentFrame = modelAnimEvent->ownerModelState->currentPlayingFrame;
            if (currentFrame != 31)
                return;

            if (doSwitchStance) {
                srLogWrite("SWITCHING TO PUNISHER STANCE");
                setActorIdleAnim(modelAnimEvent->actorIdx, getSrPlayerAnimScriptID(MOD_NAME, 5, "CLOUD.DAT"));
                toggleSwapActions(MOD_NAME, 0, 0);
                punisherActive = true;
                doSwitchStance = false;
            }
            else {
                srLogWrite("Truncating Stance Change");
                if (!punisherActive) {
                    modelAnimEvent->ownerModelState->playedAnimFrames = playingAnimation->frameCount - 1;
                    modelAnimEvent->ownerModelState->isScriptExecuting = 1;
                }
            }
            mashinNoob = false;
        }
    }
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

void useStanceHurtScript(InitBattleActorEvent* initActorEvent) {
    if (initActorEvent->characterIdx != ACT_IDX_CLOUD)
        return;

    initActorEvent->ownerState->actorBattleVars->damageAnimID = getSrPlayerAnimScriptID(MOD_NAME, 6, "CLOUD.DAT");
}


bool counterHandled = false;
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
            counterHandled = false;
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

        if (punisherActive && !counterHandled) {
            //srLogWrite("PUNISHER INACTIVE, DISPATCHING STANCE SWITCH");
            if (currentFrame >= 0 && currentFrame < 6) {
                doCounter = true;
            }
        }
    }
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
