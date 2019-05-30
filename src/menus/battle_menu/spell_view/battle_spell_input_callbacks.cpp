#include "battle_spell_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"

using namespace BattleSpellWidgetNames;

/*Spell selection handler*/
void handleSelectSpell(const BattleSpellInputEvent* event) {
    auto magicChoiceCursor = getStateCursor(event->menu, event->menuState)->context;
    auto& enabledSpells = gContext.party.get_element(getPartyKey(event->menuState)).actorMagics;
    u16* restoreTypeGlobal = (u16*)(0xDC2088);
    if (*BATTLE_MENU_STATE != 6)
        return;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (magicChoiceCursor.maxColumnBound * (magicChoiceCursor.relativeRowIndex + magicChoiceCursor.baseRowIndex)) + magicChoiceCursor.relativeColumnIndex;
    if (enabledSpells[flatIndex].propertiesMask & 2 || enabledSpells[flatIndex].magicIndex == 255) {
        playMenuSound(3);
    }
    else {
        playMenuSound(1);
        *ISSUED_ACTION_ID = enabledSpells[flatIndex].magicIndex;
        *GLOBAL_USED_ACTION_TARGET_DATA = enabledSpells[flatIndex].targetData;
        *GLOBAL_USED_MENU_INDEX = flatIndex;
        setCursorTargetingData();
        *BATTLE_MENU_STATE = 0;
        *PREVIOUS_BATTLE_MENU_STATE = 6;
        auto restoreType = gContext.attacks.get_element(assemblekey(2, enabledSpells[flatIndex].magicIndex)).attackData.restoreTypes;
        *restoreTypeGlobal = restoreType;
        if (*restoreTypeGlobal != 0xFFFF)
            initHandlerCursorState(-1, -1, 21); //Open up the "restore view" if it's defined
    }
}


void handleExitSpell(const BattleSpellInputEvent* event) {
    if (*BATTLE_MENU_STATE != 6)
        return;
    playMenuSound(4);
    *ACCEPTING_BATTLE_INPUT = 1;
    *BATTLE_MENU_STATE = 1;
    setHandlerState(6, 3);
}
