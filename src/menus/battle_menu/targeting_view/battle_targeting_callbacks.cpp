#include "battle_targeting_callbacks.h"
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
    gContext.eventBus.addListener(DRAW_BATTLE_MENU, (SrEventCallback)&drawTargetingCursors, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleSelectTargets, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&handleExitSelectTargets, modName, contextKeys);
}
