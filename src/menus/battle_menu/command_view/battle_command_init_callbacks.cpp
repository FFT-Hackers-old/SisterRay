#include "battle_command_init_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initBattleCommandViewWidget(const MenuInitEvent* event) {
    CursorGridWidget* gridWidget;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto battleCommandView = createWidget(BATTLE_COMMAND_WIDGET_NAME);
    srLogWrite("CALLING BATTLE COMMAND INIT HANDLER");

    boxParams = {
        0x90,
        0x154,
        0x6E,
        0x70,
        0.502f
    };
    boxWidget = createBoxWidget(boxParams, CMD_GRID_BOX);
    addChildWidget(battleCommandView, (Widget*)boxWidget, CMD_GRID_BOX);

    std::vector<std::string> names = { PARTY_1_CMD_GRID, PARTY_2_CMD_GRID, PARTY_3_CMD_GRID };
    for (auto idx = 0; idx < names.size(); idx++) {
        auto cmdItemChoice = getStateCursor(event->menu, idx);

        drawGridParams gridParams = { BATTLE_MENU_NAME.c_str(), BATTLE_CMD_STATE, battleCommandNameViewUpdater, 156, 350, allocateCommandRow, idx, true };
        gridWidget = createGridWidget(gridParams, names[idx]);
        addChildWidget(battleCommandView, (Widget*)gridWidget, names[idx]);
    }

    addChildWidget(mainWidget, battleCommandView, BATTLE_COMMAND_WIDGET_NAME);
}

Widget* allocateCommandRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto cmdWidget = createWidget(name);
    moveWidget(cmdWidget, xCoordinate, yCoordinate);
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.501f };
    addChildWidget(cmdWidget, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
    DrawGameAssetParams gameAssetParams = AllArrow(xCoordinate + 4, yCoordinate, 0.501f);
    addChildWidget(cmdWidget, (Widget*)createGameAssetWidget(gameAssetParams, std::string("ARW")), std::string("ARW"));
    return cmdWidget;
}

void battleCommandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    const auto& commands = gContext.party.getActivePartyMember(*BATTLE_ACTIVE_ACTOR_ID).gamePartyMember->enabledCommandArray;
    if (commands[flatIndex].commandID == 0xFF) {
        disableWidget(getChild(widget, std::string("ARW")));
        disableWidget(getChild(widget, std::string("TXT")));
        return;
    }
    enableWidget(getChild(widget, std::string("TXT")));
    updateText(getChild(widget, std::string("TXT")), gContext.commands.getResource(commands[flatIndex].commandID).commandName.str());
    auto color = COLOR_WHITE;
    if (commands[flatIndex].commandFlags & 2) {
        auto color = COLOR_GRAY;
    }
    updateTextColor(widget, color);

    if (commands[flatIndex].allCount) {
        enableWidget(getChild(widget, std::string("ARW")));
        return;
    }
    disableWidget(getChild(widget, std::string("ARW")));
}
