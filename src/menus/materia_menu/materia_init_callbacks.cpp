#include "materia_init_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../../widgets/updaters.h"

using namespace MateriaWidgetNames;

void initMateraCharDataWidget(const MateriaInitEvent* event) {
    const char * menuText;
    auto menuObject = event->menuObject;
    auto mainWidget = menuObject->menuWidget;

    TextWidget* textWidget;
    DrawTextParams textParams;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    PortraitWidget* portraitWidget;

    auto currentMateriaWidget = createWidget(CHAR_DATA_WIDGET_NAME);
    boxParams = {
       0,
       0,
       280,
       92,
       0.3f
    };
    boxWidget = createBoxWidget(boxParams, CHAR_DATA_BOX_NAME);
    addChildWidget(currentMateriaWidget, (Widget*)boxWidget, CHAR_DATA_BOX_NAME);

    drawPortraitParams portraitParams = { 17, 99, 0, 0.2f };
    portraitWidget = createPortraitWidget(portraitParams, PORTRAIT_WIDGET_NAME);
    addChildWidget(currentMateriaWidget, (Widget*)portraitWidget, PORTRAIT_WIDGET_NAME);

    drawHPBarParams hpBarParams = { 103, 30, 0, 0.2f };
    auto HPBarWidget = createHPBarWidget(hpBarParams, HPBAR_WIDGET_NAME);
    addChildWidget(currentMateriaWidget, (Widget*)HPBarWidget, HPBAR_WIDGET_NAME);

    std::vector<std::string> gearNames = { GEAR_SLOT_1_NAME, GEAR_SLOT_2_NAME };
    std::vector<std::string> equippedGearNames = { EQUIPPED_WEAPON, EQUIPPED_ARMOR };
    std::vector<std::string> viewNames = { CHECK_NAME, ARRANGE_NAME };
    std::vector <std::string> slotNames = { GEAR_1_SLOTS, GEAR_2_SLOTS };
    for (int row = 0; row < gearNames.size(); row++) {
        menuText = gContext.gameStrings.materiaMenuTexts.get_string(row);
        setTextParams(&textParams, 267, 32 + (52 * row), menuText, COLOR_TEAL, 0.2f);
        textWidget = createTextWidget(textParams, gearNames[row]);
        addChildWidget(currentMateriaWidget, (Widget*)textWidget, gearNames[row]);

        menuText = gContext.gameStrings.materiaMenuTexts.get_string(row + 2);
        setTextParams(&textParams, 280, 60 + (51 * row), menuText, COLOR_TEAL, 0.2f);
        textWidget = createTextWidget(textParams, viewNames[row]);
        addChildWidget(currentMateriaWidget, (Widget*)textWidget, viewNames[row]);

        /*There is a good chance the slot widget will have to be rewritten, since we want to expand the number of materia*/
        auto slotsParams = { 357, 59 + 50 * row, nullptr, nullptr };
        slotsWidget = createSlotsWidget(slotsParams, slotNames[row]);
        addChildWidget(currentMateriaWidget, (Widget*)slotsWidget, slotNames[row]);

        setTextParams(&textParams, 320, 32 + (54 * row), menuText, COLOR_WHITE, 0.2f);
        textWidget = createTextWidget(textParams, equippedGearNames[row]);
        addChildWidget(currentMateriaWidget, (Widget*)textWidget, equippedGearNames[row]);
    }

    addChildWidget(mainWidget, currentMateriaWidget, CHAR_DATA_WIDGET_NAME);
}

void initMateriaDescWidget(const MateriaInitEvent* event) {
    const char* materiaDescription;
    u16 materiaID;
    auto characterID = getCharacterRecordIndex(*MAT_MENU_PARTY_INDEX];

    TextWidget* textWidget;
    DrawTextParams textParams;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menuObject;
    auto mainWidget = menuObject->menuWidget;

    auto MatDescWidget = createWidget(MATERIA_DESC_WIDGET_NAME);

    boxParams = {
        0,
        92,
        280,
        33,
        0.f
    };
    boxWidget = createBoxWidget(boxParams, MATERIA_DESC_BOX);
    addChildWidget(mainWidget, (Widget*)boxWidget, MATERIA_DESC_BOX);

    materiaID = getMateriaID(characterID, slot, 0);
    materiaDescription = gContext.gameStrings.materia_descriptions.get_resource(materiaID);
    setTextParams(&textParams, 16, 105, materiaDescription, COLOR_WHITE, 0.2f);
    textWidget = createTextWidget(textParams, MATERIA_DESC);
    addChildWidget(MatDescWidget, (Widget*)textWidget, MATERIA_DESC);

    addChildWidget(mainWidget, MatDescWidget, MATERIA_DESC_WIDGET_NAME);
}

//Handles the base display
void initMateriaViewWidget(const MateriaInitEvent* event) {
    auto itemChoiceCursor = getStateCursor(event->menuObject, 1);

    drawGridParams gridParams;
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menuObject;
    auto mainWidget = menuObject->menuWidget;

    auto materiaViewWidget = createWidget(MATERIA_GRID_WIDGET_NAME);

    boxParams = {
        menuWindowConfig[2].drawDistance1,
        menuWindowConfig[2].drawDistance2,
        menuWindowConfig[2].drawDistance3,
        menuWindowConfig[2].drawDistance4,
        0.3f
    };
    boxWidget = createBoxWidget(boxParams, MATERIA_GRID_BOX);
    addChildWidget(materiaViewWidget, (Widget*)boxWidget, MATERIA_GRID_BOX);

    // displayVisibleItemIcon(343, 37 * visibleItem + 9 * baseColumnIndex + 105, itemID, 0, .1f); //Function needs to be overwritten to display correct thing
    // gameDrawAsset(548, 37 * visibleItem + 9 * baseColumnIndex + 114, 213, (u8)textColor, .1f);

    auto normalMateriaViewWidget = createWidget(MATERIA_GRID);
    gridParams = { itemChoiceCursor, &materiaNameViewUpdater, 427, 210 };
    gridWidget = createGridWidget(gridParams, MATERIA_GRID_NAMES, TextWidgetKlass());
    addChildWidget(normalMateriaViewWidget, (Widget*)gridWidget, MATERIA_GRID_NAMES);

    gridParams = { itemChoiceCursor, &materiaSphereViewUpdater, 403, 210 };
    gridWidget = createGridWidget(gridParams, MATERIA_GRID_SPHERES, GameAssetWidgetKlass());
    /*This configuration causes spheres to display, we need to map these*/
    DrawGameAssetParams assetInitParams = {0, 0, 128, 32, 16, 16, 1, 0, 0, 2};
    addChildWidget(normalMateriaViewWidget, (Widget*)gridWidget, MATERIA_GRID_SPHERES, assetInitParams);
    addChildWidget(materiaViewWidget, normalMateriaViewWidget, MATERIA_GRID);

    addChildWidget(mainWidget, materiaViewWidget, MATERIA_GRID_WIDGET_NAME);
}

void initMateriaDataWidget(const MateriaInitEvent* event) {
    DrawTextParams textParams;
    DrawBoxParams boxParams;
    DrawGameAssetParams gameAssetParams;
    DrawNumberParams numberParams;

    const char* menuText;

    auto menuObject = event->menuObject;
    auto mainWidget = menuObject->menuWidget;

    auto materiaDataWidget = createWidget(MATERIA_DATA_WIDGET_NAME);
    auto standardDisplayWidget = createWidget(STANDARD_DISPLAY);

    //Stuff that can change
    setTextParams(&textParams, 280, 60, nullptr, COLOR_WHITE, 0.2f);
    addChildWidget(standardDisplayWidget, (Widget*)createTextWidget(textParams, MATERIA_NAME); , MATERIA_NAME);
    setTextParams(&textParams, 280, 60, nullptr, COLOR_GREEN, 0.2f);
    addChildWidget(standardDisplayWidget, (Widget*)createTextWidget(textParams, MATERIA_NAME, MATERIA_ELEMENT);
    gameAssetParams = MateriaSphere(13, 212, 2, 0.1);
    addChildWidget(standardDisplayWidget, (Widget*)createGameAssetWidget(gameAssetParams, MATERIA_SPHERE), MATERIA_SPHERE);
    setNumberParams(&numberParams, 280, 60, 0, 4 COLOR_WHITE, 0.2f);
    addChildWidget(standardDisplayWidget, (Widget*)createNumberWidget(numberParams, CURRENT_AP), CURRENT_AP);
    setNumberParams(&numberParams, 280, 60, 0, 4, COLOR_WHITE, 0.2f);
    addChildWidget(standardDisplayWidget, (Widget*)createNumberWidget(numberParams, TO_LEVEL_AP), TO_LEVEL_AP);

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
    menuText = gContext.gameStrings.materiaMenuTexts.get_string(6);
    setTextParams(&textParams, 187, 300, menuText, COLOR_TEAL, 0.1f);
    addChildWidget(standardDisplayWidget, (Widget*)createTextWidget(textParams, ABILITIES_LIST_TXT, ABILITIES_LIST_TXT);
    menuText = gContext.gameStrings.materiaMenuTexts.get_string(7);
    setTextParams(&textParams, 8, 300, menuText, COLOR_TEAL, 0.1f);
    addChildWidget(standardDisplayWidget, (Widget*)createTextWidget(textParams, NEXT_LVL_TEXT, EQUIP_EFFECT_TXT);

    /*Static Array Widgets*/
    auto rowCount = 5;
    DrawStaticGridParams staticGridParams = { nullptr, 40, 326, 1, rowCount, 0, 26 }; //consider whether to use an updater here or not for updating the texts being displayed
    StaticGridWidget* abilityListWidget = createStaticGridWidget(staticGridParams, ABILITIES_LIST, TextWidgetKlass());
    addChildWidget(standardDisplayWidget, equipEffectWidget, ABILITIES_LIST);
    DrawStaticGridParams staticGridParams = { nullptr, 200, 326, 1, rowCount, 0, 26 };
    StaticGridWidget* equipEffectWidget = createStaticGridWidget(staticGridParams, EQUIP_EFFECTS); // no class here as we want to seed with custom widgets
    moveWidget(equipEffectWidget, 200, 326);
    for (auto row = 0; row < rowCount ++row) {
        auto name = std::to_string(row);
        auto statEffectRow = createWidget(name); //contains a percent sign, a plus sign, a colored number, and a stat name
        setTextParams(&textParams, 200, 326, menuText, COLOR_WHITE, 0.1f); 
        addChildWidget(statEffectRow, (Widget*)createTextWidget(textParams, std::string("STAT")), std::string("STAT"););
        setNumberParams(&numberParams, 313, 326, 0, 2, COLOR_WHITE, 0.1f);
        addChildWidget(statEffectRow, (Widget*)createNumberWidget(numberParams, std::string("AMT")), std::string("AMT"));
        auto simpleAssetParams = Sign(300, 326, COLOR_WHITE, 0.1f);
        addChildWidget(statEffectRow, (Widget*)createSimpleGameAssetWidget(simpleAssetParams, std::string("SIGN"); std::string("SIGN"));
        simpleAssetParams = Percent(337, 326, COLOR_WHITE, 0.1f);
        addChildWidget(statEffectRow, (Widget*)createSimpleGameAssetWidget(simpleAssetParams, std::string("PCNT"); std::string("PCNT"));
        addChildWidget(equipEffectWidget, statEffectRow, name);
    }
    addChildWidget(standardDisplayWidget, equipEffectWidget, EQUIP_EFFECTS);
    StaticGridWidget* independentBoosts = createStaticGridWidget(staticGridParams, INDEPENDENT_BOOSTS); // no class here as we want to seed with custom widgets
    moveWidget(independentBoosts, 200, 326);
    for (auto row = 0; row < rowCount ++row) {
        auto name = std::to_string(row);
        auto statEffectRow = createWidget(name); //contains a percent sign, a plus sign, a colored number, and a stat name
        setNumberParams(&numberParams, 140, 330, 0, 2, COLOR_WHITE, 2, 0.1f);
        addChildWidget(statEffectRow, (Widget*)createNumberWidget(numberParams, std::string("AMT")), std::string("AMT"));
        auto simpleAssetParams = Sign(127, 326, COLOR_WHITE, 0.1f);
        addChildWidget(statEffectRow, (Widget*)createSimpleGameAssetWidget(simpleAssetParams, std::string("SIGN"); std::string("SIGN"));
        simpleAssetParams = Percent(163, 330, COLOR_WHITE, 0.1f);
        addChildWidget(statEffectRow, (Widget*)createSimpleGameAssetWidget(simpleAssetParams, std::string("PCNT"); std::string("PCNT"));
        addChildWidget(equipEffectWidget, statEffectRow, name);
    }
    addChildWidget(standardDisplayWidget, independentBoosts, INDEPENDENT_BOOSTS);

    addChildWidget(mainWidget, standardDisplayWidget, STANDARD_DISPLAY);
    addChildWidget(mainWidget, materiaDataWidget, MATERIA_DATA_WIDGET_NAME);
}

/*Initializes the command view widget used */
void initCommandViewWidget(const MateriaInitEvent* event){
    auto commandChoiceCursor = getStateCursor(event->menuObject, 3);

    drawGridParams gridParams;
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menuObject;
    auto mainWidget = menuObject->menuWidget;

    auto cmdGridWidget = createWidget(COMMAND_VIEW_WIDGET_NAME);

    boxParams = {
        0x2F,
        0xD6,
        0,
        0x78,
        0.3f
    };
    boxWidget = createBoxWidget(boxParams, CMD_GRID_BOX);
    addChildWidget(cmdGridWidget, (Widget*)boxWidget, CMD_GRID_BOX);

    gridParams = { commandChoiceCursor, &commandNameViewUpdater, 0x2F + 10, 0xD6 + 11 };
    gridWidget = createGridWidget(gridParams, CMD_GRID, TextWidgetKlass());

    addChildWidget(mainWidget, cmdGridWidget, COMMAND_VIEW_WIDGET_NAME);
}

/*Initializes the spell view Widget used*/
void initSpellViewWidget(const MateriaInitEvent* event) {
    Cursor* spellViewCursor;

    drawGridParams gridParams;
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto mainWidget = menuObject->menuWidget;

    auto spellGridWidget = createWidget(SPELL_VIEW_WIDGET_NAME);

    boxParams = {
         0x2F,
         0x157,
         0x1A2,
         0x78,
         0.203f
    };
    boxWidget = createBoxWidget(boxParams, SPELL_VIEW_BOX);
    addChildWidget(spellGridWidget, (Widget*)boxWidget, SPELL_VIEW_BOX);

    spellViewCursor = getStateCursor(event->menuObject, 4);
    gridParams = { spellViewCursor, &spellNameViewUpdater, 0x2F + 35, 0x157 + 13 };
    addChildWidget(spellGridWidget, (Widget*)createGridWidget(gridParams, SPELL_GRID, TextWidgetKlass()), SPELL_GRID);

    spellViewCursor = getStateCursor(event->menuObject, 5);
    gridParams = { spellViewCursor, &summonNameViewUpdater, 0x2F + 93, 0x157 + 13 };
    addChildWidget(spellGridWidget, (Widget*)createGridWidget(gridParams, SUMMON_GRID, TextWidgetKlass()) , SUMMON_GRID);


    spellViewCursor = getStateCursor(event->menuObject, 6);
    gridParams = { spellViewCursor, &eskillNameViewUpdater, 0x2F + 40 , 0x157 + 13 };
    addChildWidget(spellGridWidget, (Widget*)createGridWidget(gridParams, ESKILL_GRID, TextWidgetKlass()), ESKILL_GRID);

    addChildWidget(mainWidget, spellGridWidget, SPELL_VIEW_WIDGET_NAME);
}
