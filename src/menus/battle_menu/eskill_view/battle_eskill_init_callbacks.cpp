#include "battle_eskill_init_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initBattleESkillViewWidget(const MenuInitEvent* event) {
    drawGridParams gridParams;
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto battleESkillView = createWidget(BATTLE_ESKILL_WIDGET_NAME);

    /*boxParams = {
        380,
        190,
        260,
        300,
        0.3f
    };
    boxWidget = createBoxWidget(boxParams, MATERIA_GRID_BOX);
    addChildWidget(materiaViewWidget, (Widget*)boxWidget, MATERIA_GRID_BOX);*/

    std::vector<std::string> names = { PARTY_1_ESKILL_GRID, PARTY_2_ESKILL_GRID, PARTY_3_ESKILL_GRID };
    for (auto idx = 0; idx < names.size(); idx++) {
        auto eskillChoice = getStateCursor(event->menu, idx);

        drawGridParams gridParams = { BATTLE_MENU_NAME.c_str(), BATTLE_ESKILL_STATE, &battleEskillNameViewUpdater, 42, 360, allocateEskillRow, idx };
        gridWidget = createGridWidget(gridParams, names[idx]);
        addChildWidget(battleESkillView, (Widget*)gridWidget, names[idx]);
    }
    addChildWidget(mainWidget, battleESkillView, BATTLE_ESKILL_WIDGET_NAME);
}

Widget* allocateEskillRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto eSkillRow = createWidget(name);
    moveWidget(eSkillRow, xCoordinate, yCoordinate);
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.1f };
    addChildWidget(eSkillRow, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
    return eSkillRow;
}
