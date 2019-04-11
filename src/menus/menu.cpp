#include "menu.h"
#include <utility>
#include "../impl.h"
#include "../events/menu_events.h"

void MenuRegistry::initializeMenu(std::string menuName, const std::string widgetName) {
    named_registry[menuName]._buildWidget(widgetName);
}


Menu::Menu(cursorContext* cursorContextArray, i32 stateCount, SrEventType eventType) {
    widgetPtr = nullptr;
    // create the equipment Widget and add it's default children
    for (auto menuState = 0; menuState < stateCount; menuState++) {
        cursorContext context = cursorContextArray[menuState];
        cursorContextRegistry[menuState] = context;
    }
    initEvent = eventType;
    menuStateCount = stateCount;
    menuState = 0;
}

Menu::Menu() {
    widgetPtr = nullptr;
    // create the equipment Widget and add it's default children
    initEvent = NO_TYPE;
}

Menu::~Menu() {
    if (widgetPtr)
        destroyWidget(widgetPtr);
}

//This dispatches the current Menu instance through initializing callbacks
void Menu::_buildWidget(const std::string equipName) {
    auto menuWidget = createWidget(equipName);
    widgetPtr = menuWidget;
    const EquipInitEvent params = { this };
    const EquipInitEvent* event = &params;
    gContext.eventBus.dispatch(initEvent, event);
}

i32 Menu::getMenuState() {
    return menuState;
}

void Menu::setMenuState(i32 value) {
    if (value >= menuStateCount) {
        //Invalid Menu State, return to 0
        menuState = 0;
        return;
    }
    menuState = value;
}


Widget* Menu::getWidget() {
    return widgetPtr;
}
