#ifndef MENU_H
#define MENU_H

#include <SisterRay/types.h>
#include <SisterRay/SisterRay.h>
#include "../named_resource_registry.h"
#include "../widgets/widget.h"
#include "../events/event.h"

/*Menu objects consist of a widget, a current menu state, and a registry of cursorContexts one per state*/
class Menu {
public:
    Menu(cursorContext* cursorContextArray, i32 menuStateCount, SrEventType initEvent, const std::string equipName);
    Menu();
    ~Menu();
    i32 menuState;
    i32 menuStateCount;
    SrEventType initEvent;
    Widget* widgetPtr;

    std::unordered_map<i32, cursorContext> cursorContextRegistry;
    void initializeMenu();
    i32 getMenuState();
    void setMenuState(i32 value);
    Widget* getWidget();
    
};

class MenuRegistry : public SrNamedResourceRegistry<Menu> {
public:
    void initializeMenu(std::string menuName); //dispatches handlers registered to "init", restoring the widget to its original state
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
