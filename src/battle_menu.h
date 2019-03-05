#ifndef BATTLE_MENU_H
#define BATTLE_MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>

SISTERRAY_API i32 renderBattleItemView();
SISTERRAY_API u32* initializeBattleItemMenuCursor();
SISTERRAY_API void battleItemMenuInputHandler();
bool isUsableInBattle(u16 itemID);
bool isThrowable(u16 itemID);
bool didItemUseSucceed(u16 itemID);
u16 getRestoreTypeGlobal(i16 itemID);
i32 setHandlerState(u16 handlerIndex, i8 state);

#endif
