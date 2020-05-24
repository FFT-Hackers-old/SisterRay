#include "transition.h"
#include "menu.h"

void controlStateView(const TransitionData& transition){
    u8* byte_DC206D = (u8*)0xDC206D;
    u8* byte_DC206E = (u8*)0xDC206E;
    u8* byte_DC206F = (u8*)0xDC206F;
    u8* byte_DC206C = (u8*)0xDC206C;
    u32* dword_DC105C = (u32*)0xDC105C;
    u32* dword_DC1060 = (u32*)0xDC1060;

    auto ctx = getGraphicsCtx();
    runContextCallbacks();

    if (!sub_41AB67(128) && !(*byte_DC206D != 2 && *byte_DC206E != 2 && *byte_DC206F != 2 && *byte_DC206C != 2)) {
        setViewport(
            transition.limitFastATBMask + *dword_DC105C + transition.field_0,
            transition.field_A + *dword_DC1060 + transition.field_2,
            transition.field_C,
            20,
            ctx);
    }
    setViewport(
        transition.limitFastATBMask + *dword_DC105C + transition.field_0,
        transition.field_A + *dword_DC1060 + transition.field_2,
        transition.field_C,
        transition.field_E,
        ctx);
}

void handleOpenMenuState(Menu* menu, u32 menuState, TransitionData& transition) {
    transition.field_C += transition.cIncrement;
    transition.field_E += transition.eIncrement;
    if (transition.field_C > transition.field_4 && transition.field_E > transition.field_6) {
        setNoTransitionState(menu, menuState);
    }
    if (transition.field_C > transition.field_4) {
        transition.field_C = transition.field_4;
    }
    if (transition.field_E > transition.field_6)
        transition.field_E = transition.field_6;

    transition.limitFastATBMask = (transition.field_4 / 2) - transition.field_C / 2;
    transition.field_A = (transition.field_6 / 2) - transition.field_E / 2;
}

void handleCloseMenuState(Menu* menu, u32 menuState, TransitionData& transition) {
    transition.field_C -= transition.cIncrement;
    transition.field_E -= transition.eIncrement;
    if (transition.field_C < 1 && transition.field_E < 1) {
        setNoTransitionState(menu, menuState);
    }
    if (transition.field_C <  1) {
        transition.field_C <  1;
    }
    if (transition.field_E < 1) {
        transition.field_E = 1;
    }
    transition.limitFastATBMask = (transition.field_4 / 2) - transition.field_C / 2;
    transition.field_A = (transition.field_6 / 2) - transition.field_E / 2;
}

void handleTransition(Menu* menu, u32 menuState) {
    auto contains = menu->stateStatus.find(menuState);
    auto stateStatus = 0;
    if (contains != menu->stateStatus.end()) {
        stateStatus = menu->stateStatus[menuState];
    }
    if ((stateStatus == 1) || (stateStatus == 2)) {
        auto containsT = menu->transitionData.find(menuState);
        if (containsT != menu->transitionData.end()) {
            auto transition = menu->transitionData[menuState];
            controlStateView(transition);
            if (stateStatus == 1) {
                handleOpenMenuState(menu, menuState, transition);
            }
            if (stateStatus == 2) {
                handleCloseMenuState(menu, menuState, transition);
            }
        }
    }
}
