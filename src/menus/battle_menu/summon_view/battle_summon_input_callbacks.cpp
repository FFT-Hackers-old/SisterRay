#include "battle_summon_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"

using namespace BattleSummonWidgetNames;

/*Spell selection handler*/
void handleSelectSummon(const BattleSpellInputEvent* event) {
    auto summonChoiceCursor = getStateCursor(event->menu, event->menuState)->context;
    auto& enabledSummons = gContext.party.get_element(getPartyKey(event->menuState)).actorSummons;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (summonChoiceCursor.maxColumnBound * (summonChoiceCursor.relativeRowIndex + summonChoiceCursor.baseRowIndex)) + summonChoiceCursor.relativeColumnIndex;
    if (enabledSummons[flatIndex].propertiesMask & 2 || enabledSummons[flatIndex].magicIndex == 255) {
        playMenuSound(3);
    }
    else {
        playMenuSound(1);
        *ISSUED_ACTION_ID = enabledSummons[flatIndex].magicIndex;
        *GLOBAL_USED_ACTION_TARGET_DATA = enabledSummons[flatIndex].targetData;
        *GLOBAL_USED_MENU_INDEX = flatIndex;
        setCursorTargetingData();
        *BATTLE_MENU_STATE = 0;
        *PREVIOUS_BATTLE_MENU_STATE = 7;
    }
}


void handleExitSummon(const BattleSpellInputEvent* event) {
    playMenuSound(4);
    *ACCEPTING_BATTLE_INPUT = 1;
    *BATTLE_MENU_STATE = 1;
    setHandlerState(7, 3);
}
