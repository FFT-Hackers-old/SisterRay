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

/* create menu with default cursors */
Menu* createMenu(SrEventType initEvent, i32 stateCount, Cursor* cursors) {
    Menu* menu = new Menu();
    menu->stateCount = stateCount;
    menu->initEvent = initEvent;
    menu->currentState = 0;
    if (cursors != nullptr) {
        for (auto i = 0; i < stateCount; i++) {
            menu->cursors[i][0] = cursors[stateCount];
            menu->activeStateCursors[i] = 0;
        }
    }
    menu->menuWidget = nullptr;
    return menu;
}

void destroyMenu(Menu* menu) {
    destroyWidget(menu->menuWidget);
    delete(menu);
}

void dispatchMenuInput(i32 updateStateMask, Menu* menuObject, SrEventContext menuContext) {
    auto menuState = menuObject->currentState;
    auto activeCursor = getStateCursor(menuObject, menuState, getActiveCursorIndex(menuObject, menuState));
    auto menuWidget = menuObject->menuWidget;

    MenuInputEvent event = { menuObject, menuState };
    auto dispatchContext = std::vector<SrEventContext>({ menuContext });
    if (checkInputReceived2(2048)) {
        gContext.eventBus.dispatch(MENU_INPUT_START, &event, dispatchContext);
    }
    if (checkInputReceived(32)) {
        gContext.eventBus.dispatch(MENU_INPUT_OK, &event, dispatchContext);
    }
    else if (checkInputReceived(64)) {
        gContext.eventBus.dispatch(MENU_INPUT_CANCEL, &event, dispatchContext);
    }
    else if (checkInputReceived(4)) {
        gContext.eventBus.dispatch(MENU_INPUT_L1, &event, dispatchContext);
    }
    else if (checkInputReceived(8)) {
        gContext.eventBus.dispatch(MENU_INPUT_R1, &event, dispatchContext);
    }
    else if (checkInputReceived(128)) {
        gContext.eventBus.dispatch(MENU_INPUT_SQUARE, &event, dispatchContext);
    }
    else if (checkInputReceived(16)) { 
        gContext.eventBus.dispatch(MENU_INPUT_TRIANGLE, &event, dispatchContext);
    }
    else if (captureDirectionInput(0x2000, 4)) {
        gContext.eventBus.dispatch(MENU_INPUT_RIGHT, &event, dispatchContext);
    }
    else if (captureDirectionInput(0x8000, 8)) {
        gContext.eventBus.dispatch(MENU_INPUT_LEFT, &event, dispatchContext);
    }
    if (activeCursor != nullptr)
        handleCursorPositionUpdate((u32*)(&(activeCursor->context)));
}

SISTERRAY_API void addState(Menu* menu, Cursor* cursor) {
    std::unordered_map<std::string, Cursor> stateCursors = {{std::string(std::to_string(0)), cursor}};
    menu->cursors[menu->stateCount++] = stateCursors;
}

SISTERRAY_API void addCursorlessState(Menu* menu) {
    menu->cursors[menu->stateCount++];
}

SISTERRAY_API Cursor* getStateCursor(Menu* menu, u32 menuState, u32 cursorIdx) {
    if (menuState < menu->stateCount) {
        auto it = menu->cursors[menuState].find(cursorIdx);
        if (it != menu->cursors[menuState].end()) {
            return &(it->second);
        }
    }
    return nullptr;
}

SISTERRAY_API void setStateCursor(Menu* menu, u32 menuState, Cursor* cursor, u32 cursorIdx) {
    if (menuState < menu->stateCount) {
        auto it = menu->cursors[menuState].find(cursorIdx);
        if (it != menu->cursors[menuState].end()) {
            menu->cursors[menuState][cursorIdx] = *cursor;
        }
    }
}

SISTERRAY_API u32 getMenuState(Menu* menu) {
    return menu->currentState;
}

SISTERRAY_API void setMenuState(Menu* menu, u32 value) {
    if (value < menu->stateCount) {
        menu->currentState = value;
    }
}

SISTERRAY_API Widget* getWidget(Menu* menu) {
    return menu->menuWidget;
}

SISTERRAY_API u32 getActiveCursorIndex(Menu* menu, u32 menuState) {
    return menu->activeStateCursors[menuState];
}

SISTERRAY_API void setActiveCursorIndex(Menu* menu, u32 menuState, u32 cursorIndex) {
    if (menuState < menu->stateCount) {
        if (menu->cursors[menuState].find(cursorIndex) != menu->cursors[menuState].end()) {
            menu->activeStateCursors[menuState] = cursorIndex;
        }
    }
}
