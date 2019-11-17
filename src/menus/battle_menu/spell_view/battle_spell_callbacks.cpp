
#include "battle_spell_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initializeBattleSpellMenu() {
    CursorContext spellSelection = { 0, 0, 3, 3, 0, 0, 3, MAGIC_COUNT / 3, 0, 0, 0, 0, 0, 1 };
    Cursor spellChoiceCursor = { spellSelection, 8, 364, 32, 156 };
    auto battleMenu = gContext.menuWidgets.get_element(BATTLE_MENU_NAME);
    setStateCursor(battleMenu, BATTLE_MAGIC_STATE, &spellChoiceCursor, 0);
    setStateCursor(battleMenu, BATTLE_MAGIC_STATE, &spellChoiceCursor, 1);
    setStateCursor(battleMenu, BATTLE_MAGIC_STATE, &spellChoiceCursor, 2);
}

void registerSpellMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_MENU});
    gContext.eventBus.addListener(DRAW_BATTLE_MENU, (SrEventCallback)&drawBattleSpellViewWidget, modName);
    gContext.eventBus.addListener(INIT_BATTLE_SPELL, (SrEventCallback)&initBattleMagicViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleSelectSpell, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&handleExitSpell, modName, contextKeys);
}
