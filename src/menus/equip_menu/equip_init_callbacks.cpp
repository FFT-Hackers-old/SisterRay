#include "equip_init_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../../widgets/updaters.h"
#include "../../gamedata/stat_names.h"

/* This Module contains code which initializes the Widget that represents the equip menu internally
   You can mutate this widget prior to draw by registered your listeners to the "OnEquipMenuDraw" event
   The Widget is also mutated by listeners registered to the Equip-Menu's input Handlers.
*/

using namespace EquipWidgetNames;

void initCharDataWidget(const MenuInitEvent* event) {
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto currentEquipWidget = createWidget(CHAR_DATA_WIDGET_NAME);

    DrawBoxParams boxParams = {
       equipMenuWindowConfig[0].x,
       equipMenuWindowConfig[0].y,
       equipMenuWindowConfig[0].width,
       equipMenuWindowConfig[0].height,
       0.3f
    };

    auto boxWidget = createBoxWidget(boxParams, CHAR_DATA_BOX_NAME);
    addChildWidget(currentEquipWidget, (Widget*)boxWidget, CHAR_DATA_BOX_NAME);

    drawPortraitParams portraitParams = { 17, 9, 0, 0.2f };
    auto portraitWidget = createPortraitWidget(portraitParams, PORTRAIT_WIDGET_NAME);
    addChildWidget(currentEquipWidget, (Widget*)portraitWidget, PORTRAIT_WIDGET_NAME);

    drawHPBarParams hpBarParams = { 110, 17, 0, 0.2f };
    auto HPBarWidget = createHPBarWidget(hpBarParams, HPBAR_WIDGET_NAME);
    addChildWidget(currentEquipWidget, (Widget*)HPBarWidget, HPBAR_WIDGET_NAME);

    DrawStaticGridParams staticGridParams = { &equipRowUpdater, 265, 13, 1, 3, 0, 36, &allocateEquipRow };
    auto charEquipGrid = createStaticGridWidget(staticGridParams, EQUIPMENT_GRID);
    addChildWidget(currentEquipWidget, (Widget*)charEquipGrid, EQUIPMENT_GRID);

    addChildWidget(mainWidget, currentEquipWidget, CHAR_DATA_WIDGET_NAME);
}


Widget* allocateEquipRow(const char* name, i32 xCoordinate, i32 yCoordinate){
    auto equipRow = createWidget(CHAR_DATA_WIDGET_NAME);
    moveWidget(equipRow, xCoordinate, yCoordinate);
    DrawTextParams textParams{ xCoordinate, yCoordinate, defaultString, COLOR_TEAL, 0.2f };
    auto textWidget = createTextWidget(textParams, "TYPE");
    addChildWidget(equipRow, (Widget*)textWidget, "TYPE");

    setTextParams(&textParams, xCoordinate + 63, yCoordinate, defaultString, COLOR_WHITE, 0.2f);
    textWidget = createTextWidget(textParams, "EQP");
    addChildWidget(equipRow, (Widget*)textWidget, "EQP");
    return equipRow;
}

void equipRowUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != StaticGridWidgetClass()) {
        return;
    }
    auto& gearSlot = gContext.party.getActivePartyCharacter(*EQUIP_MENU_PARTY_INDEX).equipment.at(flatIndex);
    updateText(getChild(widget, "TYPE"), gearSlot.slotName.str());
    updateText(getChild(widget, "EQP"), gearSlot.equipped.gearName.str());
}

void initGearDescWidget(const MenuInitEvent* event) {
    DrawTextParams textParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto GearDescWidget = createWidget(GEAR_DESC_WIDGET_NAME);

    DrawBoxParams boxParams = {
        equipMenuWindowConfig[1].x,
        equipMenuWindowConfig[1].y,
        equipMenuWindowConfig[1].width,
        equipMenuWindowConfig[1].height,
        0.3f
    };
    auto boxWidget = createBoxWidget(boxParams, GEAR_DESC_BOX);
    addChildWidget(mainWidget, (Widget*)boxWidget, GEAR_DESC_BOX);

    setTextParams(&textParams, 12, equipMenuWindowConfig[1].y + 13, defaultString, COLOR_WHITE, 0.2f);
    auto textWidget = createTextWidget(textParams, GEAR_DESCRIPTION);
    addChildWidget(GearDescWidget, (Widget*)textWidget, GEAR_DESCRIPTION);

    addChildWidget(mainWidget, GearDescWidget, GEAR_DESC_WIDGET_NAME);
}

/* Initialize the Widget for the characters Materia Slots. This will be updated when Handling in Handlers*/
void initGearMateriaSlotWidget(const MenuInitEvent* event) {
    const char * menuText;
    u8 materiaGrowth;
    auto characterID = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];

    TextWidget* textWidget;
    DrawTextParams textParams;
    SlotsWidget* slotsWidget;
    drawSlotsParams slotsParams;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto equipMateraSlotWidget = createWidget(GEAR_SLOTS_WIDGET_NAME);

    boxParams = {
        equipMenuWindowConfig[2].x,
        equipMenuWindowConfig[2].y,
        equipMenuWindowConfig[2].width,
        equipMenuWindowConfig[2].height,
        0.3f
    };
    boxWidget = createBoxWidget(boxParams, GEAR_SLOTS_BOX);
    addChildWidget(equipMateraSlotWidget, (Widget*)boxWidget, GEAR_SLOTS_BOX);

    std::vector<std::string> equipSlotDataNames = { SLOTS_NAME, GROWTH_NAME };
    for (int i = 0; i < 2; i++) {
        menuText = gContext.gameStrings.equipMenuTexts.get_string(14 + i);
        setTextParams(&textParams, 27, 42 * i + equipMenuWindowConfig[2].y + 21, menuText, COLOR_TEAL, 0.1f);
        textWidget = createTextWidget(textParams, equipSlotDataNames[i]);
        addChildWidget(equipMateraSlotWidget, (Widget*)textWidget, equipSlotDataNames[i]);
    }

    //create a default static variable later to a valid bytearray representing materia slot data
    slotsParams = { 153, equipMenuWindowConfig[2].y + 21, nullptr };
    slotsWidget = createSlotsWidget(slotsParams, GEAR_SLOTS);
    addChildWidget(equipMateraSlotWidget, (Widget*)slotsWidget, GEAR_SLOTS);

    materiaGrowth = 1;
    if (materiaGrowth < 0 || materiaGrowth > 3) //display any invalid materia growth as "None"
        materiaGrowth = 4;

    menuText = gContext.gameStrings.equipMenuTexts.get_string(materiaGrowth + 4);
    i32 growthTypeY = equipMenuWindowConfig[2].y + 64;
    i32 growthTypeX = 35;
    setTextParams(&textParams, 243 - growthTypeX / 2, growthTypeY, menuText, COLOR_WHITE, 0.2f);
    textWidget = createTextWidget(textParams, GEAR_GROWTH);
    addChildWidget(equipMateraSlotWidget, (Widget*)textWidget, GEAR_GROWTH);

    addChildWidget(mainWidget, equipMateraSlotWidget, GEAR_SLOTS_WIDGET_NAME);
}

/*Initialize the Widget That displays stats*/
void initStatDiffWidget(const MenuInitEvent* event) {
    u16 windowTop = equipMenuWindowConfig[3].y + 26;
    const char* menuText;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto statDiffWidget = createWidget(STAT_DIFF_WIDGET_NAME);

    boxParams = {
        equipMenuWindowConfig[3].x,
        equipMenuWindowConfig[3].y,
        equipMenuWindowConfig[3].width,
        equipMenuWindowConfig[3].height,
        0.5f
    };
    boxWidget = createBoxWidget(boxParams, STAT_DIFF_BOX);
    addChildWidget(statDiffWidget, (Widget*)boxWidget, STAT_DIFF_BOX);
    DrawStaticGridParams staticGridParams = {nullptr, 53, windowTop, 1, 7, 0, 26, &allocateStatDiffRow};
    auto statDiffGrid = createStaticGridWidget(staticGridParams, STAT_DIFF_GRID);
    addChildWidget(statDiffWidget, (Widget*)statDiffGrid, STAT_DIFF_GRID);

    addChildWidget(mainWidget, statDiffWidget, STAT_DIFF_WIDGET_NAME);
}

Widget* allocateStatDiffRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    DrawTextParams textParams;
    DrawNumberParams numberParams;

    auto statDiffRow = createWidget(name);
    moveWidget(statDiffRow, xCoordinate, yCoordinate);
    setTextParams(&textParams, xCoordinate, yCoordinate, defaultString, COLOR_TEAL, 0.2f);
    auto textWidget = createTextWidget(textParams, "STA");
    addChildWidget(statDiffRow, (Widget*)textWidget, "STA");

    DrawSimpleAssetParams simpleAssetParams = { xCoordinate + 191, yCoordinate, 0xDAu, COLOR_TEAL, 0.2f };
    auto simpleAssetWidget = createSimpleGameAssetWidget(simpleAssetParams, "ARW");
    addChildWidget(statDiffRow, (Widget*)simpleAssetWidget, "ARW");

    setNumberParams(&numberParams, xCoordinate + 147, yCoordinate, 0, 3, COLOR_WHITE, 0.2f);
    auto numberWidget = createNumberWidget(numberParams, "CUR");
    addChildWidget(statDiffRow, (Widget*)numberWidget, "CUR");

    setNumberParams(&numberParams, xCoordinate + 217, yCoordinate, 0, 3, COLOR_WHITE, 0.2f);
    numberWidget = createNumberWidget(numberParams, "NEW");
    addChildWidget(statDiffRow, (Widget*)numberWidget, "NEW");
    srLogWrite("ALLOCATING STATIC GRID ROW FOR EQUIPMENT STAT DIFF");
    return statDiffRow;
}

//Initialize the gear list with just a box and a series of disabled widgets.
void initGearListWidget(const MenuInitEvent* event) {
    auto gearChoiceCursor = getStateCursor(event->menu, 1);

    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto gearListWidget = createWidget(GEAR_LIST_WIDGET_NAME);

    boxParams = {
        equipMenuWindowConfig[4].x,
        equipMenuWindowConfig[4].y,
        equipMenuWindowConfig[4].width,
        equipMenuWindowConfig[4].height,
        0.5f
    };
    boxWidget = createBoxWidget(boxParams, GEAR_LIST_BOX);
    addChildWidget(gearListWidget, (Widget*)boxWidget, GEAR_LIST_BOX);

    DrawCursorGridParams gridParams = { EQUIP_MENU_NAME.c_str(), 1, &gearViewNameUpdater, 427, 193, nullptr, 0 };
    auto cursorListWidget = createGridWidget(gridParams, EQUIP_LIST, TextWidgetKlass());
    addChildWidget(gearListWidget, (Widget*)cursorListWidget, EQUIP_LIST);

    addChildWidget(mainWidget, gearListWidget, GEAR_LIST_WIDGET_NAME);
}


void gearViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto gearType = gContext.gearViewData.getItemType();
    auto relativeItemID = gContext.gearViewData.getResource(flatIndex).relativeItemID;
    const char* name = getGearNameFromRelativeID(relativeItemID, gearType);
    updateText(widget, name);
    updateTextColor(widget, COLOR_WHITE);
}
