#include "battle_summon_init_callbacks.h"
#include "../../../impl.h"

using namespace BattleSummonWidgetNames;

void initBattleSummonViewWidget(const BattleSpellDrawEvent* event) {
    drawGridParams gridParams;
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto battleSummonView = createWidget(BATTLE_SUMMON_MENU_NAME);

    /*boxParams = {
        380,
        190,
        260,
        300,
        0.3f
    };
    boxWidget = createBoxWidget(boxParams, MATERIA_GRID_BOX);
    addChildWidget(materiaViewWidget, (Widget*)boxWidget, MATERIA_GRID_BOX);*/


    std::vector<std::string> names = { PARTY_1_SPELL_GRID, PARTY_2_SPELL_GRID, PARTY_3_SPELL_GRID };
    for (auto idx = 0; idx < names.size(); idx++) {
        auto summonChoice = getStateCursor(event->menu, idx);
        gridParams = { summonChoice, &battleSummonNameViewUpdater, 42, 360, allocateSummonRow };
        gridWidget = createGridWidget(gridParams, names[idx]);
        addChildWidget(battleSummonView, (Widget*)gridWidget, names[idx]);
    }
    addChildWidget(mainWidget, battleSummonView, BATTLE_SUMMON_MENU_NAME);
}

Widget* allocateSummonRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto spellWidget = createWidget(name);
    moveWidget(spellWidget, xCoordinate, yCoordinate);
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.1f };
    addChildWidget(spellWidget, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
    return spellWidget;
}
