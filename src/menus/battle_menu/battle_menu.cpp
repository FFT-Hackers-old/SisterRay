#include "battle_menu.h"
#include "../../impl.h"

using namespace BattleMenuWidgetNames;

void battleMenuUpdateHandler(i32 updateStateMask) {
    Menu* menu = gContext.menuWidgets.get_element(BATTLE_MENU_NAME);
    i32* menuStateMask = (i32*)(0xDC35B4);
    sub_6C98A6();
    runMenu(menu, *menuStateMask);
    drawCursor(getStateCursor(menu, menu->currentState, *BATTLE_ACTIVE_ACTOR_ID), 0.1f);
}

typedef i32(*pfnsub6DD041)();
#define displayBaseMenu ((pfnsub6DD041)(0x6DD041))

typedef i32(*pfnsub6DE3DB)();
#define displayCommands ((pfnsub6DE3DB)(0x6DE3DB))

typedef i32(*pfnsub6E4B3C)();
#define sub_6E4B3C      ((pfnsub6E4B3C)(0x6E4B3C))

typedef i32(*pfnsub6E0D28)();
#define sub_6E0D28      ((pfnsub6E0D28)(0x6E0D28))

typedef i32(*pfnsub6E1308)();
#define sub_6E1308      ((pfnsub6E1308)(0x6E1308))

typedef i32(*pfnsub6E2170)();
#define sub_6E2170      ((pfnsub6E2170)(0x6E2170))

typedef i32(*pfnsub6E3135)();
#define sub_6E3135      ((pfnsub6E3135)(0x6E3135))

typedef i32(*pfnsub6E384F)();
#define sub_6E384F      ((pfnsub6E384F)(0x6E384F))

/*void resetBattleMenu() {
    byte_C05F6C = 4;
    sub_6CE811(4);
    byte_DC3D14 = 1;
    byte_DC3860 = 0;
    byte_DC3650 = dword_DC08B8 & 0x7F;
    byte_DC3864 = 0;
    sub_6DCBAA();
    byte_DC38F8 = ACTIVE_LIMIT_MASK;
    word_DC38D4 = 1;
    BATTLE_MENU_STATE = -1;
    sub_6DB0BC();
    if (word_DC38D4)
        word_DC38D4 = 0;
    gamePausedGlobal = 0;
    gBattlePaused = 0;
    word_DC1F3C = ACTIVE_LIMIT_MASK;
    CURSOR_MEMORY_ACTIVE = ((signed int)(unsigned __int16)word_DC0E12 >> 4) & 3;
    initHandlerCursorState(-1, -1, 0);
    word_CC0828 = 2;
}*/

void dispatchBattleUpdates() {
    void* ffContext;
    i32* menuStateMask = (i32*)(0xDC35B4);
    u32* dword_91BD68 = (u32*)(0x91BD68);
    u32* dword_BF2848 = (u32*)(0xBF2848);
    u32* dword_DB9580 = (u32*)(0xDB9580);
    u32* dword_DC1F40 = (u32*)(0xDC1F40);
    u32* dword_91E7B8 = (u32*)(0x91E7B8);
    u32* dword_DC2094 = (u32*)(0xDC2094);
    u32* dword_DC2098 = (u32*)(0xDC1F40);
    u32* dword_DC1F44 = (u32*)(0xDC1F44);
    u32* dword_DC1F48 = (u32*)0xDC1F48;
    u16* word_DC1F3C = (u16*)(0xDC1F3C);

    *dword_DB9580 = 0;
    if (*dword_91BD68 != *dword_BF2848) {
        *dword_DB9580 = 1;
        *word_DC1F3C = *LIMIT_ACTIVE_MASK;
        *dword_91E7B8 = 0;
        *dword_DC2094 = -256;
        *dword_DC2098 = -256;
        *dword_DC1F48 = 0;
    }
    *dword_DC1F40 ^= 1u;
    
    ffContext = getGraphicsCtx();
    sub_41A21E(ffContext);

    if (*dword_91BD68 != *dword_BF2848) {
        *dword_91BD68 = *dword_BF2848;
        *BATTLE_PAUSED_GLOBAL = *gBattlePaused;
    }

    battleMenuUpdateHandler(*menuStateMask);
 
    if (!*gBattlePaused)
        incrementTimers();
    ++(*dword_DC1F44);
}


void initializeBattleMenu() {
    auto battleMenu = createMenu(INIT_BATTLE_MENU, DRAW_BATTLE_MENU, BATTLE_MENU, 64);
    gContext.menuWidgets.add_element(BATTLE_MENU_NAME, battleMenu);
    gContext.menuWidgets.initializeMenu(BATTLE_MENU_NAME, BATTLE_MENU_WIDGET_NAME);
    registerBaseViewListeners();
    initializeBattleBaseMenu();
    registerTargetingMenuListeners();
    initializeBattleTargetingMenu();
    registerCommandMenuListeners();
    initializeBattleCommandMenu();
    //registerSpellMenuListeners();
    //initializeBattleSpellMenu();
    //registerSummonViewListeners();
    //initializeBattleSummonMenu();
    //registerItemMenuListeners();
    //initializeBattleItemMenu();
    //registerESkillMenuListeners();
    //initializeBattleESkillMenu();
}

/*Change this code so we can add more choices*/
/*void drawChangeView() {
    const char *v2; 
    unsigned __int8 v5; 
    if ((unsigned __int8)battleType < 3u || (unsigned __int8)battleType > 8u)
        v5 = 7;
    else
        v5 = 0;
    v2 = (const char *)getGameText(5u, 18, 8);
    displayTextAtLocation(
        *(signed __int16 *)(PtrToBattleMenuArrays + 152 * battleState)
        + *(signed __int16 *)(PtrToBattleMenuArrays + 152 * battleState + 18)
        + 16,
        *(signed __int16 *)(PtrToBattleMenuArrays + 152 * battleState + 2)
        + *(signed __int16 *)(PtrToBattleMenuArrays + 152 * battleState + 20)
        + 14,
        v2,
        v5,
        0.40099999);
}*/

/*void drawDefendView() {
    const char *v3; // eax
    unsigned __int8 v5; // [esp+8h] [ebp-4h]
    // defend command
    v3 = (const char *)getGameText(5u, 19, 8);
    displayTextAtLocation(
        *(signed __int16 *)(PtrToBattleMenuArrays + 152 * battleState)
        + *(signed __int16 *)(PtrToBattleMenuArrays + 152 * battleState + 18)
        + 18,
        *(signed __int16 *)(PtrToBattleMenuArrays + 152 * battleState + 2)
        + *(signed __int16 *)(PtrToBattleMenuArrays + 152 * battleState + 20)
        + 14,
        v3,
        7u,
        0.40099999);
}
*/
