#include "battle_base_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initializeBattleTargetingMenu() {
    CursorContext summonSelection = { 0, 0, 1, 3, 0, 0, 1, SUMMON_COUNT, 0, 0, 0, 0, 0, 1 };
    Cursor summonChoiceCursor = { summonSelection, 8, 364, 32, 156 };
    auto battleSpellMenu = gContext.menuWidgets.get_element(BATTLE_MENU_NAME);
    setStateCursor(battleSpellMenu, SUMMON_BATTLE_STATE, summonChoiceCursor, 0);
    setStateCursor(battleSpellMenu, SUMMON_BATTLE_STATE, summonChoiceCursor, 1);
    setStateCursor(battleSpellMenu, SUMMON_BATTLE_STATE, summonChoiceCursor, 2);
}

void registerTargetingMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_MENU});
    gContext.eventBus.addListener(MENU_INPUT_START, (SrEventCallback)&handlePauseBattle, modName, contextKeys);
}
