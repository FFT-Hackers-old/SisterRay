#include "battle_item_init_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initBattleItemViewWidget(const MenuInitEvent* event) {
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;
    auto battleItemView = createWidget(BATTLE_ITEM_WIDGET_NAME);

    boxParams = {
    0,
    340,
    640,
    120,
    0.4f
    };
    boxWidget = createBoxWidget(boxParams, ITEM_GRID_BOX);
    addChildWidget(battleItemView, (Widget*)boxWidget, ITEM_GRID_BOX);

    /*One Grid for each active party member, use a custom allocator*/
    std::vector<std::string> names = { PARTY_1_ITEM_GRID, PARTY_2_ITEM_GRID, PARTY_3_ITEM_GRID };
    for (auto idx = 0; idx < names.size(); idx++) {
        auto battleItemChoice = getStateCursor(event->menu, idx);
        drawGridParams gridParams = { BATTLE_MENU_NAME.c_str(), BATTLE_ITEM_STATE, &battleInventoryRowUpdater, 98, 360, allocateBattleInventoryRow, idx };
        gridWidget = createGridWidget(gridParams, names[idx]);
        addChildWidget(battleItemView, (Widget*)gridWidget, names[idx]);
    }
    addChildWidget(mainWidget, battleItemView, BATTLE_ITEM_WIDGET_NAME);
}

/*Allocator to be used with battle row widget*/
Widget* allocateBattleInventoryRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto battleInventoryRow = createWidget(name);
    moveWidget(battleInventoryRow, xCoordinate, yCoordinate);
    DrawGameAssetParams gameAssetParams = ItemIcon(xCoordinate, yCoordinate, 0, 0.1f);
    addChildWidget(battleInventoryRow, (Widget*)createGameAssetWidget(gameAssetParams, std::string("ICN")), std::string("ICN"));
    DrawTextParams textParams = { xCoordinate + 32, yCoordinate, getDefaultString(), COLOR_WHITE, 0.4f };
    addChildWidget(battleInventoryRow, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
    DrawSimpleAssetParams simpleAssetParams = Cross(xCoordinate + 266, yCoordinate, COLOR_WHITE, 0.4f);
    addChildWidget(battleInventoryRow, (Widget*)createSimpleGameAssetWidget(simpleAssetParams, std::string("CRS")), std::string("CRS"));
    DrawNumberParams numberParams = { xCoordinate + 280, yCoordinate, 0, 3, COLOR_WHITE, 0.4f };
    addChildWidget(battleInventoryRow, (Widget*)createNumberWidget(numberParams, std::string("AMT")), std::string("AMT"));
    return battleInventoryRow;
}
