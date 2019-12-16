#include "battle_summon_init_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initBattleSummonViewWidget(const MenuInitEvent* event) {
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;
    auto battleSummonView = createWidget(BATTLE_SUMMON_WIDGET_NAME);

    boxParams = {
        0,
        340,
        640,
        140,
        0.4f
    };
    boxWidget = createBoxWidget(boxParams, SUMMON_GRID_BOX);
    addChildWidget(battleSummonView, (Widget*)boxWidget, SUMMON_GRID_BOX);

    std::vector<std::string> names = { PARTY_1_SUMMON_GRID, PARTY_2_SUMMON_GRID, PARTY_3_SUMMON_GRID };
    for (u32 idx = 0; idx < names.size(); idx++) {
        auto summonChoice = getStateCursor(event->menu, BATTLE_SUMMON_STATE, idx);
        drawGridParams gridParams = { BATTLE_MENU_NAME.c_str(), BATTLE_SUMMON_STATE, &battleSummonNameViewUpdater, 42, 360, allocateSummonRow, idx };
        gridWidget = createGridWidget(gridParams, names[idx]);
        addChildWidget(battleSummonView, (Widget*)gridWidget, names[idx]);
    }

    addChildWidget(mainWidget, battleSummonView, BATTLE_SUMMON_WIDGET_NAME);
}

Widget* allocateSummonRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto spellWidget = createWidget(name);
    moveWidget(spellWidget, xCoordinate, yCoordinate);
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.4f };
    addChildWidget(spellWidget, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
    return spellWidget;
}
