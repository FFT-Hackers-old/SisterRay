#include "battle_item_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleItemWidgetNames;

void initializeBattleItemMenu() {
    CursorContext eSkillContext = { 0, 0, 1, 3, 0, 0, 1, ESKILL_COUNT/2, 0, 0, 0, 0, 0, 1 };
    Cursor eSkillCursor = { eSkillContext, 36, 364, 32, 156 };

    Cursor cursorArray[3] = { eSkillCursor, eSkillCursor, eSkillCursor }; /*One cursor per party member*/
    auto battleItemMenu = createMenu(INIT_BATTLE_MENU, 3, &cursorArray[0]);
    gContext.menuWidgets.add_element("BATTLE_ITEM", battleItemMenu);
    gContext.menuWidgets.initializeMenu("BATTLE_ITEM", BATTLE_ITEM_MENU_NAME);
}

void registerItemMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_ITEM_VIEW});
    gContext.eventBus.addListener(INIT_BATTLE_MENU, (SrEventCallback)&initBattleItemViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&initBattleItemViewWidget, modName, contextKeys);
}
