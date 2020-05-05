#include "battle_command_draw_callbacks.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void drawBattleCommandViewWidget(const MenuDrawEvent* event) {
    u16* G_LIMIT_ACTORS_MASK = (u16*)0x9A889E;
    auto menuWidget = event->menu->menuWidget;
    if ((event->menuState != BATTLE_CMD_STATE) && (event->menuState != BATTLE_MOVE_STATE)) {
        disableWidget(getChild(menuWidget, BATTLE_COMMAND_WIDGET_NAME));
        return;
    }
    updateCommandsActive(*BATTLE_ACTIVE_ACTOR_ID, 0);

    if (!gContext.party.isSlotEnabled(*BATTLE_ACTIVE_ACTOR_ID)) {
        setMenuState(event->menu, BATTLE_INACTIVE);
        return;
    }

    enableWidget(getChild(menuWidget, BATTLE_COMMAND_WIDGET_NAME));
    std::vector<std::string> names = { PARTY_1_CMD_GRID, PARTY_2_CMD_GRID, PARTY_3_CMD_GRID };
    for (u8 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID) {
            enableWidget(getChild(getChild(menuWidget, BATTLE_COMMAND_WIDGET_NAME), names[partyIdx]));
            setActiveCursorIndex(event->menu, BATTLE_CMD_STATE, *BATTLE_ACTIVE_ACTOR_ID);
            continue;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_COMMAND_WIDGET_NAME), names[partyIdx]));
    }
    Cursor* commandChoiceCursor = getStateCursor(event->menu, BATTLE_CMD_STATE, *BATTLE_ACTIVE_ACTOR_ID);
    commandChoiceCursor->context.maxColumnBound = gContext.party.getActivePartyMember(*BATTLE_ACTIVE_ACTOR_ID).gamePartyMember->commandColumns;
    commandChoiceCursor->context.viewColumnBound = gContext.party.getActivePartyMember(*BATTLE_ACTIVE_ACTOR_ID).gamePartyMember->commandColumns;
    resizeBox(getChild(getChild(menuWidget, BATTLE_COMMAND_WIDGET_NAME), CMD_GRID_BOX), 160, 340, 120 * commandChoiceCursor->context.maxColumnBound, 120);

    auto& actorState = gContext.battleActors.getActiveBattleActor(*BATTLE_ACTIVE_ACTOR_ID);
    auto& partyState = gContext.party.getActivePartyCharacter(*BATTLE_ACTIVE_ACTOR_ID);
    if (*G_LIMIT_ACTORS_MASK & (1 << *BATTLE_ACTIVE_ACTOR_ID)) {
        setToggleCommand(*BATTLE_ACTIVE_ACTOR_ID, 0, BASE_PREFIX, 0x14);
        gContext.party.getActivePartyMember(*BATTLE_ACTIVE_ACTOR_ID).srPartyMember->isLimitActive = true;
        *G_LIMIT_ACTORS_MASK &= (~((1 << *BATTLE_ACTIVE_ACTOR_ID)));
    }
}
