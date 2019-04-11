#include "equip_menu.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../menu.h"
#include <memory>

using namespace EquipWidgetNames;

SISTERRAY_API void equipMenuUpdateHandler(i32 updateStateMask) {
    Menu& menuObject = gContext.menuWidgets.getElementRef("EQUIP_MENU"); 
    auto menuWidget = menuObject.getWidget();
    displayEquipMenuViews(updateStateMask, menuWidget);
    if (!is_input_handling_enabled()) {
        handleEquipMenuInput(updateStateMask, menuWidget);
    }
}

//TODO: All of the magic numbers need to be pulled out and data driven
void displayEquipMenuViews(i32 stateControlMask, Widget* menuWidget) {
    cursorContext* cursorContextArray = (cursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;
    characterRecord* characterRecordArray = CHARACTER_RECORD_ARRAY;

    sub_6C98A6();
    EquipDrawEvent event = { menuWidget };
    gContext.eventBus.dispatch(DRAW_EQUIP_MENU, &event);
    drawWidget(menuWidget);

    //displayMenuObjects(cursorContextArray, *EQUIP_MENU_STATE, stateControlMask);
    displayMenuCursors(cursorContextArray, *EQUIP_MENU_STATE, stateControlMask);
    //displayMateriaSlots(cursorContextArray, *EQUIP_MENU_STATE, stateControlMask);
    //displayMenuTexts(cursorContextArray, *EQUIP_MENU_STATE, stateControlMask);
    //displayEquipGearStats(cursorContextArray, *EQUIP_MENU_STATE, stateControlMask);
}

//This function can be registered to a particular menu state to modularize modding the menus
void displayMenuObjects(cursorContext* cursorContextArray, u32 menuState, i32 stateControlMask) {
    u16 equippableGearCount;

    switch (menuState) {
        case 1: {
            //sub_6FA12F(316, 171, 324, 303); //I can't figure out what this does...
            equippableGearCount = gContext.gearViewData.slots_in_use;
            drawScrollerParams* arguments = (drawScrollerParams*)word_DCA490;
            setSideScrollerArguments(arguments, 8, equippableGearCount, cursorContextArray[1].baseRowIndex, 618, 171, 17, 303);
            renderMenuSideScroller(arguments, 0.2);
            //sub_6FA347();
            break;
        }
    }

    //HP bars and the like
    renderHPAndStatus(110, 17, *EQUIP_MENU_PARTY_INDEX, 1045220557); //HP bar
    displayPortrait(17, 9, *EQUIP_MENU_PARTY_INDEX, 1045220557); //Character Portrait

    //Draw Menu Boxes -- this part can be data driven
    for (u32 boxDataIndex = 0; boxDataIndex < 2; ++boxDataIndex) //index into box data structs and draw dem boxes
        gameDrawBox((i16*)(&(equipMenuWindowConfig)[boxDataIndex]), .3f);
    gameDrawBox((i16*)(&(equipMenuWindowConfig)[2]), .4f);
    gameDrawBox((i16*)(&(equipMenuWindowConfig)[3]), .5f);
    gameDrawBox((i16*)(&(equipMenuWindowConfig)[4]), .6f);
}

void displayMenuCursors(cursorContext* cursorContextArray, u16 menuState, u32 stateControlMask) {
    switch (menuState) {
        case 0: {
            displayCursor(207, 36 * cursorContextArray[0].relativeRowIndex + 17, 0.1f);
            break;
        }
        case 1: {
            if (stateControlMask & 2) //animate flashing cursor
                displayCursor(207, 36 * cursorContextArray[0].relativeRowIndex + 17, 0.1f);
            displayCursor(385, 36 * cursorContextArray[1].relativeRowIndex + 197, 0.1f);
            break;
        }
        default: {
            break;
        }
    }
}

color getStatDisplayColor(u8 equippedStat, u8 toEquipStat) {
    if (toEquipStat == equippedStat) {
        return COLOR_WHITE;
    }
    if (toEquipStat > equippedStat) {
        return COLOR_GREEN;
    }
    if (toEquipStat < equippedStat) {
        return COLOR_RED;
    }
    return COLOR_WHITE;
}

void handleEquipMenuInput(i32 updateStateMask, Widget* menuWidget) {
    cursorContext* cursorContextArray = (cursorContext*)EQUIP_MENU_CURSOR_CONTEXTS;
    characterRecord* characterRecordArray = CHARACTER_RECORD_ARRAY;
    u32 equipMenuState = *EQUIP_MENU_STATE;

    EquipInputEvent event = { menuWidget, *EQUIP_MENU_STATE };
    handleCursorPositionUpdate((u32*)(&(cursorContextArray[equipMenuState])));
    if (checkInputReceived(32)) {
        gContext.eventBus.dispatch(EQUIP_MENU_INPUT_OK, &event);
    }
    else if (checkInputReceived(64)) {
        gContext.eventBus.dispatch(EQUIP_MENU_INPUT_CANCEL, &event);
    }
    else if (checkInputReceived(4)) {
        gContext.eventBus.dispatch(EQUIP_MENU_INPUT_L1, &event);
    }
    else if (checkInputReceived(8)) {
        gContext.eventBus.dispatch(EQUIP_MENU_INPUT_R1, &event);
    }
    else if (checkInputReceived(128)) { //When switching to the materia view, square
        gContext.eventBus.dispatch(EQUIP_MENU_INPUT_SQUARE, &event);
    }
    else if (checkInputReceived(16)) { //unequip accessory
        gContext.eventBus.dispatch(EQUIP_MENU_INPUT_TRIANGLE, &event);
    }
}
