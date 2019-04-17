#ifndef MENU_H
#define MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../named_resource_registry.h"
#include "../widgets/widget.h"
#include "../events/event.h"
#include "../api.h"

/*The Menu type is exposed via the public API, and therefore is defined only in terms of C types
  It is in encapsulated via opaque pointer, and should be interacted with the provided functions*/
struct _Menu {
    i32 currentState;
    i32 stateCount;
    Widget* menuWidget;
    SrEventType initEvent;
    CursorContext* contexts;
    i32 contextCapacity;
    i32 contextSize;
};

Menu* createMenu(SrEventType initEvent, i32 stateCount, CursorContext* contexts);
void destroyMenu(Menu* menu);

class MenuRegistry : public SrNamedResourceRegistry<Menu*, std::string> {
public:
    ~MenuRegistry();
    void initializeMenu(std::string menuName, const std::string widgetName);
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
