#include "common_widgets.h"
#include "menu.h"
#include "menu_interface.h"
#include "../impl.h"

/*Initializes the command view widget used */
Widget* createCommandViewWidget(const char* name, i32 x, i32 y, SRLISTUPDATERPROC commandNameViewUpdater, const char* menuName, u8 cursorIdx) {
    auto commandViewWidget = createWidget(name);
    moveWidget(commandViewWidget, x, y);
    DrawBoxParams boxParams = { x, y, 98, 0x78, 0.2f };

    Widget* gridWidget;
    for (u8 idx = 0; idx < 3; idx++) {
        auto actorCommandsWidget = createWidget(std::to_string(idx));
        if (menuName) {
            DrawCursorGridParams gridParams = { menuName, cursorIdx, commandNameViewUpdater, x + 20, y + 11, &allocateCommandRow, idx, true };
            gridWidget = (Widget*)createGridWidget(gridParams, name);
        }
        else {
            DrawStaticGridParams gridParams = { commandNameViewUpdater, x + 20, y + 11, 4, 4, 50, 26, &allocateCommandRow, true };
            gridWidget = (Widget*)createStaticGridWidget(gridParams, name);
        }
        addChildWidget(actorCommandsWidget, gridWidget, "GRID");
        auto boxWidget = createBoxWidget(boxParams, "BOX");
        addChildWidget(actorCommandsWidget, (Widget*)boxWidget, "BOX");
        addChildWidget(commandViewWidget, (Widget*)actorCommandsWidget, std::to_string(idx));
    }
    return commandViewWidget;
}

void resizeCommandBox(u8 actorID, Widget* cmdWidget, Cursor* commandChoiceCursor) {
    auto boxWidget = getChild(getChild(cmdWidget, std::to_string(actorID)), "BOX");
    if (commandChoiceCursor) {
        commandChoiceCursor->context.maxColumnBound = gContext.party.getActivePartyMember(actorID).gamePartyMember->commandColumns;
        commandChoiceCursor->context.viewColumnBound = gContext.party.getActivePartyMember(actorID).gamePartyMember->commandColumns;
        resizeBox(boxWidget, 120 * commandChoiceCursor->context.maxColumnBound, 120);
    }
    resizeBox(boxWidget, 120 * gContext.party.getActivePartyMember(actorID).gamePartyMember->commandColumns, 120);
}

void updateCommandsActor(Widget* cmdWidget, u8 actorIdx, Menu* menu, u16 cursorState) {
    for (u8 partyIdx = 0; partyIdx < 3; partyIdx++) {
        if (partyIdx == actorIdx) {
            enableWidget(getChild(cmdWidget, std::to_string(partyIdx)));
            if (menu) {
                setActiveCursorIndex(menu, cursorState, actorIdx);
            }
            continue;
        }
        disableWidget(getChild(cmdWidget, std::to_string(partyIdx)));
    }
}

Widget* allocateCommandRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto cmdWidget = createWidget(name);
    moveWidget(cmdWidget, xCoordinate, yCoordinate);
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.19f };
    addChildWidget(cmdWidget, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
    DrawGameAssetParams gameAssetParams = AllArrow(xCoordinate + 4, yCoordinate, 0.19f);
    addChildWidget(cmdWidget, (Widget*)createGameAssetWidget(gameAssetParams, std::string("ARW")), std::string("ARW"));
    return cmdWidget;
}

void baseCommandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex, u8* updatingActor) {
    const auto& commands = gContext.party.getActivePartyMember(*updatingActor).gamePartyMember->enabledCommandArray;
    baseCommandNameViewUpdater(self, widget, flatIndex, commands);
}

void baseCommandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex, u32* updatingActor) {
    srLogWrite("Animating Commands for Actor: %x, ptr: %x", *updatingActor, updatingActor);
    const auto& commands = gContext.party.getActivePartyMember(*updatingActor).gamePartyMember->enabledCommandArray;
    baseCommandNameViewUpdater(self, widget, flatIndex, commands);
}

void baseCommandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex, const EnabledCommand (&commands)[16]) {
    if (self->collectionType != GridWidgetClass() && self->collectionType != StaticGridWidgetClass()) {
        return;
    }
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

Widget* createActionViewWidget(const char* name, i32 x, i32 y, SRLISTUPDATERPROC actionViewUpdater, const char* menuName, u16 cursorIdx) {
    auto actionView = createWidget(name);

    DrawBoxParams boxParams = { 0, 340, 640, 140,  0.4f };

    for (u8 idx = 0; idx < 3; idx++) {
        auto actorActionView = createWidget(std::to_string(idx));
        DrawCursorGridParams gridParams = { menuName, cursorIdx, actionViewUpdater, 42, 360, &allocateActionRow, idx };
        auto gridWidget = createGridWidget(gridParams, "GRID");
        addChildWidget(actorActionView, (Widget*)gridWidget, "GRID");
        auto boxWidget = createBoxWidget(boxParams, "BOX");
        addChildWidget(actorActionView, (Widget*)boxWidget, "BOX");
        addChildWidget(actionView, actorActionView, std::to_string(idx));
    }
    return actionView;
}

Widget* allocateActionRow(const char* name, i32 xCoordinate, i32 yCoordinate) {
    auto actionWidget = createWidget(name);
    moveWidget(actionWidget, xCoordinate, yCoordinate);
    DrawTextParams textParams = { xCoordinate, yCoordinate, getDefaultString(), COLOR_WHITE, 0.4f };
    addChildWidget(actionWidget, (Widget*)createTextWidget(textParams, std::string("TXT")), std::string("TXT"));
    DrawGameAssetParams gameAssetParams = AllArrow(xCoordinate + 4, yCoordinate, 0.4f);
    addChildWidget(actionWidget, (Widget*)createGameAssetWidget(gameAssetParams, std::string("ARW")), std::string("ARW"));
    return actionWidget;
}

void updateActionsActor(Widget* cmdWidget, u8 actorIdx, Menu* menu, u16 cursorState) {
    for (u8 partyIdx = 0; partyIdx < 3; partyIdx++) {
        if (partyIdx == actorIdx) {
            srLogWrite("SETTING ACTIVE MAGIC CURSOR to %i", partyIdx);
            enableWidget(getChild(cmdWidget, std::to_string(partyIdx)));
            if (menu) {
                setActiveCursorIndex(menu, cursorState, actorIdx);
            }
            continue;
        }
        disableWidget(getChild(cmdWidget, std::to_string(partyIdx)));
    }
}
