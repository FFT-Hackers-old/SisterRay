#include "battle_spell_init_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initBattleMagicViewWidget(const MenuInitEvent* event) {
    drawGridParams gridParams;
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto battleSpellView = createWidget(BATTLE_SPELL_WIDGET_NAME);
    srLogWrite("CALLING BATTLE SPELL INIT HANDLER");

    boxParams = {
        0,
        340,
        640,
        120,
        0.4f
    };
    boxWidget = createBoxWidget(boxParams, SPELL_GRID_BOX);
    addChildWidget(battleSpellView, (Widget*)boxWidget, SPELL_GRID_BOX);

    std::vector<std::string> names = { PARTY_1_SPELL_GRID, PARTY_2_SPELL_GRID, PARTY_3_SPELL_GRID };
    for (auto idx = 0; idx < names.size(); idx++) {
        auto magicItemChoice = getStateCursor(event->menu, idx);
        drawGridParams gridParams = { BATTLE_MENU_NAME.c_str(), BATTLE_MAGIC_STATE, &battleSpellNameViewUpdater, 42, 360, allocateSpellRow, idx };
        gridWidget = createGridWidget(gridParams, names[idx]);
        addChildWidget(battleSpellView, (Widget*)gridWidget, names[idx]);
    }

    addChildWidget(mainWidget, battleSpellView, BATTLE_SPELL_WIDGET_NAME);
}

Widget* allocateSpellRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto spellWidget = createWidget(name);
    moveWidget(spellWidget, xCoordinate, yCoordinate);
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.4f };
    addChildWidget(spellWidget, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
    DrawGameAssetParams gameAssetParams = AllArrow(xCoordinate + 4, yCoordinate, 0.4f);
    addChildWidget(spellWidget, (Widget*)createGameAssetWidget(gameAssetParams, std::string("ARW")), std::string("ARW"));
    return spellWidget;
}
