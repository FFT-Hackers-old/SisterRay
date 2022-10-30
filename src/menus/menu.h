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

struct AuxillaryCursor {
    u32 stateIdx;
    bool isFlashing;
};

struct _Menu {
    u32 currentState;
    bool cursorsActive; //Set by transition handlers, determines whether or not cursors registered to the current state are drawn
    bool dispatchInput; //Set by transition handlers, determines whether or not input handlers are dispatched
    u32 stateCount;
    Widget* menuWidget;
    SrEventType initEvent; //Attempt to deprecate by making initialization data driven
    SrEventType drawEvent;
    SrEventContext inputContext;
    std::unordered_map<std::string, u32> stateNames;
    std::unordered_map<u32, std::unordered_map<u32, Cursor>> cursors; //associate cursors to a various state by name
    std::unordered_map<u32, std::vector<AuxillaryCursor>> auxillaryDrawCursors; // list of non-interactable cursors to draw with each state
    std::unordered_map<u32, u32> activeStateCursors; //Index of the active cursors for each particular menu state
    std::unordered_map<u32, u32> stateStatus;
};

Menu* createMenu(SrEventType initEvent, SrEventType drawEvent, SrEventContext inputContext, u32 stateCount, Cursor* cursors=nullptr);
void destroyMenu(Menu* menu);
void runMenu(Menu* menu, u32 updateStateMask);

class MenuRegistry : public SrNamedResourceRegistry<Menu*, std::string> {
public:
    ~MenuRegistry();
    void initializeMenu(std::string menuName, const std::string widgetName);
};


//This will be deprecated soon
typedef struct {
    u16 x;
    u16 y;
    u16 width;
    u16 height;
} oldDrawBoxParams;

#define menuWindowConfig ((oldDrawBoxParams*)0x921C78)
#define equipMenuWindowConfig ((oldDrawBoxParams*)0x920AC8)

void dispatchMenuInput(i32 updateStateMask, Menu* menuObject, SrEventContext menuContext);

#endif
