#ifndef BATTLE_BASE_DRAW_CALLBACKS_H
#define BATTLE_BASE_DRAW_CALLBACKS_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../menu_utils.h"
#include "../../../widgets/widget.h"
#include "../../menu.h"
#include "../battle_menu_widget_names.h"


void drawBaseViewWidget(const MenuDrawEvent* event);
void handleActorReady(const MenuDrawEvent* event);
void handleUpdateInputActive(const MenuDrawEvent* event);


char* srGetStrFromGlobalBuffer(u16 strBufferIndex);
void handleBattleStrSubstitions(char* ret, const char* base);
u16 insertEncodedStr(char* dst, u16 baseStart, const char* toInject);
#endif
