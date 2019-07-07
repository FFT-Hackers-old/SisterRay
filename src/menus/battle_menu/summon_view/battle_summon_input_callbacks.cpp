#include "battle_summon_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"

using namespace BattleSummonWidgetNames;

/*Spell selection handler*/
void handleSelectSummon(const BattleSpellInputEvent* event) {
    auto summonChoiceCursor = getStateCursor(event->menu, event->menuState)->context;
    auto& enabledSummons = gContext.party.get_element(getPartyKey(event->menuState)).actorSummons;
    srLogWrite("Select Summon input handler");
    if (*ACCEPTING_BATTLE_INPUT)
        return;
    if (*BATTLE_MENU_STATE != 7)
        return;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (summonChoiceCursor.maxColumnBound * (summonChoiceCursor.relativeRowIndex + summonChoiceCursor.baseRowIndex)) + summonChoiceCursor.relativeColumnIndex;
    srLogWrite("trying to execute summon with flat index: %i", flatIndex);
    srLogWrite("magic index: %i", enabledSummons[flatIndex].magicIndex);
    srLogWrite("properties mask: %x", enabledSummons[flatIndex].propertiesMask);
    if (enabledSummons[flatIndex].propertiesMask & 2 || enabledSummons[flatIndex].magicIndex == 0xFF) {
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
    if (*BATTLE_MENU_STATE != 7)
        return;
    srLogWrite("HANDLING EXIT SUMMON MENU");
    playMenuSound(4);
    *ACCEPTING_BATTLE_INPUT = 1;
    *BATTLE_MENU_STATE = 1;
    setHandlerState(7, 3);
}
