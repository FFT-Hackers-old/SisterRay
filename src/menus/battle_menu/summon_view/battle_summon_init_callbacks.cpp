#include "battle_summon_init_callbacks.h"
#include "../../common_widgets.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initBattleSummonViewWidget(const MenuInitEvent* event) {
    auto mainWidget = event->menu->menuWidget;
    auto battleSummonView = createActionViewWidget(BATTLE_SUMMON_WIDGET_NAME.c_str(), 0, 340, battleSummonNameViewUpdater, BATTLE_MENU_NAME.c_str(), BATTLE_SUMMON_STATE);
    addChildWidget(mainWidget, battleSummonView, BATTLE_SUMMON_WIDGET_NAME);
}


void battleSummonNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto summons = getActivePartyMember(*BATTLE_ACTIVE_ACTOR_ID).srPartyMember->actorSummons;
    if (summons[flatIndex].magicIndex == 0xFF) {
        disableWidget(getChild(widget, std::string("TXT")));
        return;
    }
    enableWidget(getChild(widget, std::string("TXT")));
    updateText(getChild(widget, std::string("TXT")), getCommandAction(CMD_SUMMON, summons[flatIndex].magicIndex).attackName.str());
    updateTextColor(getChild(widget, std::string("TXT")), COLOR_WHITE);
}
