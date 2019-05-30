#include "battle_menu.h"
#include "../../impl.h"

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


/*this function will replace the games version, handling drawing for battle menu view based on the menu state passed in as an argument
  It also handles input, so we will NOP the input functions the game relied on up to this point, which are separate, for these views
  We will also have to kill the games cursor/box drawing stuff eventually*/
u8 drawBattleHandlers(i32 updateStateMask, i16 battleMenuState) {
    u8* byte_DC3654 = (u8*)(0xDC3654);
    switch (battleMenuState) {
        case 0:
            displayBaseMenu();
            break;
        case 1:
            displayCommands();
            break;
        case 2:
            //drawChangeView(); //We want to expand change to add extra commands
            break;
        case 3:
            //drawDefendView(); 
        case 4: // This one will be redone with the widget system
            battleESkillUpdateHandler(updateStateMask);
            break;
        case 5:
            battleItemUpdateHandler(updateStateMask);
            break;
        case 6:
            battleSpellUpdateHandler(updateStateMask);
            break;
        case 7: // last one to be redone with the widget system
            battleSummonUpdateHandler(updateStateMask);
            break;
        case 9:
            sub_6E4B3C();
            break;
        case 19:
            //displayManipulateView(); //Needs to be re-implemented so that it still works
            break;
        case 20:
            sub_6E0D28();
            break;
        case 21:
            sub_6E1308();
            break;
        case 24:
            //displayLimitView();
            break;
        case 26:
            sub_6E2170();
            break;
        case 27:
            sub_6E3135();
            break;
        case 28:
            sub_6E384F();
            break;
        default:
            //sub_6DC1EB();
            break;
    }
    return ((*byte_DC3654) = (*byte_DC3654 + 1)) + 1;
}


#define DRAW_BATTLE_VIEWS                  ((void*)0x6D82EA)
#define BATTLE_SPELL_INPUT_HANDLER         ((void*)0x6D9B98)
#define BATTLE_SUMMON_INPUT_HANDLER        ((void*)0x6DA072)
#define BATTLE_ESKILL_INPUT_HANDLER        ((void*)0x6DA3A8)
#define BATTLE_ITEM_INPUT_HANDLER          ((void*)0x6D98E3)

void initializeBattleMenu() {
    registerSpellMenuListeners();
    initializeBattleSpellMenu();
    registerSummonMenuListeners();
    initializeBattleSummonMenu();
    registerItemMenuListeners();
    initializeBattleItemMenu();
    registerESkillMenuListeners();
    initializeBattleESkillMenu();
    mogReplaceFunction(DRAW_BATTLE_VIEWS, &drawBattleHandlers);
    mogReplaceFunction(BATTLE_SPELL_INPUT_HANDLER, &battleSpellInputHandler);
    mogReplaceFunction(BATTLE_SUMMON_INPUT_HANDLER, &battleSummonInputHandler);
    mogReplaceFunction(BATTLE_ESKILL_INPUT_HANDLER, &battleESkillInputHandler);
    mogReplaceFunction(BATTLE_ITEM_INPUT_HANDLER, &battleItemInputHandler);
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
