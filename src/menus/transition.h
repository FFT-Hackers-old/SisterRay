#ifndef TRANSITION_H
#define TRANSITION_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "../widgets/widget.h"
#include "../events/event.h"
#include "menu_interface.h"

struct _TransitionData {
    u16 field_0;
    u16 field_2;
    u16 field_4;
    u16 field_6;
    u16 field_8;
    u16 field_A;
    u16 field_C;
    u16 field_E;
};

void controlStateView(const TransitionData& transition);
#endif
