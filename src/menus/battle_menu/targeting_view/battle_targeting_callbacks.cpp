#include "battle_base_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initializeBattleTargetingMenu() {
    srLogWrite(std::string("initializing targeting menu").c_str());
}

void registerTargetingMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_MENU});
    gContext.eventBus.addListener(MENU_INPUT_START, (SrEventCallback)&handlePauseBattle, modName, contextKeys);
}
