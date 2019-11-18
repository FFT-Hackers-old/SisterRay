
#include "battle_eskill_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initializeBattleESkillMenu() {
    CursorContext eSkillContext = { 0, 0, 2, 3, 0, 0, 2, ESKILL_COUNT/2, 0, 0, 0, 0, 0, 1 };
    Cursor eSkillChoiceCursor = { eSkillContext, 8, 364, 32, 156 };

    auto battleMenu = gContext.menuWidgets.get_element(BATTLE_MENU_NAME);
    setStateCursor(battleMenu, BATTLE_ESKILL_STATE, eSkillChoiceCursor, 0);
    setStateCursor(battleMenu, BATTLE_ESKILL_STATE, eSkillChoiceCursor, 1);
    setStateCursor(battleMenu, BATTLE_ESKILL_STATE, eSkillChoiceCursor, 2);
}

void registerESkillMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_ESKILL_VIEW});
    gContext.eventBus.addListener(INIT_BATTLE_ESKILL, (SrEventCallback)&initBattleESkillViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleSelectESkill, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&handleExitESkill, modName, contextKeys);
}
