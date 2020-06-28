#include "battle_limit_init_callbacks.h"
#include "../../common_widgets.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initBattleLimitViewWidget(const MenuInitEvent* event) {
    auto mainWidget = event->menu->menuWidget;
    auto battleLimitView = createActionViewWidget(BATTLE_LIMIT_WIDGET_NAME.c_str(), 0, 340, battleLimitNameViewUpdater, BATTLE_MENU_NAME.c_str(), BATTLE_LIMIT_STATE);
    addChildWidget(mainWidget, battleLimitView, BATTLE_LIMIT_WIDGET_NAME);
}


void battleLimitNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    if (!gContext.battleActors.isActorSummon(*BATTLE_ACTIVE_ACTOR_ID)) {
        auto characterLimits = gContext.party.getActivePartyCharacter(*BATTLE_ACTIVE_ACTOR_ID).limitBreaks;
        if (flatIndex >= characterLimits.size()) {
            disableWidget(getChild(widget, std::string("TXT")));
            return;
        }
        if (!(gContext.party.getActivePartyCharacter(*BATTLE_ACTIVE_ACTOR_ID).gameCharacter->learned_limits & (1 << flatIndex))) {
            disableWidget(getChild(widget, std::string("TXT")));
            return;
        }
        auto& limitAction = getCommandAction(CMD_LIMIT, characterLimits[flatIndex]);
        enableWidget(getChild(widget, std::string("TXT")));
        updateText(getChild(widget, std::string("TXT")), limitAction.name.str());
        updateTextColor(getChild(widget, std::string("TXT")), COLOR_WHITE);
    }
    else {
        auto activeSummonIdx = gContext.battleActors.getActiveSummonIdx();
        auto summonLimits = gContext.summons.getResource(activeSummonIdx).limitBreaks;
        if (activeSummonIdx == 0xFFFF) {
            srLogWrite("ERROR: INVALID SUMMON INDEX BUT SUMMON IS ACTIVE");
            return;
        }
        if (flatIndex >= summonLimits.size()) {
            disableWidget(getChild(widget, std::string("TXT")));
            return;
        }
        auto& limitAction = getCommandAction(CMD_LIMIT, summonLimits[flatIndex]);
        enableWidget(getChild(widget, std::string("TXT")));
        updateText(getChild(widget, std::string("TXT")), limitAction.name.str());
        updateTextColor(getChild(widget, std::string("TXT")), COLOR_WHITE);
    }

}
