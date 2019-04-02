#include "equip_menu_widget.h"
#include "../impl.h"
#include "../party/party_utils.h"

/* This Module contains code which initializes the Widget that represents the equip menu internally
   You can mutate this widget prior to draw by registered your listeners to the "OnEquipMenuDraw" event
   The Widget is also mutated by listeners registered to the Equip-Menu's input Handlers.
*/

/* Returns a pointer to a Widget representing the Equipment menu*/
Widget* initEquipMenuWidget() {
    auto mainWidget = createWidget("equipMenuMain");
    // create the equipment Widget and add it's default children
    initCurrentEqupWidget(mainWidget);
    initStatDiffWidget(mainWidget);
    return mainWidget;
}

void initCurrentEqupWidget(Widget* mainWidget) {
    char * menuText;
    char* fetchedName;
    u16 kernelObjectID;
    auto characterID = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];

    TextWidget* textWidget;
    drawTextParams textParams;
    BoxWidget* boxWidget;
    drawBoxParams boxParams;

    auto currentEquipWidget = createWidget("equipNamesWidgets");

    // Now add the base Box
    // Need to figure out actual params for this box in the game
    boxParams = { 0, 0, 0, 0, 0.3f };
    auto boxWidget = createBoxWidget(boxParams, "BoundingBox");
    addChildWidget(currentEquipWidget, (Widget*)boxWidget, "BoundingBox");

    std::vector<char*> gearNames = { "Wpn", "Arm", "Acc" };
    std::vector<char*> equippedGearNames = { "eWpn", "eArm", "eAcc" };
    for (int i = 0; i < 3; i++) {
        menuText = gContext.game_strings.equip_menu_texts.get_string(i);
        textParams = { 250, 13 + (34 * i), menuText, COLOR_TEAL, 0.2f };
        textWidget = createTextWidget(textParams, gearNames[i]);
        addChildWidget(currentEquipWidget, (Widget*)textWidget, gearNames[i]);

        kernelObjectID = getEquippedGear(characterID, i + 1);
        fetchedName = getNameFromRelativeID(kernelObjectID, i + 1);
        textParams = { 303, 13, fetchedName, COLOR_WHITE, 0.2f };
        textWidget = createTextWidget(textParams, equippedGearNames[i]);
        addChildWidget(currentEquipWidget, (Widget*)textWidget, equippedGearNames[i]);
    }

    addChildWidget(mainWidget, currentEquipWidget, "equipmentNames");
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
    NumberWidget* numberWidget;
    drawNumberParams numberParams;
    SlotsWidget* slotsWidget;
    drawSlotsParams slotsParams;
    BoxWidget* boxWidget;
    drawBoxParams boxParams;

    auto equipMateraSlotWidget = createWidget("equipNamesWidgets");

    boxParams = {
      equipMenuWindowConfig[2].drawDistance1,
      equipMenuWindowConfig[2].drawDistance2,
      equipMenuWindowConfig[2].drawDistance3,
      equipMenuWindowConfig[2].drawDistance4,
      0.1f
    };
    boxWidget = createBoxWidget(boxParams, "BoundingBox");
    addChildWidget(equipMateraSlotWidget, (Widget*)boxWidget, "BoundingBox");

    std::vector<char*> equipSlotDataNames = { "Slots", "Growth" };
    for (int i = 0; i < 2; i++) {
        menuText = gContext.game_strings.equip_menu_texts.get_string(4 + i);
        textParams.set(27, 42 * i + equipMenuWindowConfig[2].drawDistance2 + 21, menuText, COLOR_TEAL, 0.1f);
        textWidget = createTextWidget(textParams, equipSlotDataNames[i]);
        addChildWidget(equipMateraSlotWidget, (Widget*)textWidget, equipSlotDataNames[i]);
    }

    kernelObjectID = getEquippedGear(characterID, 1);

    //Display current weapon slots & growth by default -- overridden in handlers to behave correctly
    materiaSlots = &(gContext.weapons.get_resource(kernelObjectID).materia_slots[0]);
    slotsParams = { 153, equipMenuWindowConfig[2].drawDistance2 + 21, materiaSlots };
    slotsWidget = createSlotsWidget(slotsParams, "gearMateriaSlots");
    addChildWidget(equipMateraSlotWidget, (Widget*)slotsWidget, "gearMateriaSlots");

    materiaGrowth = gContext.weapons.get_resource(kernelObjectID).materia_growth;
    if (materiaGrowth < 0 || materiaGrowth > 3) //display any invalid materia growth as "None"
        materiaGrowth = 4;

    menuText = gContext.game_strings.equip_menu_texts.get_string(materiaGrowth + 4);
    i32 growthTypeY = equipMenuWindowConfig[2].drawDistance2 + 64;
    i32 growthTypeX = sub_6F54A2((u8*)menuText);
    textParams.set(243 - growthTypeX / 2, growthTypeY, menuText, COLOR_WHITE, 0.2f);
    textWidget = createTextWidget(textParams, "gearGrowthValue");
    addChildWidget(equipMateraSlotWidget, (Widget*)textWidget, "gearGrwothValue");


    addChildWidget(mainWidget, equipMateraSlotWidget, "equipmentNames");
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

    auto statDiffWidget = createWidget("statDiffWidget");

    boxParams.set(
        equipMenuWindowConfig[3].drawDistance1,
        equipMenuWindowConfig[3].drawDistance2,
        equipMenuWindowConfig[3].drawDistance3,
        equipMenuWindowConfig[3].drawDistance4,
        0.5f
    );
    boxWidget = createBoxWidget(boxParams, "BoundingBox");
    addChildWidget(statDiffWidget, (Widget*)boxWidget, "BoundingBox");


    equippedStats[0] = gContext.weapons.get_resource(equippedWeaponID).weapon_strength;
    equippedStats[1] = gContext.weapons.get_resource(equippedWeaponID).weapon_hit_rate;
    equippedStats[2] = gContext.armors.get_resource(equippedArmorID).defense;
    equippedStats[3] = gContext.armors.get_resource(equippedArmorID).evade;
    equippedStats[4] = 0;
    equippedStats[5] = gContext.armors.get_resource(equippedArmorID).magic_defense;
    equippedStats[6] = gContext.armors.get_resource(equippedArmorID).magic_evade;

    std::vector<char*> statNames = { "WpnStrength", "WpnHit", "Defense", "Evade", "MagAtk", "mDefense", "mEvade" };
    std::vector<char*> numberNames = { "WpnStrength#A", "WpnHit#A", "Defense#A", "Evade#A", "MagAtk#A", "mDefense#A", "mEvade#A" };
    std::vector<char*> candidateNumberNames = { "WpnStrength#B", "WpnHit#B", "Defense#B", "Evade#B", "MagAtk#B", "mDefense#B", "mEvade#B" };
    std::vector<char*> arrowNames = { "WpnStrength->", "WpnHit->", "Defense->", "Evade->", "MagAtk->", "mDefense->", "mEvade->" };
    for (i32 i = 0; i < 7; ++i) {
        menuText = gContext.game_strings.equip_menu_texts.get_string(2);
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
    addChildWidget(mainWidget, statDiffWidget, "statDiffWidget");
}
