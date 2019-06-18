
#include "battle_spell_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleSpellWidgetNames;

void initializeBattleSpellMenu() {
    CursorContext spellSelection = { 0, 0, 3, 3, 0, 0, 3, MAGIC_COUNT / 3, 0, 0, 0, 0, 0, 1 };
    Cursor spellChoiceCursor = { spellSelection, 8, 364, 32, 156 };

    Cursor cursorArray[3] = { spellChoiceCursor, spellChoiceCursor, spellChoiceCursor }; /*One cursor per party member*/
    auto battleSpellMenu = createMenu(INIT_BATTLE_SPELL, 3, &cursorArray[0]);
    gContext.menuWidgets.add_element("BATTLE_SPELL", battleSpellMenu);
    gContext.menuWidgets.initializeMenu("BATTLE_SPELL", BATTLE_SPELL_MENU_NAME);
}

void registerSpellMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_SPELL_VIEW});
    gContext.eventBus.addListener(INIT_BATTLE_SPELL, (SrEventCallback)&initBattleMagicViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleSelectSpell, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&handleExitSpell, modName, contextKeys);
}
