#ifndef BATTLE_MENU_H
#define BATTLE_MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>

SISTERRAY_API i32 renderBattleItemView();
SISTERRAY_API u32* initializeBattleItemMenuCursor();
bool isUsableInBattle(u16 itemID);
bool isThrowable(u16 itemID);

#endif
