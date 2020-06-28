#include "battle_technique_init_callbacks.h"
#include "../../common_widgets.h"
#include "../../../impl.h"

using namespace BattleMenuWidgetNames;

void initBattleTechniqueViewWidget(const MenuInitEvent* event) {
    auto mainWidget = event->menu->menuWidget;
    auto battleTechniqueView = createActionViewWidget(BATTLE_TECHNIQUE_WIDGET_NAME.c_str(), 0, 340, battleTechniqueNameViewUpdater, BATTLE_MENU_NAME.c_str(), BATTLE_TECHNIQUE_STATE);
    addChildWidget(mainWidget, battleTechniqueView, BATTLE_TECHNIQUE_WIDGET_NAME);
}


void battleTechniqueNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    techniqueViewUpdater(self, widget, flatIndex, BATTLE_ACTIVE_ACTOR_ID);
}
