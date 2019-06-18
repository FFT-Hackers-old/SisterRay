#include "battle_change_init_callbacks.h"
#include "../../../impl.h"

using namespace BattleChangeWidgetNames;

void initBattleChangeViewWidget(const BattleSpellDrawEvent* event) {
    drawGridParams gridParams;
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto battleChangeView = createWidget(BATTLE_CHANGE_MENU_NAME);

    /*boxParams = {
        380,
        190,
        260,
        300,
        0.3f
    };
    boxWidget = createBoxWidget(boxParams, MATERIA_GRID_BOX);
    addChildWidget(materiaViewWidget, (Widget*)boxWidget, MATERIA_GRID_BOX);*/

    /*Extra command frames. Will include LIMIT, Change, and switch*/
    /*std::vector<std::string> names = { PARTY_1_CHANGE_GRID, PARTY_2_CHANGE_GRID, PARTY_3_CHANGE_GRID };
    for (auto idx = 0; idx < names.size(); idx++) {
        auto changeChoice = getStateCursor(event->menu, idx);
        gridParams = { changeChoice, &nullptr, 68, 360, allocateChangeRow };
        gridWidget = createGridWidget(gridParams, names[idx]);
        addChildWidget(battleChangeView, (Widget*)gridWidget, names[idx]);
    }
    addChildWidget(mainWidget, battleChangeView, BATTLE_CHANGE_MENU_NAME);*/
}

Widget* allocateChangeRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto changeRow = createWidget(name);
    moveWidget(changeRow, xCoordinate, yCoordinate);
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.1f };
    addChildWidget(changeRow, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
    return changeRow;
}
