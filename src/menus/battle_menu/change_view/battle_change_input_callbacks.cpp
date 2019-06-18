#include "battle_change_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"

using namespace BattleChangeWidgetNames;

/*Spell selection handler*/
void handleSelectChange(const BattleSpellInputEvent* event) {
    auto changeCommandChoice = getStateCursor(event->menu, event->menuState)->context;
    auto& enabledESkills = gContext.party.get_element(getPartyKey(event->menuState)).actorEnemySkills;
    if (*ACCEPTING_BATTLE_INPUT)
        return;
    if (*BATTLE_MENU_STATE != 4)
        return;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (changeCommandChoice.maxColumnBound * (changeCommandChoice.relativeRowIndex + changeCommandChoice.baseRowIndex)) + changeCommandChoice.relativeColumnIndex;
    if (enabledESkills[flatIndex].propertiesMask & 2 || enabledESkills[flatIndex].magicIndex == 255) {
        playMenuSound(3);
    }
    else {
        playMenuSound(1);
        *ISSUED_ACTION_ID = enabledESkills[flatIndex].magicIndex;
        *GLOBAL_USED_ACTION_TARGET_DATA = enabledESkills[flatIndex].targetData;
        *GLOBAL_USED_MENU_INDEX = flatIndex;
        setCursorTargetingData();
        *BATTLE_MENU_STATE = 0;
        *PREVIOUS_BATTLE_MENU_STATE = 4;
    }
}


void handleExitChange(const BattleSpellInputEvent* event) {
    if (*BATTLE_MENU_STATE != 4)
        return;
    playMenuSound(4);
    *ACCEPTING_BATTLE_INPUT = 1;
    *BATTLE_MENU_STATE = 1;
    setHandlerState(4, 3);
}
