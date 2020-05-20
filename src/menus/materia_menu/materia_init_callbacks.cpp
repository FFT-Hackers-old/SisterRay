#include "materia_init_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../../widgets/updaters.h"

using namespace MateriaWidgetNames;

void initMateraCharDataWidget(const MenuInitEvent* event) {
    const char * menuText;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto materiaEquipWidget = createWidget(CHAR_DATA_WIDGET_NAME);
    DrawBoxParams boxParams = { 0, 0, 640, 148, 0.3f };
    auto boxWidget = createBoxWidget(boxParams, CHAR_DATA_BOX_NAME);
    addChildWidget(materiaEquipWidget, (Widget*)boxWidget, CHAR_DATA_BOX_NAME);

    drawPortraitParams portraitParams = { 17, 19, 0, 0.2f };
    auto portraitWidget = createPortraitWidget(portraitParams, PORTRAIT_WIDGET_NAME);
    addChildWidget(materiaEquipWidget, (Widget*)portraitWidget, PORTRAIT_WIDGET_NAME);

    drawHPBarParams hpBarParams = { 103, 30, 0, 0.2f };
    auto HPBarWidget = createHPBarWidget(hpBarParams, HPBAR_WIDGET_NAME);
    addChildWidget(materiaEquipWidget, (Widget*)HPBarWidget, HPBAR_WIDGET_NAME);

    DrawStaticGridParams staticGridParams = { &matEquipRowUpdater, 330, 60, 1, 2, 0, 30, &allocateMateriaEquipRow };
    auto charEquipGrid = createStaticGridWidget(staticGridParams, EQUIP_MATERIA_GRID);
    addChildWidget(materiaEquipWidget, (Widget*)charEquipGrid, EQUIP_MATERIA_GRID);

    staticGridParams = { nullptr, 200, 10, 3, 1, 80, 0, &allocateOptionsRow };
    auto optionsGrid = createStaticGridWidget(staticGridParams, MATERIA_OPTIONS);
    updateText(getChild((Widget*)optionsGrid, 0), EncodedString::from_unicode("Set").str());
    updateText(getChild((Widget*)optionsGrid, 1), EncodedString::from_unicode("Check").str());
    updateText(getChild((Widget*)optionsGrid, 2), EncodedString::from_unicode("Arrange").str());
    addChildWidget(materiaEquipWidget, (Widget*)optionsGrid, MATERIA_OPTIONS);

    addChildWidget(mainWidget, materiaEquipWidget, CHAR_DATA_WIDGET_NAME);
}

Widget* allocateOptionsRow(const char* name, i32 x, i32 y) {
    DrawTextParams textParams = { x, y, defaultString, COLOR_WHITE, 0.2f };
    return (Widget*)createTextWidget(textParams, "OPTION");
}

Widget* allocateMateriaEquipRow(const char* name, i32 x, i32 y) {
    auto materiaEquipRow = createWidget("ROW");
    moveWidget(materiaEquipRow, x, y);

    DrawTextParams textParams = { x, y, defaultString, COLOR_TEAL, 0.2f };
    auto textWidget = createTextWidget(textParams, "GEAR");
    addChildWidget(materiaEquipRow, (Widget*)textWidget, "GEAR");

    /*There is a good chance the slot widget will have to be rewritten, since we want to expand the number of materia*/
    drawSlotsParams slotsParams = { x + 70, y, nullptr, nullptr };
    auto slotsWidget = createSlotsWidget(slotsParams, "MAT");
    addChildWidget(materiaEquipRow, (Widget*)slotsWidget, "MAT");
    return materiaEquipRow;
}

void matEquipRowUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != StaticGridWidgetClass()) {
        return;
    }
    auto& gearSlot = gContext.party.getActivePartyCharacter(*MAT_MENU_PARTY_INDEX).equipment.at(flatIndex);

    updateText(getChild(widget, "GEAR"), gearSlot.slotName.str());

    u8* slotsPtr = getMateriaSlots(gearSlot.equippedIdx, gearSlot.slotGearType);
    if (!slotsPtr) {
        disableWidget(widget);
        return;
    }

    updateMateriaSlots(getChild(widget, "MAT"), slotsPtr);
    updateMateriaData(getChild(widget, "MAT"), &(gearSlot.materia[0]));
}

void initMateriaDescWidget(const MenuInitEvent* event) {
    const char* materiaDescription;
    u16 materiaID;
    auto characterID = getCharacterRecordIndex(*MAT_MENU_PARTY_INDEX);

    TextWidget* textWidget;
    DrawTextParams textParams;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto MatDescWidget = createWidget(MATERIA_DESC_WIDGET_NAME);

    boxParams = {
        0,
        148,
        640,
        50,
        0.3f
    };
    boxWidget = createBoxWidget(boxParams, MATERIA_DESC_BOX);
    addChildWidget(mainWidget, (Widget*)boxWidget, MATERIA_DESC_BOX);

    setTextParams(&textParams, 16, 160, defaultString, COLOR_WHITE, 0.2f);
    textWidget = createTextWidget(textParams, MATERIA_DESC);
    addChildWidget(MatDescWidget, (Widget*)textWidget, MATERIA_DESC);

    addChildWidget(mainWidget, MatDescWidget, MATERIA_DESC_WIDGET_NAME);
}

//Handles the base display
void initMateriaViewWidget(const MenuInitEvent* event) {
    auto materiaChoiceCursor = getStateCursor(event->menu, 2);

    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto materiaViewWidget = createWidget(MATERIA_GRID_WIDGET_NAME);

    boxParams = {
        380,
        190,
        260,
        300,
        0.3f
    };
    boxWidget = createBoxWidget(boxParams, MATERIA_GRID_BOX);
    addChildWidget(materiaViewWidget, (Widget*)boxWidget, MATERIA_GRID_BOX);

    auto normalMateriaViewWidget = createWidget(MATERIA_GRID);
    drawGridParams gridParams = { MATERIA_MENU_NAME.c_str(), 2, &materiaEntryUpdater, 427, 210, &allocateMateriaRow, 0 };
    gridWidget = createGridWidget(gridParams, MATERIA_GRID_NAMES);
    addChildWidget(normalMateriaViewWidget, (Widget*)gridWidget, MATERIA_GRID_NAMES);

    addChildWidget(materiaViewWidget, normalMateriaViewWidget, MATERIA_GRID);

    addChildWidget(mainWidget, materiaViewWidget, MATERIA_GRID_WIDGET_NAME);
}

Widget* allocateMateriaRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto materiaWidget = createWidget(name);
    moveWidget(materiaWidget, xCoordinate, yCoordinate);
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.2f };
    addChildWidget(materiaWidget, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));

    DrawGameAssetParams assetInitParams = MateriaSphere(xCoordinate - 20, yCoordinate, 0, 0.2f);
    addChildWidget(materiaWidget, (Widget*)createGameAssetWidget(assetInitParams, std::string("SPH")), std::string("SPH"));
    return materiaWidget;
}

void materiaEntryUpdater(CollectionWidget* self, Widget*widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto textWidget = getChild(widget, std::string("TXT"));
    auto sphereWidget = getChild(widget, std::string("SPH"));
    auto materiaID = gContext.materiaInventory->getResource(flatIndex).materiaID;
    if (materiaID != 0xFFFF) {
        enableWidget(textWidget);
        const char* name = gContext.gameStrings.materia_names.get_string(materiaID);
        updateText(textWidget, name);
        updateTextColor(textWidget, COLOR_WHITE);

        enableWidget(sphereWidget);
        transformAsset(sphereWidget, 128, 32, 16, 16);
        auto materiaAssetType = getMateriaColorType(materiaID);
        updateAssetType(sphereWidget, materiaAssetType);
    }
    else {
        disableWidget(textWidget);
        disableWidget(sphereWidget);
    }
}


void initMateriaDataWidget(const MenuInitEvent* event) {
    DrawTextParams textParams;
    DrawBoxParams boxParams;
    DrawGameAssetParams gameAssetParams;
    DrawNumberParams numberParams;
    DrawStaticGridParams staticGridParams;

    const char* menuText;

    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto materiaDataWidget = createWidget(MATERIA_DATA_WIDGET_NAME);
    auto standardDisplayWidget = createWidget(STANDARD_DISPLAY);

    boxParams = {
      0,
      190,
      680,
      300,
      0.4f
    };
    addChildWidget(materiaDataWidget, (Widget*)createBoxWidget(boxParams, MATERIA_DATA_BOX), MATERIA_DATA_BOX);

    //Stuff that can change
    setTextParams(&textParams, 40, 214, nullptr, COLOR_WHITE, 0.1f);
    addChildWidget(standardDisplayWidget, (Widget*)createTextWidget(textParams, MATERIA_NAME), MATERIA_NAME);
    setTextParams(&textParams, 40, 244, nullptr, COLOR_GREEN, 0.1f);
    addChildWidget(standardDisplayWidget, (Widget*)createTextWidget(textParams, MATERIA_ELEMENT), MATERIA_ELEMENT);
    gameAssetParams = MateriaSphere(13, 212, 0xA, 0.01f);
    addChildWidget(standardDisplayWidget, (Widget*)createGameAssetWidget(gameAssetParams, MATERIA_SPHERE), MATERIA_SPHERE);
    setNumberParams(&numberParams, 238, 244, 0, 7, COLOR_WHITE, 0.1f);
    addChildWidget(standardDisplayWidget, (Widget*)createNumberWidget(numberParams, CURRENT_AP), CURRENT_AP);
    setNumberParams(&numberParams, 250, 270, 0, 7, COLOR_WHITE, 0.1f);
    addChildWidget(standardDisplayWidget, (Widget*)createNumberWidget(numberParams, TO_LEVEL_AP), TO_LEVEL_AP);

    setStaticGridParams(&staticGridParams, 207, 212, 5, 1, 25, 0, nullptr, nullptr);
    StaticGridWidget* starWidget = createStaticGridWidget(staticGridParams, MATERIA_STARS);
    moveWidget((Widget*)starWidget, 207, 212);
    for (auto row = 0; row < 5; ++row) {
        auto name = std::to_string(row);
        gameAssetParams = MateriaStar(207, 212, 0xA, 0.1f, false);
        addChildWidget((Widget*)starWidget, (Widget*)createGameAssetWidget(gameAssetParams, name), name);
    }
    addChildWidget(standardDisplayWidget, (Widget*)starWidget, MATERIA_STARS);

    /*Static Texts*/
    menuText = gContext.gameStrings.materiaMenuTexts.get_string(4);
    setTextParams(&textParams, 108, 270, menuText, COLOR_TEAL, 0.1f);
    addChildWidget(standardDisplayWidget, (Widget*)createTextWidget(textParams, NEXT_LVL_TEXT), NEXT_LVL_TEXT);
    menuText = gContext.gameStrings.materiaMenuTexts.get_string(5);
    setTextParams(&textParams, 207, 244, menuText, COLOR_TEAL, 0.1f);
    addChildWidget(standardDisplayWidget, (Widget*)createTextWidget(textParams, AP_TEXT), AP_TEXT);
    menuText = gContext.gameStrings.materiaMenuTexts.get_string(8);
    setTextParams(&textParams, 250, 244, menuText, COLOR_WHITE, 0.1f);
    addChildWidget(standardDisplayWidget, (Widget*)createTextWidget(textParams, MASTERED), MASTERED);
    menuText = gContext.gameStrings.materiaMenuTexts.get_string(7);
    setTextParams(&textParams, 187, 300, menuText, COLOR_TEAL, 0.1f);
    addChildWidget(standardDisplayWidget, (Widget*)createTextWidget(textParams, ABILITIES_LIST_TXT), ABILITIES_LIST_TXT);
    menuText = gContext.gameStrings.materiaMenuTexts.get_string(6);
    setTextParams(&textParams, 8, 300, menuText, COLOR_TEAL, 0.1f);
    addChildWidget(standardDisplayWidget, (Widget*)createTextWidget(textParams, EQUIP_EFFECT_TXT), EQUIP_EFFECT_TXT);

    /*Static Array Widgets*/
    auto rowCount = 5;
    setStaticGridParams(&staticGridParams, 25, 326, 1, rowCount, 0, 26, nullptr, nullptr); //consider whether to use an updater here or not for updating the texts being displayed
    StaticGridWidget* abilityListWidget = createStaticGridWidget(staticGridParams, ABILITIES_LIST);
    for (auto row = 0; row < rowCount; ++row) {
        auto name = std::to_string(row);
        auto statEffectRow = createWidget(name); //contains a percent sign, a plus sign, a colored number, and a stat name
        moveWidget(statEffectRow, 25, 326);

        setTextParams(&textParams, 25, 326, menuText, COLOR_WHITE, 0.1f);
        addChildWidget(statEffectRow, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
        setNumberParams(&numberParams, 110, 330, 0, 3, COLOR_WHITE, 0.1f);
        addChildWidget(statEffectRow, (Widget*)createNumberWidget(numberParams, std::string("AMT")), std::string("AMT"));
        auto simpleAssetParams = Sign(98, 330, COLOR_WHITE, 0.1f, true);
        addChildWidget(statEffectRow, (Widget*)createSimpleGameAssetWidget(simpleAssetParams, std::string("SIGN")), std::string("SIGN"));
        simpleAssetParams = Percent(145, 330, COLOR_WHITE, 0.1f);
        addChildWidget(statEffectRow, (Widget*)createSimpleGameAssetWidget(simpleAssetParams, std::string("PCNT")), std::string("PCNT"));
        addChildWidget((Widget*)abilityListWidget, (Widget*)statEffectRow, name);
    }
    addChildWidget(standardDisplayWidget, (Widget*)abilityListWidget, ABILITIES_LIST);

    setStaticGridParams(&staticGridParams, 200, 326, 1, rowCount, 0, 26, nullptr, nullptr);
    StaticGridWidget* equipEffectWidget = createStaticGridWidget(staticGridParams, EQUIP_EFFECTS); 
    moveWidget((Widget*)equipEffectWidget, 200, 326);
    for (auto row = 0; row < rowCount; ++row) {
        auto name = std::to_string(row);
        auto statEffectRow = createWidget(name);
        setTextParams(&textParams, 200, 326, menuText, COLOR_WHITE, 0.1f); 
        addChildWidget(statEffectRow, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
        setNumberParams(&numberParams, 313, 326, 0, 3, COLOR_WHITE, 0.1f);
        addChildWidget(statEffectRow, (Widget*)createNumberWidget(numberParams, std::string("AMT")), std::string("AMT"));
        auto simpleAssetParams = Sign(300, 326, COLOR_WHITE, 0.1f);
        addChildWidget(statEffectRow, (Widget*)createSimpleGameAssetWidget(simpleAssetParams, std::string("SIGN")), std::string("SIGN"));
        simpleAssetParams = Percent(337, 326, COLOR_WHITE, 0.1f);
        addChildWidget(statEffectRow, (Widget*)createSimpleGameAssetWidget(simpleAssetParams, std::string("PCNT")), std::string("PCNT"));
        addChildWidget((Widget*)equipEffectWidget, (Widget*)statEffectRow, name);
    }
    addChildWidget(standardDisplayWidget, (Widget*)equipEffectWidget, EQUIP_EFFECTS);

    addChildWidget(materiaDataWidget, standardDisplayWidget, STANDARD_DISPLAY);
    addChildWidget(mainWidget, materiaDataWidget, MATERIA_DATA_WIDGET_NAME);
}

/*Initializes the command view widget used */
void initCommandViewWidget(const MenuInitEvent* event) {
    auto commandChoiceCursor = getStateCursor(event->menu, 3);

    drawGridParams gridParams;
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto commandViewWidget = createWidget(COMMAND_VIEW_WIDGET_NAME);

    boxParams = {
        0x2F,
        0xD6,
        98,
        0x78,
        0.3f
    };
    boxWidget = createBoxWidget(boxParams, CMD_GRID_BOX);
    addChildWidget(commandViewWidget, (Widget*)boxWidget, CMD_GRID_BOX);

    gridParams = { MATERIA_MENU_NAME.c_str(), 3, &commandNameViewUpdater, 0x2F + 10, 0xD6 + 11, nullptr, 0 };
    gridWidget = createGridWidget(gridParams, CMD_GRID, TextWidgetKlass());
    addChildWidget(commandViewWidget, (Widget*)gridWidget, CMD_GRID);

    addChildWidget(mainWidget, commandViewWidget, COMMAND_VIEW_WIDGET_NAME);
}

/*Temporary function until we also provide infrastructure for extending the number of commands*/
void commandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    const auto& commands = gContext.party.getActivePartyMember(*MAT_MENU_PARTY_INDEX).gamePartyMember->enabledCommandArray;;
    auto commandID = commands[flatIndex].commandID;
    if (commandID == 0xFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    updateText(widget, gContext.commands.getResource(commands[flatIndex].commandID).commandName.str());
    updateTextColor(widget, COLOR_WHITE);
}

/*Initializes the spell view Widget used*/
void initSpellViewWidget(const MenuInitEvent* event) {
    drawGridParams gridParams;
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto mainWidget = event->menu->menuWidget;

    auto spellViewWidget = createWidget(SPELL_VIEW_WIDGET_NAME);

    boxParams = {
         0x2F,
         0x157,
         0x1A2,
         0x78,
         0.203f
    };
    boxWidget = createBoxWidget(boxParams, SPELL_VIEW_BOX);
    addChildWidget(spellViewWidget, (Widget*)boxWidget, SPELL_VIEW_BOX);

    gridParams = { MATERIA_MENU_NAME.c_str(), 4, &spellNameViewUpdater, 0x2F + 35, 0x157 + 13, nullptr, 0 };
    addChildWidget(spellViewWidget, (Widget*)createGridWidget(gridParams, SPELL_GRID, TextWidgetKlass()), SPELL_GRID);

    gridParams = { MATERIA_MENU_NAME.c_str(), 5, &summonNameViewUpdater, 0x2F + 93, 0x157 + 13, nullptr, 0 };
    addChildWidget(spellViewWidget, (Widget*)createGridWidget(gridParams, SUMMON_GRID, TextWidgetKlass()) , SUMMON_GRID);

    gridParams = { MATERIA_MENU_NAME.c_str(), 6, &eskillNameViewUpdater, 0x2F + 40 , 0x157 + 13, nullptr, 0 };
    addChildWidget(spellViewWidget, (Widget*)createGridWidget(gridParams, ESKILL_GRID, TextWidgetKlass()), ESKILL_GRID);

    addChildWidget(mainWidget, spellViewWidget, SPELL_VIEW_WIDGET_NAME);
}

void spellNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto& magics = getSrPartyMember(*MAT_MENU_PARTY_INDEX).srPartyMember->actorMagics;
    if (magics[flatIndex].magicIndex == 0xFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    updateText(widget, getCommandAction(CMD_MAGIC, magics[flatIndex].magicIndex).attackName.str());
    updateTextColor(widget, COLOR_WHITE);
}

void summonNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto summons = getSrPartyMember(*MAT_MENU_PARTY_INDEX).srPartyMember->actorSummons;
    if (summons[flatIndex].magicIndex == 0xFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    updateText(widget, getCommandAction(CMD_SUMMON, summons[flatIndex].magicIndex).attackName.str());
    updateTextColor(widget, COLOR_WHITE);
}

void eskillNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto eSkills = getSrPartyMember(*MAT_MENU_PARTY_INDEX).srPartyMember->actorEnemySkills;
    if (eSkills[flatIndex].magicIndex == 0xFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    updateText(widget, getCommandAction(CMD_ENEMY_SKILL, eSkills[flatIndex].magicIndex).attackName.str());
    updateTextColor(widget, COLOR_WHITE);
}
