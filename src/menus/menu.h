#ifndef MENU_H
#define MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../named_resource_registry.h"
#include "../widgets/widget.h"

class MenuWidgetRegistry : public SrNamedResourceRegistry<Widget*> {
public:
    ~MenuWidgetRegistry();
};

typedef struct {
    i16 drawDistance1;
    i16 drawDistance2;
    i16 drawDistance3;
    i16 drawDistance4;
} oldDrawBoxParams;

#define menuWindowConfig ((oldDrawBoxParams*)0x921C78)
#define equipMenuWindowConfig ((oldDrawBoxParams*)0x920AC8)

#endif
