#include "battle_base_callbacks.h"
#include "../../menu.h"
#include "../../../impl.h"
#include "../battle_menu_utils.h"

using namespace BattleMenuWidgetNames;

typedef void(*pfnsub6CE882)(i16);
#define sub_6CE862   ((pfnsub6CE882)0x6CE882)

/*Spell selection handler*/
void handlePauseBattle(const MenuInputEvent* event) {
    void* ffContext;
    u8* gBattlePaused = (u8*)(0xDC0E70);
    u8* gamePausedGlobal = (u8*)0xDC0E6C;
    i32* menuStateMask = (i32*)(0xDC35B4);
    u8* byte_BFCDFC = (u8*)(0xBFCDFC);

    if (*byte_BFCDFC == 4) {
        *gBattlePaused ^= 1u;
        if (*gBattlePaused)
            // This handle changes the audio based on whether or not the game is paused
            sub_6CE882(0x99u);
        else
            sub_6CE882(0x98u);
    }
}
