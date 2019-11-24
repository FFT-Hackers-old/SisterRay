#ifndef TRANSITION_H
#define TRANSITION_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "../widgets/widget.h"
#include "../events/event.h"
#include "menu_interface.h"

struct _TransitionData {
    i16 field_0;
    i16 field_2;
    i16 field_4;
    i16 field_6;
    i16 field_8;
    i16 field_A;
    i16 field_C;
    i16 field_E;
    i16 cIncrement;
    i16 eIncrement;
};

void controlStateView(const TransitionData& transition);
void handleOpenMenuState(Menu* menu, u32 menuState, TransitionData& transition);
void handleCloseMenuState(Menu* menu, u32 menuState, TransitionData& transition);
void handleTransition(Menu* menu, u32 menuState);
#endif
