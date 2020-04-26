#include "battle_move_init_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initBattleMoveViewWidget(const MenuInitEvent* event) {
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto battleCommandView = createWidget(BATTLE_MOVE_WIDGET_NAME);

    boxParams = {
        15 + 25,
        340,
        120,
        120,
        0.502f
    };
    boxWidget = createBoxWidget(boxParams, MOVE_GRID_BOX);
    addChildWidget(battleCommandView, (Widget*)boxWidget, MOVE_GRID_BOX);

    std::vector<std::string> names = { PARTY_1_MOVE_GRID, PARTY_2_MOVE_GRID, PARTY_3_MOVE_GRID };
    for (auto idx = 0; idx < names.size(); idx++) {
        auto cmdItemChoice = getStateCursor(event->menu, idx);

        drawGridParams gridParams = { BATTLE_MENU_NAME.c_str(), BATTLE_MOVE_STATE, &battleMoveNameViewUpdater, 15 + 8 + 25, 350, allocateMoveRow, idx };
        gridWidget = createGridWidget(gridParams, names[idx]);
        addChildWidget(battleCommandView, (Widget*)gridWidget, names[idx]);
    }

    addChildWidget(mainWidget, battleCommandView, BATTLE_MOVE_WIDGET_NAME);
}

Widget* allocateMoveRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto cmdWidget = createWidget(name);
    moveWidget(cmdWidget, xCoordinate, yCoordinate);
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.501f };
    addChildWidget(cmdWidget, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
    return cmdWidget;
}

void battleMoveNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    EncodedString actionName;
    switch (flatIndex) {
    case 0: {
        actionName = EncodedString::from_unicode("Advance");
        break;
    }
    case 1: {
        actionName = EncodedString::from_unicode("Retreat");
        break;
    }
    case 2 : {
        actionName = EncodedString::from_unicode("PHS");
        break;
    }
    }
    if (flatIndex > 2) {
        disableWidget(getChild(widget, std::string("ARW")));
        disableWidget(getChild(widget, std::string("TXT")));
        return;
    }
    enableWidget(getChild(widget, std::string("TXT")));
    updateText(getChild(widget, std::string("TXT")), actionName.str());
    auto color = COLOR_WHITE;
    /*if (commands[flatIndex].commandFlags & 2) {
        auto color = COLOR_GRAY;
    }*/
    updateTextColor(widget, color);
}
