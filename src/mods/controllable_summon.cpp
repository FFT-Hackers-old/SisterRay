#include "controllable_summon.h"
#include "../battle/battle_engine_api.h"
#include "../events/event_bus_interface.h"
#include "../impl.h"
#include "../gamedata/summons.h"

#define CONTROL_SUMMON_MOD_NAME "srPlayableSummon"
void loadControllableSummon() {
    registerOpCode(activatePlayableSummon, CONTROL_SUMMON_MOD_NAME, 0);
    u8 newSummonAnimScript[17] = { 0xE8, 0xFC, 3, 0xED, 0xA5, 0xEA, 0xC, 0xD, 0xEC, 0xF4, 0xF, 0xF3, 0xDA, 0, 0, 0xEE };
    u16* wordPtr = (u16*)&newSummonAnimScript[13];
    *wordPtr = getOpCodeIdx(CONTROL_SUMMON_MOD_NAME, 0);
    u8 returnScript[5] = { 0xE, 4, 0xFA, 0xE5, 0xEE };
    for (auto name : characterModelNames) {
        srLogWrite("ADDING NEW SUMMON ANIMATION SCRIPT to model %s", name.c_str());
        addAnimationScript(CONTROL_SUMMON_MOD_NAME, 0, name.c_str(), newSummonAnimScript, 17);
    }
    gContext.commands.getResource(CMD_SUMMON).auxData.animationScriptIndex = getSrPlayerAnimScriptID(CONTROL_SUMMON_MOD_NAME, 0, "CLOUD.DAT");

    gContext.eventBus.addListener(INIT_SUMMON_PARTY_MEMBER, (SrEventCallback)initializeIfrit, std::string(CONTROL_SUMMON_MOD_NAME));
    initPlayableSummons();
}

void initPlayableSummons() {
    loadPlayableSummonAnimations();
    loadPlayableSummonAnimScripts();
    initIfritMenu();
}

void loadPlayableSummonAnimScripts() {
    u8 summonIdle[3] = { getSrPlayerAnimationIdx(CONTROL_SUMMON_MOD_NAME, 0, summonModelNames[2].c_str()),  0xFE, 0xC0 };
    addAnimationScript(CONTROL_SUMMON_MOD_NAME, 0, summonModelNames[2].c_str(), &(summonIdle[0]), 3);
    u8 summonHurt[3] = { 0xF2, 0xE5, 0xEE };
    addAnimationScript(CONTROL_SUMMON_MOD_NAME, 1, summonModelNames[2].c_str(), &(summonHurt[0]), 3);
    u8 summonFiller[3] = { getSrPlayerAnimationIdx(CONTROL_SUMMON_MOD_NAME, 0, summonModelNames[2].c_str()),  0xEE};
    for (auto idx = 0; idx < 26; idx++) {
        addAnimationScript(CONTROL_SUMMON_MOD_NAME, idx + 2, summonModelNames[2].c_str(), &(summonFiller[0]), 2);
    }
    u8 summonCast[12] = { 0xE8, 0xFC, getSrPlayerAnimationIdx(CONTROL_SUMMON_MOD_NAME, 2, summonModelNames[2].c_str()), 0xA5, 0xEA, 6, 0xEC, 0xF4, 0xF, 0xF3, 0xE5, 0xEE };
    addAnimationScript(CONTROL_SUMMON_MOD_NAME, 29, summonModelNames[2].c_str(), &(summonCast[0]), 12);
    srLogWrite("REGISTERING SUMMON ANIMATION, Model : %s has %i animations", summonModelNames[2].c_str(), gContext.battleAnimationScripts.getElement(summonModelNames[2].c_str()).scriptCount);
}



#define IFRIT_PATH1  "mods\\ifrit\\ifreetda1.srm"
#define IFRIT_PATH2  "mods\\ifrit\\ifreetda2.srm"
#define IFRIT_PATH3  "mods\\ifrit\\ifreetda3.srm"
void loadPlayableSummonAnimations() {
    srLogWrite("Loading Summon Model Animations");
    auto ifritAnim1 = readLGPArchive(srGetGamePath(IFRIT_PATH1));
    auto ifritAnims1 = loadModelAnimationFromDAFile(summonModelNames[2].c_str(), ifritAnim1, true);
    auto idx = 0;

    for (auto animation : ifritAnims1) {
        if (idx > 2)
            break;
        addModelAnimation(CONTROL_SUMMON_MOD_NAME, idx, summonModelNames[2].c_str(), animation["BASE"]);
        idx++;
    }

    auto ifritAnim2 = readLGPArchive(srGetGamePath(IFRIT_PATH2));
    auto ifritAnims2 = loadModelAnimationFromDAFile(summonModelNames[2].c_str(), ifritAnim2, true);

    u8 localIdx = 0;
    for (auto animation : ifritAnims2) {
        if (localIdx > 2)
            break;
        addModelAnimation(CONTROL_SUMMON_MOD_NAME, idx, summonModelNames[2].c_str(), animation["BASE"]);
        idx++;
        localIdx++;
    }


    auto ifritAnim3 = readLGPArchive(srGetGamePath(IFRIT_PATH3));
    auto ifritAnims3 = loadModelAnimationFromDAFile(summonModelNames[2].c_str(), ifritAnim3, true);
    localIdx = 0;
    for (auto animation : ifritAnims3) {
        if (localIdx > 2)
            break;
        addModelAnimation(CONTROL_SUMMON_MOD_NAME, idx, summonModelNames[2].c_str(), animation["BASE"]);
        idx++;
        localIdx++;
    }
}

void createIfritCommand() {
    auto commandData = SrCommandData();
    commandData.baseData.singleCameraID = 0xFFFF;
    commandData.baseData.multipleCameraID = 0xFFFF;
    commandData.auxData.animationEffectID = 0xFF;
    commandData.commandName = "Ravage";
    commandData.commandDesc = "Ravage the opponent";
    addSrCommand(commandData, 1, CONTROL_SUMMON_MOD_NAME);
    registerSelectCallback(CONTROL_SUMMON_MOD_NAME, 0, cmdRavageSelectHandler);
    registerSetupCallback(CONTROL_SUMMON_MOD_NAME, 0, loadAbility);
    registerSetupCallback(CONTROL_SUMMON_MOD_NAME, 0, applyDamage);

    auto inferno = SrCommandData();
    inferno.baseData.singleCameraID = 0xFFFF;
    inferno.baseData.multipleCameraID = 0xFFFF;
    inferno.auxData.animationEffectID = 0xFF;
    inferno.commandName = "Inferno";
    inferno.commandDesc = "Use Inferno skills";
    addSrCommand(inferno, 2, CONTROL_SUMMON_MOD_NAME);
    registerSelectCallback(CONTROL_SUMMON_MOD_NAME, 0, cmdInfernoSelectHandler);
    registerSetupCallback(CONTROL_SUMMON_MOD_NAME, 0, loadAbility);
    registerSetupCallback(CONTROL_SUMMON_MOD_NAME, 0, applyDamage);
}


#define INFERNO_STATE_NAME "INFERNO_CMD"
void cmdInfernoSelectHandler(SelectCommandEvent* menuEvent) {
    setMenuState(menuEvent->menuObect, getStateByName(menuEvent->menuObect, INFERNO_STATE_NAME));
}



#define RAVAGE_STATE_NAME "RAVAGE_CMD"
void cmdRavageSelectHandler(SelectCommandEvent* menuEvent) {
    setMenuState(menuEvent->menuObect, getStateByName(menuEvent->menuObect, RAVAGE_STATE_NAME));
}


# define SUMMON_ACTOR_IDX  1
OpCodeControlSequence activatePlayableSummon(AnimScriptEvent* srEvent) {
    auto& ownerModelState = *getBattleModelState(SUMMON_ACTOR_IDX);
    auto& ownerModelState74 = *getBattleModelState74(SUMMON_ACTOR_IDX);
    auto& scriptCtx = *srEvent->scriptContext;
    //auto characterID = ownerModelState74.actionIdx;
    auto spellEffectIdx = getBattleModelState(getActionActorIdx())->animationEffect;
    auto modelName = getSummonModelNameFromEffectIdx(spellEffectIdx);
    srLogWrite("Switching party for new summon model: %s", modelName.c_str());
    std::vector<char> chars(modelName.begin(), modelName.end());
    chars.push_back(0);
    char* cModelName = &(chars[0]);
    auto summonModel = srSetSummonModel(6, cModelName);
    // srSetPlayerModel(0, 1, SUMMON_ACTOR_IDX, const_cast<char*>(modelName.c_str()));
    if (getActiveModelFromArray(SUMMON_ACTOR_IDX)) {
        srDestroyBattleModel(getActiveModelFromArray(SUMMON_ACTOR_IDX));
        setActiveModelFromArray(SUMMON_ACTOR_IDX, summonModel);
    }
    if (getActiveModelFromArray(SUMMON_ACTOR_IDX))
        ownerModelState.modelDataPtr = getActiveModelFromArray(SUMMON_ACTOR_IDX);

    ownerModelState.runningAnimIdx = getSrPlayerAnimationIdx(CONTROL_SUMMON_MOD_NAME, 0, summonModelNames[2].c_str());
    ownerModelState.playedAnimFrames = 0;
    ownerModelState.currentPlayingFrame = 0;
    ownerModelState.tableRelativeModelAnimIdx = 0;
    ownerModelState.setForLimitBreaks = 0;
    auto newModelDataPtr = getActiveModelFromArray(SUMMON_ACTOR_IDX)->animScriptStruct;// get the start value
    //copyDataFromBFile(SUMMON_ACTOR_IDX);
    ownerModelState.field_25 &= 0xFDu;
    ownerModelState74.field_C &= 0xFFFBu;
    ownerModelState.actorIsNotActing = 1;
    ownerModelState74.field_C &= 0xFFF7u;
    ownerModelState.animScriptIndex = 1;
    scriptCtx.scriptPtr = srEvent->animationScriptTable[ownerModelState.animScriptIndex];
    ownerModelState.isScriptExecuting = 0;
    ownerModelState.currentScriptPosition = 0;
    ownerModelState.waitFrames = 0;
    setActorIdleAnim(1, 1);
    setActorHurtAnim(1, 2);
    //getSrPlayerAnimScriptID(SUMMON_MOD_NAME, 1, summonModelNames[2].c_str());
    return BREAK;
}

void initializeIfrit(InitSummonEvent* summEvent) {
    if (summEvent->summonIdx != 2)
        return;

    gContext.party.clearSummonActions(2);
    enableSummonCommand(2, 0, getInternalCommandID(1, CONTROL_SUMMON_MOD_NAME));
    enableSummonCommand(2, 1, getInternalCommandID(2, CONTROL_SUMMON_MOD_NAME));
    enableSummonCommand(2, 2, 2);
    enableSummonMagic(2, 0, 27);
    enableSummonMagic(2, 1, 28);
    enableSummonMagic(2, 2, 29);
    auto& stats = summEvent->summonState->srPartyMember->playerStats;

    stats[StatNames::HP].baseValue = 8530;
    stats[StatNames::MP].baseValue = 630;
    stats[StatNames::STRENGTH].baseValue = 120;
    stats[StatNames::VITALITY].baseValue = 75;
    stats[StatNames::MAGIC].baseValue = 97;
    stats[StatNames::SPIRIT].baseValue = 65;
    stats[StatNames::DEXTERITY].baseValue = 75;
    stats[StatNames::LUCK].baseValue = 53;

    stats[StatNames::HP].statValue = 7530;
    stats[StatNames::MP].statValue = 630;
    stats[StatNames::STRENGTH].statValue = 120;
    stats[StatNames::VITALITY].statValue = 75;
    stats[StatNames::MAGIC].statValue = 97;
    stats[StatNames::SPIRIT].statValue = 65;
    stats[StatNames::DEXTERITY].statValue = 75;
    stats[StatNames::LUCK].statValue = 53;
    stats[StatNames::FIRE_AFF].statValue = 100;
    stats[StatNames::FIRE_RES].statValue = 200;

    auto& gamePartyMember = *summEvent->summonState->gamePartyMember;
    gamePartyMember.characterID = 0xFF;
    gamePartyMember.maxHP = stats[StatNames::HP].statValue;
    gamePartyMember.maxMP = stats[StatNames::MP].statValue;
    gamePartyMember.currentHP = stats[StatNames::HP].statValue;
    gamePartyMember.currentMP = stats[StatNames::MP].statValue;
    gamePartyMember.physAttack = stats[StatNames::STRENGTH].statValue;
    gamePartyMember.physDefense = stats[StatNames::VITALITY].statValue;
    gamePartyMember.magAttack = stats[StatNames::MAGIC].statValue;
    gamePartyMember.magDefense = stats[StatNames::SPIRIT].statValue;
    gamePartyMember.strength = stats[StatNames::STRENGTH].statValue;
    gamePartyMember.vitality = stats[StatNames::VITALITY].statValue;
    gamePartyMember.magic = stats[StatNames::MAGIC].statValue;
    gamePartyMember.spirit = stats[StatNames::SPIRIT].statValue;
    gamePartyMember.speed = stats[StatNames::DEXTERITY].statValue;
    gamePartyMember.luck = stats[StatNames::LUCK].statValue;
}

void initIfritMenu() {
    auto battleMenu = getMenu(BattleMenuWidgetNames::BATTLE_MENU_NAME.c_str());

    CursorContext inferno = { 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 1 };
    Cursor* infernoCursor = createCursor(&inferno, 8, 364, 45, 190);
    addState(battleMenu, infernoCursor, INFERNO_STATE_NAME);
    srLogWrite("VALUE OF SOLDIER MENU STATE: %i", getStateByName(battleMenu, INFERNO_STATE_NAME));
    setStateCursor(battleMenu, getStateByName(battleMenu, INFERNO_STATE_NAME), *infernoCursor, 0);
    setStateCursor(battleMenu, getStateByName(battleMenu, INFERNO_STATE_NAME), *infernoCursor, 1);
    setStateCursor(battleMenu, getStateByName(battleMenu, INFERNO_STATE_NAME), *infernoCursor, 2);
    const auto& contextKeys = std::unordered_set<SrEventContext>({ BATTLE_MENU });
    srAddListener(INIT_BATTLE_MENU, (SrEventCallback)initInfernoViewWidget, CONTROL_SUMMON_MOD_NAME);
    srAddListener(DRAW_BATTLE_MENU, (SrEventCallback)drawInfernoViewWidget, CONTROL_SUMMON_MOD_NAME);
    srAddListener(MENU_INPUT_OK, (SrEventCallback)handleSelectInfernoAction, CONTROL_SUMMON_MOD_NAME, BATTLE_MENU);
}

void initInfernoViewWidget(const MenuInitEvent* menuEvent) {
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menu = menuEvent->menu;
    auto mainWidget = getWidget(menu);
    auto soldierCommandView = srNewWidget(mainWidget, InfernoWidgetNames::INFERNO_WIDGET_NAME.c_str());

    boxParams = {
        0,
        340,
        640,
        140,
        0.4f
    };
    srNewBoxWidget(soldierCommandView, boxParams, InfernoWidgetNames::INFERNO_BOX_NAME.c_str());

    std::vector<std::string> names = { InfernoWidgetNames::INFERNO_GRID_NAME };
    for (u32 idx = 0; idx < names.size(); idx++) {
        auto characterChoice = getStateCursor(menu, getStateByName(menu, INFERNO_STATE_NAME), idx);
        drawGridParams gridParams = { BattleMenuWidgetNames::BATTLE_MENU_NAME.c_str(), getStateByName(menu, INFERNO_STATE_NAME), infernoCommandUpdater, 50, 364, allocateInfernoRow, idx };
        srNewGridWidget(soldierCommandView, gridParams, names[idx].c_str());
    }
}


Widget* allocateInfernoRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto charWidget = srGetAllocatorBase(name);
    moveWidget(charWidget, xCoordinate, yCoordinate);
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.4f };
    srNewTextWidget(charWidget, textParams, "TXT");
    return charWidget;
}

void infernoCommandUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    /*if (self->collectionType != GridWidgetClass()) {
        return;
    }*/
    auto typedPtr = (CursorGridWidget*)self;
    if (flatIndex >= getCommandActionCount(CONTROL_SUMMON_MOD_NAME, 0)) {
        disableWidget(srGetChild(widget, "TXT"));
    }

    updateText(srGetChild(widget, "TXT"), getSrCommandAction(CONTROL_SUMMON_MOD_NAME, 2, flatIndex).attackName);
    enableWidget(srGetChild(widget, "TXT"));
    updateTextColor(srGetChild(widget, "TXT"), COLOR_WHITE);
}

void drawInfernoViewWidget(const MenuDrawEvent* event) {
    auto menuWidget = getWidget(event->menu);
    if (event->menuState != getStateByName(event->menu, INFERNO_STATE_NAME)) {
        disableWidget(srGetChild(menuWidget, InfernoWidgetNames::INFERNO_WIDGET_NAME.c_str()));
        return;
    }
    enableWidget(srGetChild(menuWidget, InfernoWidgetNames::INFERNO_WIDGET_NAME.c_str()));
    setActiveCursorIndex(event->menu, getStateByName(event->menu, INFERNO_STATE_NAME), 0);
}

void handleSelectInfernoAction(const MenuInputEvent* event) {
    //if (!checkHandlingInput())
        //return;
    if (event->menuState != getStateByName(event->menu, INFERNO_STATE_NAME))
        return;

    auto infernoCMDChoice = getStateCursor(event->menu, event->menuState, 0)->context;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (infernoCMDChoice.maxColumnBound * (infernoCMDChoice.relativeRowIndex + infernoCMDChoice.baseRowIndex)) + infernoCMDChoice.relativeColumnIndex;
    auto action = getSrCommandAction(CONTROL_SUMMON_MOD_NAME, 2, flatIndex);

    playMenuSound(1);
    setChosenActionID(flatIndex);
    setChosenActionMenuIndex(flatIndex);
    setTargetingFromFlags(action.baseData.targetingFlags, false);
    setMenuState(event->menu, BATTLE_TARGETING_STATE);
    *PREVIOUS_BATTLE_MENU_STATE = getStateByName(event->menu, INFERNO_STATE_NAME);
}
