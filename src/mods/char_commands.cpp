#include "char_commands.h"
#include "../battle/battle_models/model_animations.h"
#include "../battle/battle_models/battle_models_api.h"
#include "../gamedata/game_data_interface.h"
#include "../EncodedString.h"
#include "../gamedata/element_names.h"
#include "../menus/menu_interface.h"
#include "../widgets/widgets_api.h"
#include "../events/event_bus_interface.h"
#include "../party/party_interface.h"
#include "../menus/battle_menu/battle_menu_widget_names.h"
#include "../battle/engine/battle_engine_interface.h"
#include "../impl.h"

#define MOD_NAME "SR_NEW_COMMANDS"
#define SOLDIER_MENU_STATE 38
#define SOLDIER_STATE_NAME "SOLDIER_CMD"


void enableSoldier(const EnableDefaultAbilitiesEvent* const event) {
    if (event->characterIdx != ACT_IDX_CLOUD) {
        return;
    }
    enableCommand(event->characterIdx, 0, getInternalCommandID(0, MOD_NAME));
}

void loadCharMod() {
    auto impaleAnimation = readLGPArchive(srGetGamePath("mods\\CLOUD.DAT\\impale.srm"));
    auto impaleAnims = loadModelAnimationFromDAFile("CLOUD.DAT", impaleAnimation, true);
    auto idx = 0;
    //Load the lacerate animations
    for (auto animation : impaleAnims) {
        if (idx > 2)
            break;
        addPlayerModelAnimation(MOD_NAME, idx, "CLOUD.DAT", animation["BASE"], animation["WPN"]);
        idx++;
    }

    u8 lacerateScript[16] = {};
    addAnimationScript(MOD_NAME, 0, "CLOUD.DAT", lacerateScript, 16);

    auto commandData = SrCommandData();
    commandData.baseData.singleCameraID = 0xFFFF;
    commandData.baseData.multipleCameraID = 0xFFFF;
    commandData.auxData.animationEffectID = 0xFF;
    commandData.commandName = EncodedString::from_unicode("SOLDIER").str();
    addSrCommand(commandData, 0, MOD_NAME);
    registerSelectCallback(MOD_NAME, 0, cmdSoldierelectHandler);
    registerSetupCallback(MOD_NAME, 0, loadAbility);
    registerSetupCallback(MOD_NAME, 0, applyDamage);

    auto actionData = SrActionData();
    actionData.attackName = EncodedString::from_unicode("LACERATE").str();
    actionData.baseData.attackPower = 5;
    actionData.baseData.additionalEffect = 0;
    actionData.baseData.additionalEffectModifier = 4;
    actionData.baseData.animationEffectID = 0xFF;
    actionData.baseData.impactEffectID = 0;
    actionData.baseData.impactSoundID = 0;
    actionData.baseData.targetingFlags = TGT_FLAG_ENABLE_SELECT & TGT_FLAG_START_ENEMIES;
    addSrAction(actionData, 0, MOD_NAME);
    addElementToAction(MOD_NAME, 0, ElementNames::SLASH.c_str());

    srAddListener(ENABLE_ACTIONS, (SrEventCallback)enableSoldier, MOD_NAME, ENABLE_ACTION_NO_MATERIA);

    addActionToCommand(MOD_NAME, 0, MOD_NAME, 0);
}


void initCloudMenu() {
    auto battleMenu = getMenu(BattleMenuWidgetNames::BATTLE_MENU_NAME.c_str());

    CursorContext soldierSelection = { 0, 0, 2, 1, 0, 0, 2, 10, 0, 0, 0, 0, 0, 1 };
    Cursor* soldierCursor = createCursor(&soldierSelection, 8, 364, 22, 120);
    addState(battleMenu, soldierCursor, SOLDIER_STATE_NAME);
    srAddListener(INIT_BATTLE_MENU, (SrEventCallback)initSoldierCommandViewWidget, MOD_NAME);
    srAddListener(DRAW_BATTLE_MENU, (SrEventCallback)drawSoldierViewWidget, MOD_NAME);

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
        drawGridParams gridParams = { BattleMenuWidgetNames::BATTLE_MENU_NAME.c_str(), getStateByName(menu, SOLDIER_STATE_NAME), soldierCommandUpdater, 42, 360, allocateSoldierRow, idx };
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

void drawSoldierViewWidget(const MenuDrawEvent* event) {
    auto menuWidget = getWidget(event->menu);
    if (event->menuState != getStateByName(event->menu, SOLDIER_STATE_NAME)) {
        disableWidget(srGetChild(menuWidget, SoldierWidgetNames::SOLDIER_WIDGET_NAME.c_str()));
        return;
    }
    enableWidget(srGetChild(menuWidget, SoldierWidgetNames::SOLDIER_WIDGET_NAME.c_str()));
}


void cmdSoldierelectHandler(SelectCommandEvent* menuEvent) {
    setMenuState(menuEvent->menuObect, getStateByName(menuEvent->menuObect, SOLDIER_STATE_NAME));
}


void handleSelectSoldierAction(const MenuInputEvent* event) {
    //if (!checkHandlingInput())
        //return;

    if (event->menuState != getStateByName(event->menu, SOLDIER_STATE_NAME));
        return;

    auto soldierCmdChoice = getStateCursor(event->menu, event->menuState, 0)->context;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (soldierCmdChoice.maxColumnBound * (soldierCmdChoice.relativeRowIndex + soldierCmdChoice.baseRowIndex)) + soldierCmdChoice.relativeColumnIndex;
    auto action = getSrCommandAction(MOD_NAME, 0, flatIndex);
    /* if (enabledSummons[flatIndex].propertiesMask & 2 || enabledSummons[flatIndex].magicIndex == 0xFF) {
        playMenuSound(3);
        return;
    } */
    playMenuSound(1);
    setChosenActionID(flatIndex);
    setChosenActionMenuIndex(flatIndex);
    setTargetingFromFlags(action.baseData.targetingFlags, false);
    setMenuState(event->menu, BATTLE_TARGETING_STATE);
    *PREVIOUS_BATTLE_MENU_STATE = getStateByName(event->menu, SOLDIER_STATE_NAME);
}
