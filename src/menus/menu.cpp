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
    for (auto element : named_registry) {
        destroyMenu(element.second);
    }
}

Menu* createMenu(SrEventType initEvent, i32 stateCount, CursorContext* contexts) {
    Menu* menu = (Menu*)malloc(sizeof(Menu));
    menu->stateCount = stateCount;
    menu->initEvent = initEvent;
    menu->contextCapacity = stateCount;
    menu->currentState = 0;
    menu->contexts = (CursorContext*)malloc(sizeof(CursorContext)*stateCount);
    memcpy(menu->contexts, contexts, sizeof(CursorContext)*stateCount);
    menu->contextSize = stateCount;
    menu->menuWidget = nullptr;
    return menu;
}

void destroyMenu(Menu* menu) {
    free(menu->contexts);
    destroyWidget(menu->menuWidget);
    free(menu);
}

void addState(Menu* menu, CursorContext* context) {
    if (menu->contextSize < menu->contextCapacity) {
        memcpy(&(menu->contexts[menu->contextSize]), context, sizeof(CursorContext));
        menu->contextSize++;
        menu->stateCount++;
        return;
    }
    menu->contexts = (CursorContext*)realloc(menu->contexts, sizeof(CursorContext) * menu->contextCapacity * 2);
    menu->contextCapacity = 2 * menu->contextCapacity;
    memcpy(&(menu->contexts[menu->contextSize]), context, sizeof(CursorContext));
    menu->contextSize++;
    menu->stateCount++;
}

CursorContext* getStateCursor(Menu* menu, i32 menuState) {
    if (menuState < menu->stateCount) {
        srLogWrite("returning addr of cursor for state %i", menuState);
        srLogWrite("addr of cursor %p:", &(menu->contexts[menuState]));
        return &(menu->contexts[menuState]);
    }
    return nullptr;
}

void setStateCursor(Menu* menu, i32 menuState, CursorContext* context) {
    if (menuState < menu->stateCount) {
        memcpy(&(menu->contexts[menuState]), context, sizeof(CursorContext));
    }
}

i32 getMenuState(Menu* menu) {
    return menu->currentState;
}

void setMenuState(Menu* menu, i32 value) {
    if (value < menu->stateCount) {
        menu->currentState = value;
    }
}

Widget* getWidget(Menu* menu) {
    return menu->menuWidget;
}
