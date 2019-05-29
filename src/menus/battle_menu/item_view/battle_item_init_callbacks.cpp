#include "battle_item_init_callbacks.h"
#include "../../../impl.h"

using namespace BattleItemWidgetNames;

void initBattleItemViewWidget(const BattleSpellDrawEvent* event) {
    drawGridParams gridParams;
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;
    auto battleItemView = createWidget(BATTLE_ITEM_MENU_NAME);

    /*boxParams = {
        380,
        190,
        260,
        300,
        0.3f
    };
    boxWidget = createBoxWidget(boxParams, MATERIA_GRID_BOX);
    addChildWidget(materiaViewWidget, (Widget*)boxWidget, MATERIA_GRID_BOX);*/

    /*One Grid for each active party member, use a custom allocator*/
    std::vector<std::string> names = { PARTY_1_SPELL_GRID, PARTY_2_SPELL_GRID, PARTY_3_SPELL_GRID };
    for (auto idx = 0; idx < names.size(); idx++) {
        auto battleItemChoice = getStateCursor(event->menu, idx);
        gridParams = { battleItemChoice, &battleInventoryRowUpdater, 98, 360, allocateBattleInventoryRow };
        gridWidget = createGridWidget(gridParams, names[idx]);
        addChildWidget(battleItemView, (Widget*)gridWidget, names[idx]);
    }
    addChildWidget(mainWidget, battleItemView, BATTLE_ITEM_MENU_NAME);
}

/*Allocator to be used with battle row widget*/
Widget* allocateBattleInventoryRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto battleInventoryRow = createWidget(name);
    moveWidget(battleInventoryRow, xCoordinate, yCoordinate);
    DrawGameAssetParams gameAssetParams = ItemIcon(xCoordinate, yCoordinate, 0, 0.1f);
    addChildWidget(battleInventoryRow, (Widget*)createGameAssetWidget(gameAssetParams, std::string("ICN")), std::string("ICN"));
    DrawTextParams textParams = { xCoordinate + 32, yCoordinate, getDefaultString(), COLOR_WHITE, 0.1f };
    addChildWidget(battleInventoryRow, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
    DrawSimpleAssetParams simpleAssetParams = Cross(xCoordinate + 266, yCoordinate, COLOR_WHITE, 0.1f);
    addChildWidget(battleInventoryRow, (Widget*)createSimpleGameAssetWidget(simpleAssetParams, std::string("CRS")), std::string("CRS"));
    DrawNumberParams numberParams = { xCoordinate + 280, yCoordinate, 0, 3, COLOR_WHITE, 0.1f };
    addChildWidget(battleInventoryRow, (Widget*)createNumberWidget(numberParams, std::string("AMT")), std::string("AMT"));
    return battleInventoryRow;
}
