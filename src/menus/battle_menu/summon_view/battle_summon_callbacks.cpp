
#include "battle_summon_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initializeBattleSummonMenu() {
    CursorContext summonSelection = { 0, 0, 1, 3, 0, 0, 1, SUMMON_COUNT, 0, 0, 0, 0, 0, 1 };
    Cursor summonChoiceCursor = { summonSelection, 8, 364, 32, 156 };
    auto battleMenu = gContext.menuWidgets.get_element(BATTLE_MENU_NAME);
    setStateCursor(battleMenu, BATTLE_SUMMON_STATE, summonChoiceCursor, 0);
    setStateCursor(battleMenu, BATTLE_SUMMON_STATE, summonChoiceCursor, 1);
    setStateCursor(battleMenu, BATTLE_SUMMON_STATE, summonChoiceCursor, 2);
}

void registerSummonViewListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_MENU});
    gContext.eventBus.addListener(DRAW_BATTLE_MENU, (SrEventCallback)&drawBattleSummonViewWidget);
    gContext.eventBus.addListener(INIT_BATTLE_MENU, (SrEventCallback)&initBattleSummonViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleSelectSummon, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&handleExitSummon, modName, contextKeys);
}
