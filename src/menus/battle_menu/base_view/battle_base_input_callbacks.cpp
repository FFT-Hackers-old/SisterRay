#include "battle_base_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"

using namespace BattleMenuWidgetNames;

/*Spell selection handler*/
void handlePauseBattle(const MenuInputEvent* event) {
    i32* menuStateMask = (i32*)(0xDC35B4);
    u8* byte_BFCDFC = (u8*)(0xBFCDFC);

    if (*byte_BFCDFC == 4) {
        srLogWrite("TOGGLING PAUSE BATTLE", *BATTLE_PAUSED);
        *BATTLE_PAUSED ^= 1u;
        if (*BATTLE_PAUSED)
            sub_6CE882(0x99u);
        else
            sub_6CE882(0x98u);
    }
}

void swapCharacterBattle(const MenuInputEvent* event) {
    u8* byte_DC2069 = (u8*)(0xDC2069);
    u8* byte_DC207B = (u8*)(0xDC207B);
    u8* byte_DC2082 = (u8*)(0xDC2082);
    u8* byte_DC2083 = (u8*)(0xDC2083);
    if (!checkHandlingInput())
        return;

    srLogWrite("RUNNING ACTOR SWITCH");
    if (getMenuState(event->menu) != BATTLE_INACTIVE) {
        srLogWrite("Handling ACTOR SWITCH");
        playMenuSound(1);
        cycleActors();
        setMenuState(event->menu, BATTLE_INACTIVE);
        *ACCEPTING_BATTLE_INPUT = 1;
    }

    /*srLogWrite("RUNNING ACTOR SWITCH");
    if ((*byte_DC2069 == 2 || *byte_DC207B == 2) && *byte_DC2082 != 2 && *byte_DC2083 != 2) {
        srLogWrite("Handling ACTOR SWITCH");
        playMenuSound(1);
        cycleActors();
        setMenuState(event->menu, BATTLE_INACTIVE);
        *ACCEPTING_BATTLE_INPUT = 1;
    }*/
}
