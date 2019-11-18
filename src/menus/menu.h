#ifndef MENU_H
#define MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../sr_named_registry.h"
#include "../widgets/widget.h"
#include "../events/event.h"
#include "cursor.h"
#include "menu_interface.h"
#include <vector>
#include <unordered_map>

/*The Menu type is exposed via the public API, and therefore is defined only in terms of C types
  It is in encapsulated via opaque pointer, and should be interacted with the provided functions*/
struct _Menu {
    u32 currentState;
    u32 stateCount;
    Widget* menuWidget;
    SrEventType initEvent;
    SrEventType drawEvent;
    SrEventContext inputContext;
    std::unordered_map<u32, std::unordered_map<u32, Cursor>> cursors; //associate cursors to a various state by name
    std::unordered_map<u32, u32> activeStateCursors;
    std::unordered_map<u32, TransitionData> transitionData;
    std::unordered_map<u32, u8> stateStatus;
};

Menu* createMenu(SrEventType initEvent, SrEventType drawEvent, SrEventContext inputContext, u32 stateCount, Cursor* cursors=nullptr);
void destroyMenu(Menu* menu);
void runMenu(Menu* menu, u32 updateStateMask)

class MenuRegistry : public SrNamedResourceRegistry<Menu*, std::string> {
public:
    ~MenuRegistry();
    void initializeMenu(std::string menuName, const std::string widgetName);
};


//This will be deprecated soon
typedef struct {
    i16 drawDistance1;
    i16 drawDistance2;
    i16 drawDistance3;
    i16 drawDistance4;
} oldDrawBoxParams;

#define menuWindowConfig ((oldDrawBoxParams*)0x921C78)
#define equipMenuWindowConfig ((oldDrawBoxParams*)0x920AC8)

void dispatchMenuInput(i32 updateStateMask, Menu* menuObject, SrEventContext menuContext);

#endif
