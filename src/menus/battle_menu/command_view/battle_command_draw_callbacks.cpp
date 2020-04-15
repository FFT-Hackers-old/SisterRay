#include "battle_command_draw_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void drawBattleCommandViewWidget(const MenuDrawEvent* event) {
    srLogWrite("UPDATING COMMAND VIEW WIDGET");
    auto menuWidget = event->menu->menuWidget;
    Cursor* commandChoiceCursor = getStateCursor(event->menu, BATTLE_CMD_STATE, *BATTLE_ACTIVE_ACTOR_ID);
    commandChoiceCursor->context.maxColumnBound = gContext.party.getActivePartyMember(*BATTLE_ACTIVE_ACTOR_ID).gamePartyMember->commandColumns;
    commandChoiceCursor->context.viewColumnBound = gContext.party.getActivePartyMember(*BATTLE_ACTIVE_ACTOR_ID).gamePartyMember->commandColumns;
    srLogWrite("VIEW COLUMN BOUND: %i", commandChoiceCursor->context.viewColumnBound);
    resizeBox(getChild(menuWidget, CMD_GRID_BOX), 135, 340, 88 * commandChoiceCursor->context.maxColumnBound, 150);

    if (event->menuState != BATTLE_CMD_STATE || event->menuState != BATTLE_MOVE_STATE) {
        disableWidget(getChild(menuWidget, BATTLE_COMMAND_WIDGET_NAME));
        return;
    }
    updateCommandsActive(*BATTLE_ACTIVE_ACTOR_ID, 0);

    enableWidget(getChild(menuWidget, BATTLE_COMMAND_WIDGET_NAME));
    std::vector<std::string> names = { PARTY_1_CMD_GRID, PARTY_2_CMD_GRID, PARTY_3_CMD_GRID };
    for (i32 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID) {
            enableWidget(getChild(getChild(menuWidget, BATTLE_COMMAND_WIDGET_NAME), names[partyIdx]));
            setActiveCursorIndex(event->menu, BATTLE_CMD_STATE, *BATTLE_ACTIVE_ACTOR_ID);
            continue;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_COMMAND_WIDGET_NAME), names[partyIdx]));
    }
}
