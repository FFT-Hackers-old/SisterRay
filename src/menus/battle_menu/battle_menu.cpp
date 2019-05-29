#include "battle_menu.h"
#include "../../impl.h"

typedef i32(*pfnsub6DD041)();
#define displayBaseMenu ((pfnsub6DD041)(0x6DD041))

typedef i32(*pfnsub6DE3D8)();
#define displayCommands ((pfnsub6DE3D8)(0x6DE3D8))

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
void drawBattleHandlers(i32 updateStateMask, u16 battleMenuState) {
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
