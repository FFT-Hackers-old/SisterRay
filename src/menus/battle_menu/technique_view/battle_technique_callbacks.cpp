#include "battle_technique_callbacks.h"
#include "../../menu.h"
#include <unordered_set>
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initializeBattleTechniqueMenu() {
    CursorContext techniqueSelection = { 0, 0, 3, 3, 0, 0, 3, 12 / 3, 0, 0, 0, 0, 0, 1 };
    Cursor spellTechniqueCursor = { techniqueSelection, 8, 364, 32, 156 };
    auto battleMenu = gContext.menuWidgets.getElement(BATTLE_MENU_NAME);
    setStateCursor(battleMenu, BATTLE_TECHNIQUE_STATE, spellTechniqueCursor, 0);
    setStateCursor(battleMenu, BATTLE_TECHNIQUE_STATE, spellTechniqueCursor, 1);
    setStateCursor(battleMenu, BATTLE_TECHNIQUE_STATE, spellTechniqueCursor, 2);
}

void registerTechniqueMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({BATTLE_MENU});
    gContext.eventBus.addListener(DRAW_BATTLE_MENU, (SrEventCallback)&drawBattleTechniqueViewWidget, modName);
    gContext.eventBus.addListener(INIT_BATTLE_MENU, (SrEventCallback)&initBattleTechniqueViewWidget, modName);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&handleSelectTechnique, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&handleExitTechnique, modName, contextKeys);
}
