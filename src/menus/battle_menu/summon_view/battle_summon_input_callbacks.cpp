#include "battle_summon_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"
#include "../../../battle/engine/battle_engine_interface.h"

using namespace BattleMenuWidgetNames;

/*Spell selection handler*/
void handleSelectSummon(const MenuInputEvent* event) {
    auto summonChoiceCursor = getStateCursor(event->menu, event->menuState, *BATTLE_ACTIVE_ACTOR_ID)->context;
    auto& enabledSummons = gContext.party.get_element(getPartyKey(*BATTLE_ACTIVE_ACTOR_ID)).actorSummons;
    if (*ACCEPTING_BATTLE_INPUT)
        return;

    if (event->menuState != BATTLE_SUMMON_STATE)
        return;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (summonChoiceCursor.maxColumnBound * (summonChoiceCursor.relativeRowIndex + summonChoiceCursor.baseRowIndex)) + summonChoiceCursor.relativeColumnIndex;
    if (enabledSummons[flatIndex].propertiesMask & 2 || enabledSummons[flatIndex].magicIndex == 0xFF) {
        playMenuSound(3);
        return;
    }
    playMenuSound(1);
    setChosenActionID(enabledSummons[flatIndex].magicIndex);
    setChosenActionMenuIndex(flatIndex);
    setTargetingFromFlags(enabledSummons[flatIndex].targetData, false);
    setMenuState(event->menu, BATTLE_TARGETING_STATE);
    *PREVIOUS_BATTLE_MENU_STATE = BATTLE_SUMMON_STATE;
}


void handleExitSummon(const MenuInputEvent* event) {
    if (event->menuState != BATTLE_SUMMON_STATE)
        return;
    playMenuSound(4);
    *ACCEPTING_BATTLE_INPUT = 1;
    setMenuState(event->menu, BATTLE_CMD_STATE);
}
