#ifndef MATERIA_MENU_H
#define MATERIA_MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../../events/menu_events.h"

SISTERRAY_API void materiaMenuUpdateHandler(i32 updateStateMask);
void displayActiveCursorStates(Menu* menu, u16 menuState, u32 stateControlMask);
void handleMateriaMenuInput(i32 updateStateMask, Menu* menuObject);

#endif
