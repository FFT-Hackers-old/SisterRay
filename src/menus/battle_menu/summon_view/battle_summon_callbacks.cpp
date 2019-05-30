
#include "battle_summon_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleSummonWidgetNames;

void initializeBattleSummonMenu() {
    CursorContext summonSelection = { 0, 0, 1, 3, 0, 0, 1, SUMMON_COUNT, 0, 0, 0, 0, 0, 1 };
    Cursor summonChoiceCursor = { summonSelection, 36, 364, 32, 156 };

    Cursor cursorArray[3] = { summonChoiceCursor, summonChoiceCursor, summonChoiceCursor }; /*One cursor per party member*/
    auto battleSpellMenu = createMenu(INIT_BATTLE_SUMMON, 3, &cursorArray[0]);
    gContext.menuWidgets.add_element("BATTLE_SUMMON", battleSpellMenu);
    gContext.menuWidgets.initializeMenu("BATTLE_SUMMON", BATTLE_SUMMON_MENU_NAME);
}

void registerSummonMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_SPELL_VIEW});
    gContext.eventBus.addListener(INIT_BATTLE_SUMMON, (SrEventCallback)&initBattleSummonViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&initBattleSummonViewWidget, modName, contextKeys);
}
