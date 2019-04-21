#include "menu.h"
#include <utility>
#include "../impl.h"
#include "../events/menu_events.h"

void MenuRegistry::initializeMenu(std::string menuName, const std::string widgetName) {
    auto menu = get_element(menuName);
    auto menuWidget = createWidget(widgetName);
    menu->menuWidget = menuWidget;
    EquipInitEvent event = { menu };
    gContext.eventBus.dispatch(menu->initEvent, &event);

}

MenuRegistry::~MenuRegistry() {
    for (auto element : _resource_registry) {
        destroyMenu(element);
    }
}

SISTERRAY_API Menu* getMenu(char* menuName) {
    return gContext.menuWidgets.get_element(std::string(menuName));
}

Menu* createMenu(SrEventType initEvent, i32 stateCount, Cursor* contexts) {
    Menu* menu = (Menu*)malloc(sizeof(Menu));
    menu->stateCount = stateCount;
    menu->initEvent = initEvent;
    menu->contextCapacity = stateCount;
    menu->currentState = 0;
    menu->contexts = (Cursor*)malloc(sizeof(Cursor)*stateCount);
    memcpy(menu->contexts, contexts, sizeof(Cursor)*stateCount);
    menu->contextSize = stateCount;
    menu->menuWidget = nullptr;
    return menu;
}

void destroyMenu(Menu* menu) {
    free(menu->contexts);
    destroyWidget(menu->menuWidget);
    free(menu);
}

SISTERRAY_API void addState(Menu* menu, Cursor* context) {
    if (menu->contextSize < menu->contextCapacity) {
        memcpy(&(menu->contexts[menu->contextSize]), context, sizeof(Cursor));
        menu->contextSize++;
        menu->stateCount++;
        return;
    }
    menu->contexts = (Cursor*)realloc(menu->contexts, sizeof(Cursor) * menu->contextCapacity * 2);
    menu->contextCapacity = 2 * menu->contextCapacity;
    memcpy(&(menu->contexts[menu->contextSize]), context, sizeof(Cursor));
    menu->contextSize++;
    menu->stateCount++;
}

SISTERRAY_API Cursor* getStateCursor(Menu* menu, i32 menuState) {
    if (menuState < menu->stateCount) {
        return &(menu->contexts[menuState]);
    }
    return nullptr;
}

SISTERRAY_API void setStateCursor(Menu* menu, i32 menuState, Cursor* cursor) {
    if (menuState < menu->stateCount) {
        memcpy(&(menu->contexts[menuState]), cursor, sizeof(Cursor));
    }
}

SISTERRAY_API i32 getMenuState(Menu* menu) {
    return menu->currentState;
}

SISTERRAY_API void setMenuState(Menu* menu, i32 value) {
    if (value < menu->stateCount) {
        menu->currentState = value;
    }
}

SISTERRAY_API Widget* getWidget(Menu* menu) {
    return menu->menuWidget;
}
