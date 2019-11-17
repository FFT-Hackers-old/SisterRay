#include "battle_spell_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"
#include "../../../battle/engine/battle_engine_interface.h"

using namespace BattleMenuWidgetNames;

/*Spell selection handler*/
void handleSelectSpell(const MenuInputEvent* event) {
    auto magicChoiceCursor = getStateCursor(event->menu, event->menuState, *BATTLE_ACTIVE_ACTOR_ID)->context;
    auto& enabledSpells = gContext.party.get_element(getPartyKey(event->menuState)).actorMagics;
    u16* restoreTypeGlobal = (u16*)(0xDC2088);
    if (*ACCEPTING_BATTLE_INPUT)
        return;

    if (event->menuState != BATTLE_MAGIC_STATE)
        return;

    *ACCEPTING_BATTLE_INPUT = 1;
    auto flatIndex = (magicChoiceCursor.maxColumnBound * (magicChoiceCursor.relativeRowIndex + magicChoiceCursor.baseRowIndex)) + magicChoiceCursor.relativeColumnIndex;
    if (enabledSpells[flatIndex].propertiesMask & 2 || enabledSpells[flatIndex].magicIndex == 255) {
        playMenuSound(3);
        return;
    }
    playMenuSound(1);
    setChosenActionID(enabledSpells[flatIndex].magicIndex);
    setChosenActionMenuIndex(flatIndex);
    setTargetingFromFlags(enabledSpells[flatIndex].targetData, false);
    setMenuState(event->menu, BATTLE_TARGETING_STATE);
    *PREVIOUS_BATTLE_MENU_STATE = BATTLE_MAGIC_STATE;
    //auto restoreType = gContext.attacks.get_element(assemblekey(2, enabledSpells[flatIndex].magicIndex)).attackData.restoreTypes;
    //*restoreTypeGlobal = restoreType;
    // if (*restoreTypeGlobal != 0xFFFF)
    //    initHandlerCursorState(-1, -1, 21); //Open up the "restore view" if it's defined
}


void handleExitSpell(const MenuInputEvent* event) {
    if (event->menuState != BATTLE_MAGIC_STATE)
        return;

    playMenuSound(4);
    *ACCEPTING_BATTLE_INPUT = 1;
    setMenuState(event->menu, BATTLE_CMD_STATE);
}
