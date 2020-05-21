#include "status_init_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../../widgets/updaters.h"
#include "../../gamedata/stat_names.h"
#include "status_widget_names.h"
#include "../common_widgets.h"

using namespace StatusWidgetNames;

void initStatCharDataWidget(const MenuInitEvent* event) {
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto charStatusWidget = createWidget(CHAR_DATA_WIDGET_NAME);

    DrawBoxParams boxParams = { 0, 0, 640, 480, 0.3f };

    auto boxWidget = createBoxWidget(boxParams, CHAR_DATA_BOX_NAME);
    addChildWidget(charStatusWidget, (Widget*)boxWidget, CHAR_DATA_BOX_NAME);

    drawPortraitParams portraitParams = { 17, 9, 0, 0.2f };
    auto portraitWidget = createPortraitWidget(portraitParams, PORTRAIT_WIDGET_NAME);
    addChildWidget(charStatusWidget, (Widget*)portraitWidget, PORTRAIT_WIDGET_NAME);

    drawHPBarParams hpBarParams = { 110, 17, 0, 0.2f };
    auto HPBarWidget = createHPBarWidget(hpBarParams, HPBAR_WIDGET_NAME);
    addChildWidget(charStatusWidget, (Widget*)HPBarWidget, HPBAR_WIDGET_NAME);

    DrawStaticGridParams staticGridParams = { primaryStatsUpdater, 17, 100, 1, 8, 0, 20, &allocateStatRow };
    auto charEquipGrid = createStaticGridWidget(staticGridParams, PRIMARY_STATS_GRID);
    addChildWidget(charStatusWidget, (Widget*)charEquipGrid, PRIMARY_STATS_GRID);

    auto commandView = createCommandViewWidget(STATUS_COMMAND_WIDGET_NAME, 0x90, 0x154, &statusCommandNameViewUpdater);
    addChildWidget(mainWidget, commandView, STATUS_COMMAND_WIDGET_NAME);

    addChildWidget(mainWidget, charStatusWidget, CHAR_DATA_WIDGET_NAME);
}

void statusCommandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    baseCommandNameViewUpdater(self, widget, flatIndex, STAT_MENU_ACTIVE_ACTOR);
}

Widget* allocateStatRow(const char* name, i32 xCoordinate, i32 yCoordinate){
    auto equipRow = createWidget(CHAR_DATA_WIDGET_NAME);
    moveWidget(equipRow, xCoordinate, yCoordinate);
    DrawTextParams textParams{ xCoordinate, yCoordinate, defaultString, COLOR_TEAL, 0.2f };
    auto textWidget = createTextWidget(textParams, "STAT");
    addChildWidget(equipRow, (Widget*)textWidget, "STAT");

    setTextParams(&textParams, xCoordinate + 60, yCoordinate, defaultString, COLOR_WHITE, 0.2f);
    textWidget = createTextWidget(textParams, "VAL");
    addChildWidget(equipRow, (Widget*)textWidget, "VAL");
    return equipRow;
}

void primaryStatUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != StaticGridWidgetClass()) {
        return;
    }
    std::vector<std::string> primaryStatOrder = {StatNames::STRENGTH, StatNames::DEXTERITY, StatNames::VITALITY, StatNames::MAGIC,
        StatNames::FOCUS, StatNames::SPIRIT, StatNames::AGILITY, StatNames::LUCK};

    PartyMemberState partyMember = gContext.party.getActivePartyMember(*STAT_MENU_ACTIVE_ACTOR);
    const auto& actorStats = partyMember.srPartyMember->playerStats;
    for (auto& statName : primaryStatOrder) {
        updateText(getChild(widget, "STAT"), gContext.stats.getElement(statName).displayName.str());
        updateText(getChild(widget, "VAL"), actorStats.at(statName).statValue);
    }
}


