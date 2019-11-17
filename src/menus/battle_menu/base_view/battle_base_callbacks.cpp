#include "battle_base_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initializeBattleBaseMenu(std::string battleMenuName) {
    srLogWrite(std::string("Initializing base battle view").c_str());
}

void registerBaseViewListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_MENU});
    gContext.eventBus.addListener(INIT_BATTLE_MENU, (SrEventCallback)&initBaseViewWidget, modName);
    gContext.eventBus.addListener(DRAW_BATTLE_MENU, (SrEventCallback)&drawBaseViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_START, (SrEventCallback)&handlePauseBattle, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_TRIANGLE, (SrEventCallback)&swapCharacterBattle, modName, contextKeys);
}
