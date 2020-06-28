#include "status_init_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../../widgets/updaters.h"
#include "../../gamedata/stat_names.h"
#include "status_widget_names.h"
#include "../common_widgets.h"
#include "../equip_menu/equip_init_callbacks.h"

using namespace StatusWidgetNames;

void initStatCharDataWidget(const MenuInitEvent* event) {
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto charStatusWidget = createWidget(CHAR_DATA_WIDGET_NAME);

    DrawBoxParams boxParams = { 0, 0, 640, 480, 0.5f };

    auto boxWidget = createBoxWidget(boxParams, CHAR_DATA_BOX_NAME);
    addChildWidget(charStatusWidget, (Widget*)boxWidget, CHAR_DATA_BOX_NAME);

    drawPortraitParams portraitParams = { 17, 9, 0, 0.2f };
    auto portraitWidget = createPortraitWidget(portraitParams, PORTRAIT_WIDGET_NAME);
    addChildWidget(charStatusWidget, (Widget*)portraitWidget, PORTRAIT_WIDGET_NAME);

    drawHPBarParams hpBarParams = { 110, 17, 0, 0.2f };
    auto HPBarWidget = createHPBarWidget(hpBarParams, HPBAR_WIDGET_NAME);
    addChildWidget(charStatusWidget, (Widget*)HPBarWidget, HPBAR_WIDGET_NAME);

    addChildWidget(mainWidget, charStatusWidget, CHAR_DATA_WIDGET_NAME);
}

void initPrimaryStatsView(const MenuInitEvent* event) {
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto charStatusWidget = createWidget(PRIM_STATS_WIDGET_NAME);
    DrawStaticGridParams staticGridParams = { &primaryStatUpdater, 38, 110, 1, 9, 0, 20, &allocateStatRow };
    auto primaryStats = createStaticGridWidget(staticGridParams, PRIMARY_STATS_GRID);
    addChildWidget(charStatusWidget, (Widget*)primaryStats, PRIMARY_STATS_GRID);

    DrawStaticGridParams equipStatsParams = { &equipmentStatUpdater, 420, 150, 1, 5, 0, 20, &allocateStatRow };
    auto equipStats = createStaticGridWidget(equipStatsParams, EQUIPMENT_STATS);
    addChildWidget(charStatusWidget, (Widget*)equipStats, EQUIPMENT_STATS);

    auto commandView = createCommandViewWidget(STATUS_COMMAND_WIDGET_NAME.c_str(), 0xAF, 150, &statusCommandNameViewUpdater);
    addChildWidget(charStatusWidget, commandView, STATUS_COMMAND_WIDGET_NAME);

    DrawStaticGridParams equipParams = { &statEquipRowUpdater, 420, 60, 1, 3, 0, 26, &allocateEquipRow };
    auto charEquipGrid = createStaticGridWidget(equipParams, EQUIPMENT_GRID);
    addChildWidget(charStatusWidget, (Widget*)charEquipGrid, EQUIPMENT_GRID);

    addChildWidget(mainWidget, charStatusWidget, PRIM_STATS_WIDGET_NAME);
}


void initStatCombatDataWidget(const MenuInitEvent* event) {
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;
    auto combatStatsWidget = createWidget(COMBAT_STATS_WIDGET);

    DrawBoxParams boxParams = { 0, 110, 640, 480, 0.3f };
    auto boxWidget = createBoxWidget(boxParams, "BOX");
    addChildWidget(combatStatsWidget, (Widget*)boxWidget, "BOX");

    DrawBoxParams helpBoxParams = { 0, 360, 640, 120, 0.2f };
    auto helpBoxWidget = createBoxWidget(helpBoxParams, "HELPBOX");
    addChildWidget(combatStatsWidget, (Widget*)helpBoxWidget, "HELPBOX");

    DrawStaticGridParams combatStatsParams = { &combatStatUpdater, 60, 120, 2, 11, 230, 20, &allocateStatRow, true };
    auto combatStats = createStaticGridWidget(combatStatsParams, COMBAT_STATS_GRID);
    addChildWidget(combatStatsWidget, (Widget*)combatStats, COMBAT_STATS_GRID);
    addChildWidget(mainWidget, combatStatsWidget, COMBAT_STATS_WIDGET);
}

void statusCommandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    commandNameViewUpdater(self, widget, flatIndex, STAT_MENU_ACTIVE_ACTOR);
}

Widget* allocateStatRow(const char* name, i32 xCoordinate, i32 yCoordinate){
    auto equipRow = createWidget(CHAR_DATA_WIDGET_NAME);
    moveWidget(equipRow, xCoordinate, yCoordinate);
    DrawTextParams textParams{ xCoordinate, yCoordinate, defaultString, COLOR_TEAL, 0.2f };
    auto textWidget = createTextWidget(textParams, "STAT");
    addChildWidget(equipRow, (Widget*)textWidget, "STAT");

    DrawNumberParams numberParams = { xCoordinate + 78, yCoordinate + 5, 0, 4, COLOR_WHITE, 0.2f };
    auto numberWidget = createNumberWidget(numberParams, "VAL");
    addChildWidget(equipRow, (Widget*)numberWidget, "VAL");
    return equipRow;
}

void primaryStatUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != StaticGridWidgetClass()) {
        return;
    }
    std::vector<std::string> primaryStatOrder = {StatNames::STRENGTH, StatNames::VITALITY, StatNames::DEXTERITY, StatNames::AGILITY, StatNames::MAGIC,
        StatNames::SPIRIT, StatNames::FOCUS, StatNames::INSIGHT, StatNames::LUCK};

    PartyMemberState partyMember = gContext.party.getActivePartyMember(*STAT_MENU_ACTIVE_ACTOR);
    const auto& actorStats = partyMember.srPartyMember->stats;
    updateText(getChild(widget, "STAT"), gContext.stats.getElement(primaryStatOrder[flatIndex]).displayName.str());
    updateNumber(getChild(widget, "VAL"), actorStats.at(primaryStatOrder[flatIndex]).statValue);
}


void equipmentStatUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != StaticGridWidgetClass()) {
        return;
    }
    std::vector<std::string> equipmentStatOrder = { StatNames::WEAPON_ATTACK, StatNames::WEAPON_ACCURACY, StatNames::WEAPON_MAGIC, StatNames::ARMOR_DEFENSE, StatNames::ARMOR_MDEFENSE };

    PartyMemberState partyMember = gContext.party.getActivePartyMember(*STAT_MENU_ACTIVE_ACTOR);
    const auto& actorStats = partyMember.srPartyMember->stats;
    updateText(getChild(widget, "STAT"), gContext.stats.getElement(equipmentStatOrder[flatIndex]).displayName.str());
    updateNumber(getChild(widget, "VAL"), actorStats.at(equipmentStatOrder[flatIndex]).statValue);
    moveWidget(getChild(widget, "VAL"), widget->xCoordinate + 105, widget->yCoordinate + 5);
}

void statEquipRowUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != StaticGridWidgetClass()) {
        return;
    }
    auto& gearSlot = gContext.party.getActivePartyCharacter(*STAT_MENU_ACTIVE_ACTOR).equipment.at(flatIndex);
    updateText(getChild(widget, "TYPE"), gearSlot.slotName.str());
    updateText(getChild(widget, "EQP"), gearSlot.equipped.gearName.str());
}

void combatStatUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != StaticGridWidgetClass()) {
        return;
    }

    std::vector<std::string> statOrder = {
        StatNames::ATTACK_POWER,
        StatNames::ATTACK_SPEED,
        StatNames::ACCURACY,
        StatNames::CRIT_RATE,
        StatNames::CRIT_POWER,
        StatNames::STAGGER_POWER,
        StatNames::EVASION_PEN,
        StatNames::PARRY_PEN,
        StatNames::BLOCK_PEN,
        StatNames::ARMOR_PEN,
        StatNames::DEFENSE_PEN,

        StatNames::DEFENSE,
        StatNames::ARMOR,
        StatNames::BLOCK,
        StatNames::BLOCK_RATE,
        StatNames::PARRY_RATE,
        StatNames::EVADE,
        StatNames::STAGGER_RES,
        StatNames::CRIT_RES,
        StatNames::WOUND_RES,
        StatNames::PHYS_DEBUFF_RES,
        StatNames::PHYS_BUFF_AFF
    };

    PartyMemberState partyMember = gContext.party.getActivePartyMember(*STAT_MENU_ACTIVE_ACTOR);
    const auto& actorStats = partyMember.srPartyMember->stats;
    updateText(getChild(widget, "STAT"), gContext.stats.getElement(statOrder[flatIndex]).displayName.str());
    updateNumber(getChild(widget, "VAL"), actorStats.at(statOrder[flatIndex]).statValue);
    moveWidget(getChild(widget, "VAL"), widget->xCoordinate + 105, widget->yCoordinate + 5);
}
