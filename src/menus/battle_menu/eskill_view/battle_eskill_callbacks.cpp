
#include "battle_eskill_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleESkillWidgetNames;

void initializeBattleESkillMenu() {
    CursorContext eSkillContext = { 0, 0, 2, 3, 0, 0, 2, ESKILL_COUNT/2, 0, 0, 0, 0, 0, 1 };
    Cursor eSkillCursor = { eSkillContext, 36, 364, 32, 156 };

    Cursor cursorArray[3] = { eSkillCursor, eSkillCursor, eSkillCursor }; /*One cursor per party member*/
    auto battleSpellMenu = createMenu(INIT_BATTLE_ESKILL, 3, &cursorArray[0]);
    gContext.menuWidgets.add_element("BATTLE_ESKILL", battleSpellMenu);
    gContext.menuWidgets.initializeMenu("BATTLE_ESKILL", BATTLE_ESKILL_MENU_NAME);
}

void registerESkillMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_ESKILL_VIEW});
    gContext.eventBus.addListener(INIT_BATTLE_ESKILL, (SrEventCallback)&initBattleESkillViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&initBattleESkillViewWidget, modName, contextKeys);
}
