#include "equip_menu_widget.h"
#include "../../impl.h"
#include "../../party/party_utils.h"

/* This Module contains code which initializes the Widget that represents the equip menu internally
   You can mutate this widget prior to draw by registered your listeners to the "OnEquipMenuDraw" event
   The Widget is also mutated by listeners registered to the Equip-Menu's input Handlers.
*/

using namespace EquipWidgetNames;

/* Returns a pointer to a Widget representing the Equipment menu*/
Widget* initEquipMenuWidget() {
    auto mainWidget = createWidget(EQUIP_MENU_NAME);
    // create the equipment Widget and add it's default children
    initCharDataWidget(mainWidget);
    initStatDiffWidget(mainWidget);
    initGearDescWidget(mainWidget);
    return mainWidget;
}

void initCharDataWidget(Widget* mainWidget) {
    char * menuText;
    char* fetchedName;
    u16 kernelObjectID;
    auto characterID = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];

    TextWidget* textWidget;
    drawTextParams textParams;
    BoxWidget* boxWidget;
    drawBoxParams boxParams;

    auto currentEquipWidget = createWidget(CHAR_DATA_WIDGET_NAME);

    boxParams = {
       equipMenuWindowConfig[0].drawDistance1,
       equipMenuWindowConfig[0].drawDistance2,
       equipMenuWindowConfig[0].drawDistance3,
       equipMenuWindowConfig[0].drawDistance4,
       0.1f
    };
    boxWidget = createBoxWidget(boxParams, CHAR_DATA_BOX_NAME);
    addChildWidget(currentEquipWidget, (Widget*)boxWidget, CHAR_DATA_BOX_NAME);

    std::vector<std::string> gearNames = { GEAR_SLOT_1_NAME, GEAR_SLOT_2_NAME, GEAR_SLOT_3_NAME };
    std::vector<std::string> equippedGearNames = { EQUIPPED_WEAPON, EQUIPPED_ARMOR, EQUIPPED_ACC };
    for (int row = 0; row < 3; row++) {
        menuText = gContext.game_strings.equipMenuTexts.get_string(row);
        textParams = { 250, 13 + (34 * row), menuText, COLOR_TEAL, 0.2f };
        textWidget = createTextWidget(textParams, gearNames[row]);
        addChildWidget(currentEquipWidget, (Widget*)textWidget, gearNames[row]);

        kernelObjectID = getEquippedGear(characterID, row + 1);
        fetchedName = getNameFromRelativeID(kernelObjectID, row + 1);
        textParams = { 303, 13, fetchedName, COLOR_WHITE, 0.2f };
        textWidget = createTextWidget(textParams, equippedGearNames[row]);
        addChildWidget(currentEquipWidget, (Widget*)textWidget, equippedGearNames[row]);
    }

    addChildWidget(mainWidget, currentEquipWidget, CHAR_DATA_WIDGET_NAME);
}

void initGearDescWidget(Widget* mainWidget) {
    char* fetchedName;
    u16 kernelObjectID;
    auto characterID = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];

    TextWidget* textWidget;
    drawTextParams textParams;
    BoxWidget* boxWidget;
    drawBoxParams boxParams;

    auto GearDescWidget = createWidget(GEAR_DESC_WIDGET_NAME);

    boxParams = {
        equipMenuWindowConfig[1].drawDistance1,
        equipMenuWindowConfig[1].drawDistance2,
        equipMenuWindowConfig[1].drawDistance3,
        equipMenuWindowConfig[1].drawDistance4,
        0.1f
    };
    boxWidget = createBoxWidget(boxParams, GEAR_DESC_BOX);
    addChildWidget(mainWidget, (Widget*)boxWidget, GEAR_DESC_BOX);

    kernelObjectID = getEquippedGear(characterID, 1);
    fetchedName = getNameFromRelativeID(kernelObjectID, 1);
    textParams = { 24, equipMenuWindowConfig[1].drawDistance2 + 13, fetchedName, COLOR_WHITE, 0.2f };
    textWidget = createTextWidget(textParams, GEAR_DESCRIPTION);
    addChildWidget(GearDescWidget, (Widget*)textWidget, GEAR_DESCRIPTION);

    addChildWidget(mainWidget, (Widget*)textWidget, GEAR_DESC_WIDGET_NAME);
}

/* Initialize the Widget for the characters Materia Slots. This will be updated when Handling in Handlers*/
void initGearMateriaSlotWidget(Widget* mainWidget) {
    char * menuText;
    u16 kernelObjectID;
    u8 materiaGrowth;
    u8* materiaSlots;
    auto characterID = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];

    TextWidget* textWidget;
    drawTextParams textParams;
    SlotsWidget* slotsWidget;
    drawSlotsParams slotsParams;
    BoxWidget* boxWidget;
    drawBoxParams boxParams;

    auto equipMateraSlotWidget = createWidget(GEAR_SLOTS_WIDGET_NAME);

    boxParams = {
        equipMenuWindowConfig[2].drawDistance1,
        equipMenuWindowConfig[2].drawDistance2,
        equipMenuWindowConfig[2].drawDistance3,
        equipMenuWindowConfig[2].drawDistance4,
        0.1f
    };
    boxWidget = createBoxWidget(boxParams, GEAR_SLOTS_BOX);
    addChildWidget(equipMateraSlotWidget, (Widget*)boxWidget, GEAR_SLOTS_BOX);

    std::vector<std::string> equipSlotDataNames = { SLOTS_NAME, GROWTH_NAME };
    for (int i = 0; i < 2; i++) {
        menuText = gContext.game_strings.equipMenuTexts.get_string(4 + i);
        textParams.set(27, 42 * i + equipMenuWindowConfig[2].drawDistance2 + 21, menuText, COLOR_TEAL, 0.1f);
        textWidget = createTextWidget(textParams, equipSlotDataNames[i]);
        addChildWidget(equipMateraSlotWidget, (Widget*)textWidget, equipSlotDataNames[i]);
    }

    kernelObjectID = getEquippedGear(characterID, 1);

    //Display current weapon slots & growth by default -- overridden in handlers to behave correctly
    materiaSlots = &(gContext.weapons.get_resource(kernelObjectID).materia_slots[0]);
    slotsParams = { 153, equipMenuWindowConfig[2].drawDistance2 + 21, materiaSlots };
    slotsWidget = createSlotsWidget(slotsParams, GEAR_SLOTS);
    addChildWidget(equipMateraSlotWidget, (Widget*)slotsWidget, GEAR_SLOTS);

    materiaGrowth = gContext.weapons.get_resource(kernelObjectID).materia_growth;
    if (materiaGrowth < 0 || materiaGrowth > 3) //display any invalid materia growth as "None"
        materiaGrowth = 4;

    menuText = gContext.game_strings.equipMenuTexts.get_string(materiaGrowth + 4);
    i32 growthTypeY = equipMenuWindowConfig[2].drawDistance2 + 64;
    i32 growthTypeX = sub_6F54A2((u8*)menuText);
    textParams.set(243 - growthTypeX / 2, growthTypeY, menuText, COLOR_WHITE, 0.2f);
    textWidget = createTextWidget(textParams, GEAR_GROWTH);
    addChildWidget(equipMateraSlotWidget, (Widget*)textWidget, GEAR_GROWTH);


    addChildWidget(mainWidget, equipMateraSlotWidget, GEAR_SLOTS_BOX);
}

/*Initialize the Widget That displays stats*/
void initStatDiffWidget(Widget* mainWidget) {
    u8 characterID = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];
    u16 equippedWeaponID = (CHARACTER_RECORD_ARRAY)[characterID].equipped_weapon;
    u16 equippedArmorID = (CHARACTER_RECORD_ARRAY)[characterID].equipped_armor;
    u16 equippedAccessoryID = (CHARACTER_RECORD_ARRAY)[characterID].equipped_accessory;
    u8 equippedStats[8];
    u16 windowTop = equipMenuWindowConfig[3].drawDistance2 + 26;
    char* menuText;

    TextWidget* textWidget;
    drawTextParams textParams;
    NumberWidget* numberWidget;
    drawNumberParams numberParams;
    ArrowWidget* arrowWidget;
    drawArrowParams arrowParams;
    BoxWidget* boxWidget;
    drawBoxParams boxParams;

    auto statDiffWidget = createWidget(STAT_DIFF_WIDGET_NAME);

    boxParams.set(
        equipMenuWindowConfig[3].drawDistance1,
        equipMenuWindowConfig[3].drawDistance2,
        equipMenuWindowConfig[3].drawDistance3,
        equipMenuWindowConfig[3].drawDistance4,
        0.5f
    );
    boxWidget = createBoxWidget(boxParams, STAT_DIFF_BOX);
    addChildWidget(statDiffWidget, (Widget*)boxWidget, STAT_DIFF_BOX);


    equippedStats[0] = gContext.weapons.get_resource(equippedWeaponID).weapon_strength;
    equippedStats[1] = gContext.weapons.get_resource(equippedWeaponID).weapon_hit_rate;
    equippedStats[2] = gContext.armors.get_resource(equippedArmorID).defense;
    equippedStats[3] = gContext.armors.get_resource(equippedArmorID).evade;
    equippedStats[4] = 0;
    equippedStats[5] = gContext.armors.get_resource(equippedArmorID).magic_defense;
    equippedStats[6] = gContext.armors.get_resource(equippedArmorID).magic_evade;



    std::vector<std::string> statNames = { STAT_NAME_1, STAT_NAME_2, STAT_NAME_3, STAT_NAME_4, STAT_NAME_5, STAT_NAME_6, STAT_NAME_7 };
    std::vector<std::string> numberNames = { STAT_VALUE_1, STAT_VALUE_2, STAT_VALUE_3, STAT_VALUE_4, STAT_VALUE_5, STAT_VALUE_5, STAT_VALUE_6, STAT_VALUE_7 };
    std::vector<std::string> candidateNumberNames = { NEW_STAT_VALUE_1, NEW_STAT_VALUE_2, NEW_STAT_VALUE_3, NEW_STAT_VALUE_4, NEW_STAT_VALUE_5, NEW_STAT_VALUE_5, NEW_STAT_VALUE_6, NEW_STAT_VALUE_7 };
    std::vector<std::string> arrowNames = { ARROW_1, ARROW_2, ARROW_3, ARROW_4, ARROW_5, ARROW_6, ARROW_7 };
    for (i32 i = 0; i < 7; ++i) {
        menuText = gContext.game_strings.equipMenuTexts.get_string(2);
        textParams.set(53, windowTop + 26 * i - 6, menuText, COLOR_TEAL, 0.2f);
        textWidget = createTextWidget(textParams, statNames[i]);
        addChildWidget(statDiffWidget, (Widget*)textWidget, statNames[i]);

        arrowParams = { 50 + 194, 26 * i + windowTop, 0xDAu, COLOR_TEAL, 0.2f };
        arrowWidget = createArrowWidget(arrowParams, arrowNames[i]);
        addChildWidget(statDiffWidget, (Widget*)arrowWidget, arrowNames[i]);

        numberParams.set(53, windowTop + 26 * i, equippedStats[i], 3, COLOR_WHITE, 0.2f);
        numberWidget = createNumberWidget(numberParams, numberNames[i]);
        addChildWidget(statDiffWidget, (Widget*)numberWidget, numberNames[i]);

        numberParams.set(270, windowTop + 26 * i, equippedStats[i], 3, COLOR_WHITE, 0.2f);
        numberWidget = createNumberWidget(numberParams, numberNames[i]);
        addChildWidget(statDiffWidget, (Widget*)numberWidget, numberNames[i]);
    }
    addChildWidget(mainWidget, statDiffWidget, STAT_DIFF_WIDGET_NAME);
}


//Initialize the gear list with just a box and a series of disabled widgets.
void initGearListWidget(Widget* mainWidget) {
    char * menuText;
    char* fetchedName;
    u16 kernelObjectID;
    auto characterID = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];
    cursorContext* cursorContextArray = (cursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;

    GridWidgetParams gridParams;
    BoxWidget* boxWidget;
    drawBoxParams boxParams;

    auto gearListWidget = createWidget(GEAR_LIST_WIDGET_NAME);

    boxParams.set(
        equipMenuWindowConfig[4].drawDistance1,
        equipMenuWindowConfig[4].drawDistance2,
        equipMenuWindowConfig[4].drawDistance3,
        equipMenuWindowConfig[4].drawDistance4,
        0.5f
    );
    boxWidget = createBoxWidget(boxParams, GEAR_LIST_BOX);
    addChildWidget(gearListWidget, (Widget*)boxWidget, GEAR_LIST_BOX);

    gridParams = {(cursorContext*)&(cursorContextArray[1]), &gearViewNameUpdater, 427, 193, 36, 0};
    auto cursorListWidget = createGridWidget(gridParams, EQUIP_LIST, &kTextWidgetClass);
    addChildWidget(gearListWidget, (Widget*)cursorListWidget, EQUIP_LIST);



    addChildWidget(mainWidget, gearListWidget, GEAR_LIST_WIDGET_NAME);
}

