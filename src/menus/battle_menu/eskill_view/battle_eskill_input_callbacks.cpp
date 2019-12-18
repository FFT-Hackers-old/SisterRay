#include "battle_eskill_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"
#include "../../../battle/engine/battle_engine_interface.h"

using namespace BattleMenuWidgetNames;

/*Spell selection handler*/
void handleSelectESkill(const MenuInputEvent* event) {
    if (!checkHandlingInput())
        return;
    if (event->menuState != BATTLE_ESKILL_STATE)
        return;

    auto eSkillChoiceCursor = getStateCursor(event->menu, event->menuState)->context;
    auto& enabledESkills = gContext.party.getElement(getPartyKey(event->menuState)).actorEnemySkills;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (eSkillChoiceCursor.maxColumnBound * (eSkillChoiceCursor.relativeRowIndex + eSkillChoiceCursor.baseRowIndex)) + eSkillChoiceCursor.relativeColumnIndex;
    if (enabledESkills[flatIndex].propertiesMask & 2 || enabledESkills[flatIndex].magicIndex == 255) {
        playMenuSound(3);
        return;
    }
    playMenuSound(1);
    setChosenActionID(enabledESkills[flatIndex].magicIndex);
    setChosenActionMenuIndex(flatIndex);
    setTargetingFromFlags(gContext.battleInventory->getResource(flatIndex).targetFlags, false);
    setMenuState(event->menu, BATTLE_TARGETING_STATE);
    *PREVIOUS_BATTLE_MENU_STATE = 4;
}


void handleExitESkill(const MenuInputEvent* event) {
    if (*BATTLE_MENU_STATE != 4)
        return;
    playMenuSound(4);
    *ACCEPTING_BATTLE_INPUT = 1;
    *BATTLE_MENU_STATE = 1;
    setHandlerState(4, 3);
}
