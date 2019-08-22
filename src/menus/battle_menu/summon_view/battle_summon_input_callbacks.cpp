#include "battle_summon_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"

using namespace BattleMenuWidgetNames;

/*Spell selection handler*/
void handleSelectSummon(const BattleSpellInputEvent* event) {
    auto summonChoiceCursor = getStateCursor(event->menu, event->menuState, *BATTLE_ACTIVE_ACTOR_ID)->context;
    auto& enabledSummons = gContext.party.get_element(getPartyKey(*BATTLE_ACTIVE_ACTOR_ID)).actorSummons;
    if (*ACCEPTING_BATTLE_INPUT)
        return;

    if (event->menuState != SUMMON_BATTLE_STATE)
        return;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (summonChoiceCursor.maxColumnBound * (summonChoiceCursor.relativeRowIndex + summonChoiceCursor.baseRowIndex)) + summonChoiceCursor.relativeColumnIndex;
    if (enabledSummons[flatIndex].propertiesMask & 2 || enabledSummons[flatIndex].magicIndex == 0xFF) {
        playMenuSound(3);
    }
    else {
        playMenuSound(1);
        *ISSUED_ACTION_ID = enabledSummons[flatIndex].magicIndex;
        *GLOBAL_USED_ACTION_TARGET_DATA = enabledSummons[flatIndex].targetData;
        *GLOBAL_USED_MENU_INDEX = flatIndex;
        setCursorTargetingData();
        setMenuState(menu, 0);
        *PREVIOUS_BATTLE_MENU_STATE = SUMMON_BATTLE_STATE;
    }
}


void handleExitSummon(const BattleSpellInputEvent* event) {
    if (event->menuState != SUMMON_BATTLE_STATE)
        return;
    playMenuSound(4);
    *ACCEPTING_BATTLE_INPUT = 1;
    setMenuState(event->menu, 1);
}
