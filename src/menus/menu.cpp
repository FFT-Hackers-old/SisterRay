#include "menu.h"
#include <utility>
#include "../impl.h"

void MenuRegistry::initializeMenu(std::string menuName, const std::string widgetName) {
    auto menu = getElement(menuName);
    auto menuWidget = createWidget(widgetName);
    menu->menuWidget = menuWidget;
    MenuInitEvent event = { menu };
    gContext.eventBus.dispatch(menu->initEvent, &event);

}

MenuRegistry::~MenuRegistry() {
    for (auto element : _resource_registry) {
        destroyMenu(element);
    }
}

SISTERRAY_API Menu* getMenu(const char* menuName) {
    return gContext.menuWidgets.getElement(std::string(menuName));
}

/* create menu with default cursors */
Menu* createMenu(SrEventType initEvent, SrEventType drawEvent, SrEventContext inputContext, u32 stateCount, Cursor* cursors) {
    Menu* menu = new Menu();
    menu->stateCount = stateCount;
    menu->initEvent = initEvent;
    menu->drawEvent = drawEvent;
    menu->inputContext = inputContext;
    menu->currentState = 0;
    if (cursors != nullptr) {
        for (auto i = 0; i < stateCount; i++) {
            menu->cursors[i][0] = cursors[i];
            menu->activeStateCursors[i] = 0;
            menu->stateStatus[i] = 0;
        }
    }
    menu->menuWidget = nullptr;
    return menu;
}

void destroyMenu(Menu* menu) {
    destroyWidget(menu->menuWidget);
    delete(menu);
}

void runMenu(Menu* menu, u32 updateStateMask) {
    MenuDrawEvent event = { menu, getMenuState(menu), updateStateMask };
    gContext.eventBus.dispatch(menu->drawEvent, &event);
    drawWidget(menu->menuWidget);
    /*for (u32 menuState = 0; menuState < menu->stateCount; menuState++) {
        handleTransition(menu, menuState);
    }*/
    if (menu->inputContext == BATTLE_MENU) {
        dispatchMenuInput(updateStateMask, menu, menu->inputContext);
        return;
    }
    if (!checkMenuInputEnabled()) {
        dispatchMenuInput(updateStateMask, menu, menu->inputContext);
    }
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
    else if (checkInputReceived(2)) {
        gContext.eventBus.dispatch(MENU_INPUT_R2, &event, dispatchContext);
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
    else if (checkInputReceived(256)) {
        gContext.eventBus.dispatch(MENU_INPUT_SELECT, &event, dispatchContext);
    }
    else if (captureDirectionInput(0x2000, 4)) {
        gContext.eventBus.dispatch(MENU_INPUT_RIGHT, &event, dispatchContext);
    }
    else if (captureDirectionInput(0x8000, 8)) {
        gContext.eventBus.dispatch(MENU_INPUT_LEFT, &event, dispatchContext);
    }
    if (activeCursor != nullptr) {
        if (menuObject->inputContext == BATTLE_MENU && *BATTLE_PAUSED) {
            return;
        }
        handleCursorPositionUpdate((u32*)(&(activeCursor->context)));
    }
}

SISTERRAY_API void addState(Menu* menu, Cursor* cursor, const char* stateName) {
    std::unordered_map<u32, Cursor> stateCursors(
        { {0, *cursor} }
    );
    menu->stateNames[std::string(stateName)] = menu->stateCount;
    menu->cursors[menu->stateCount] = stateCursors;
    menu->activeStateCursors[menu->stateCount] = 0;
    menu->stateStatus[menu->stateCount] = 0;
    menu->stateCount++;
    srLogWrite("Menu State %s assigned to idx %i", stateName, menu->stateNames[stateName]);
}


SISTERRAY_API u8 getStateByName(Menu* menu, const char* stateName) {
    return menu->stateNames[std::string(stateName)];
}

SISTERRAY_API void addCursorlessState(Menu* menu) {
    menu->cursors[menu->stateCount++];
    menu->stateStatus[menu->stateCount] = 0;
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

SISTERRAY_API void setStateCursor(Menu* menu, u32 menuState, Cursor cursor, u32 cursorIdx) {
    if (menuState < menu->stateCount) {
        menu->cursors[menuState][cursorIdx] = cursor;
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

SISTERRAY_API void setTransitionData(Menu* menu, u32 menuState, TransitionData transition) {
    if (menuState < menu->stateCount) {
        auto it = menu->transitionData.find(menuState);
        if (it != menu->transitionData.end()) {
            menu->transitionData[menuState] = transition;
        }
    }
}

SISTERRAY_API TransitionData* getTransitionData(Menu* menu, u32 menuState) {
    if (menuState < menu->stateCount) {
        auto it = menu->transitionData.find(menuState);
        if (it != menu->transitionData.end()) {
            return &(it->second);
        }
    }
    return nullptr;
}

SISTERRAY_API void setOpeningState(Menu* menu, u32 menuState) {
    if (menuState < menu->stateCount) {
        auto it = menu->stateStatus.find(menuState);
        if (it != menu->stateStatus.end()) {
            menu->stateStatus[menuState] = 1;
        }
    }
}

SISTERRAY_API void setClosingState(Menu* menu, u32 menuState) {
    if (menuState < menu->stateCount) {
        auto it = menu->stateStatus.find(menuState);
        if (it != menu->stateStatus.end()) {
            menu->stateStatus[menuState] = 2;
        }
    }
}

SISTERRAY_API void setNoTransitionState(Menu* menu, u32 menuState) {
    if (menuState < menu->stateCount) {
        auto it = menu->stateStatus.find(menuState);
        if (it != menu->stateStatus.end()) {
            menu->stateStatus[menuState] = 0;
        }
    }
}
