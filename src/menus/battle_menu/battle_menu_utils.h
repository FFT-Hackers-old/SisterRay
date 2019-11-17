#ifndef BATTLE_MENU_UTILS_H
#define BATTLE_MENU_UTILS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../menu_utils.h"
#include "../../widgets/widget.h"
#include "../../events/menu_events.h"

/*Return True if the character is usable by the character attempting to use it*/
bool isUsableInBattle(u16 itemID);
bool isThrowable(u16 itemID); 
bool didItemUseSucceed(u16 itemID);
u16 getRestoreTypeGlobal(i16 itemID);
i32 setHandlerState(u16 handlerIndex, i8 state);

#endif // !BATTLE_MENU_UTILS_H
