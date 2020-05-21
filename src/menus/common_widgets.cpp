#include "common_widgets.h"
#include "menu.h"
#include "menu_interface.h"

/*Initializes the command view widget used */
Widget* createCommandViewWidget(const char* name, i32 x, i32 y, SRLISTUPDATERPROC commandNameViewUpdater, const char* menuName, u8 cursorIdx) {
    auto commandViewWidget = createWidget(name);

    DrawBoxParams boxParams = { x, y, 98, 0x78, 0.3f };
    auto boxWidget = createBoxWidget(boxParams, "BOX");
    addChildWidget(commandViewWidget, (Widget*)boxWidget, "BOX");

    Widget* gridWidget;
    for (u8 idx = 0; idx < 3; idx++) {
        auto actorCommandsWidget = createWidget(std::to_string(idx));
        if (menuName) {
            DrawCursorGridParams gridParams = { menuName, cursorIdx, commandNameViewUpdater, x + 10, y + 11, &allocateCommandRow, 0, idx, true };
            auto gridWidget = (Widget*)createGridWidget(gridParams, name);
        }
        else {
            DrawStaticGridParams gridParams = { commandNameViewUpdater, x, y, 4, 4, 50, 20, &allocateCommandRow, true };
            auto gridWiget = (Widget*)createStaticGridWidget(gridParams, name);
        }
        addChildWidget(actorCommandsWidget, gridWidget, "GRID");
        auto boxWidget = createBoxWidget(boxParams, "BOX");
        addChildWidget(actorCommandsWidget, boxWidget, "BOX");
        addChildWidget(commandViewWidget, (Widget*)actorCommandsWidget, std::to_string(idx));
    }
    return commandViewWidget;
}

void resizeCommandBox(u8 actorID, Widget* cmdWidget, Cursor* commandChoiceCursor) {
    auto boxWidget = getChild(getChild(cmdWidget, std::to_string(actorID)), "BOX");
    if (commandChoiceCursor) {
        commandChoiceCursor->context.maxColumnBound = gContext.party.getActivePartyMember(actorID).gamePartyMember->commandColumns;
        commandChoiceCursor->context.viewColumnBound = gContext.party.getActivePartyMember(actorID).gamePartyMember->commandColumns;
        resizeBox(boxWidget, 160, 340, 120 * commandChoiceCursor->context.maxColumnBound, 120);
    }
    resizeBox(boxWidget, 160, 340, 120 * gContext.party.getActivePartyMember(actorID).gamePartyMember->commandColumns, 120);
}

void updateCommandsActor(Widget* cmdWidget, u8 actorIdx, Menu* menu, u16 cursorState) {
    std::vector<std::string> names = { "0", "1", "2" };
    for (u8 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (partyIdx == actorIdx) {
            enableWidget(getChild(cmdWidget, names[partyIdx]));
            if (menu) {
                setActiveCursorIndex(menu, cursorState, actorIdx);
            }
            continue;
        }
        disableWidget(getChild(cmdWidget, names[partyIdx]));
    }
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

/*Temporary function until we also provide infrastructure for extending the number of commands*/
void baseCommandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex, u32* updatingActor) {
    if (self->collectionType != GridWidgetClass() || self->collectionType != StaticGridWidgetClass()) {
        return;
    }

    const auto& commands = gContext.party.getActivePartyMember(*updatingActor).gamePartyMember->enabledCommandArray;
    if (commands[flatIndex].commandID == 0xFF) {
        disableWidget(getChild(widget, std::string("ARW")));
        disableWidget(getChild(widget, std::string("TXT")));
        return;
    }
    enableWidget(getChild(widget, std::string("TXT")));
    updateText(getChild(widget, std::string("TXT")), gContext.commands.getResource(commands[flatIndex].commandID).commandName.str());
    auto color = COLOR_WHITE;
    if (commands[flatIndex].commandFlags & 2) {
        color = COLOR_GRAY;
    }
    if (commands[flatIndex].commandID == CMD_LIMIT) {
        color = COLOR_TEAL;
    }
    updateTextColor(widget, color);

    if (commands[flatIndex].allCount) {
        enableWidget(getChild(widget, std::string("ARW")));
        return;
    }
    disableWidget(getChild(widget, std::string("ARW")));
}

void createActionViewWidget(const char* name, i32 x, i32 y, SRLISTUPDATERPROC actionViewUpdater, const char* menuName, u16 cursorIdx) {
    auto actionView = createWidget(name);

    DrawBoxParams boxParams = { 0, 340, 640, 140,  0.4f };

    std::vector<std::string> names = { "0", "1", "2" };
    for (u8 idx = 0; idx < names.size(); idx++) {
        auto actorActionView = createWidget(std::to_string(idx));
        DrawCursorGridParams gridParams = { menuName, cursorIdx, &actionView, 42, 360, allocateActionRow, idx };
        auto gridWidget = createGridWidget(gridParams, names[idx]);
        addChildWidget(actorActionView, (Widget*)gridWidget, names[idx]);
        auto boxWidget = createBoxWidget(boxParams, "BOX");
        addChildWidget(actorActionView, (Widget*)boxWidget, "BOX");
    }
    return actionView
}

Widget* allocateActionRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto actionWidget = createWidget(name);
    moveWidget(actionWidget, xCoordinate, yCoordinate);
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.4f };
    addChildWidget(actionWidget, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
    return actionWidget;
}

void updateActionsActor(Widget* cmdWidget, u8 actorIdx, Menu* menu, u16 cursorState) {
    for (u8 partyIdx = 0; partyIdx < 3; partyIdx++) {
        if (partyIdx == actorIdx) {
            enableWidget(cmdWidget, std::to_string(actorIdx)));
            if (menu) {
                setActiveCursorIndex(menu, cursorState, actorIdx);
            }
            continue;
        }
        disableWidget(cmdWidget, std::to_string(actorIdx)));
    }
}
