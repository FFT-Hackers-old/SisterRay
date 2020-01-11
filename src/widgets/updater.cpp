#include "updaters.h"
#include "../impl.h"
#include "../inventories/inventory_utils.h"
#include "../menus/battle_menu/battle_menu_utils.h"

void battleSpellNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto magics = getSrPartyMember(*BATTLE_ACTIVE_ACTOR_ID).srPartyMember->actorMagics;
    if (magics[flatIndex].magicIndex == 0xFF) {
        disableWidget(getChild(widget, std::string("ARW")));
        disableWidget(getChild(widget, std::string("TXT")));
        return;
    }
    enableWidget(getChild(widget, std::string("TXT")));
    updateText(getChild(widget, std::string("TXT")), getCommandAction(CMD_MAGIC, magics[flatIndex].magicIndex).attackName.str());
    updateTextColor(widget, COLOR_WHITE);
    if (magics[flatIndex].allCount) {
        enableWidget(getChild(widget, std::string("ARW")));
        return;
    }
    disableWidget(getChild(widget, std::string("ARW")));
}

void battleSummonNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto summons = getSrPartyMember(*BATTLE_ACTIVE_ACTOR_ID).srPartyMember->actorSummons;
    if (summons[flatIndex].magicIndex == 0xFF) {
        disableWidget(getChild(widget, std::string("TXT")));
        return;
    }
    enableWidget(getChild(widget, std::string("TXT")));
    updateText(getChild(widget, std::string("TXT")), getCommandAction(CMD_SUMMON, summons[flatIndex].magicIndex).attackName.str());
    updateTextColor(getChild(widget, std::string("TXT")), COLOR_WHITE);
}

void battleEskillNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto eSkills = getSrPartyMember(*BATTLE_ACTIVE_ACTOR_ID).srPartyMember->actorEnemySkills;
    if (eSkills[flatIndex].magicIndex == 0xFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    updateText(widget, getCommandAction(CMD_ENEMY_SKILL, eSkills[flatIndex].magicIndex).attackName.str());
    updateTextColor(widget, COLOR_WHITE);
}

void battleInventoryRowUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto itemID = gContext.battleInventory->getResource(flatIndex).item_id;
    if (itemID == 0xFFFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    auto textColor = (isUsableInBattle(itemID)) ? COLOR_WHITE : COLOR_GRAY;
    updateText(getChild(widget, std::string("TXT")), getNameFromItemID(itemID));
    updateTextColor(getChild(widget, std::string("TXT")), textColor);
    updateNumber(getChild(widget, std::string("AMT")), gContext.battleInventory->getResource(itemID).quantity);
    updateNumberColor(getChild(widget, std::string("AMT")), textColor);
    updateItemIcon(getChild(widget, std::string("ICN")), gContext.itemTypeData.getResource(itemID).itemIconType);
}
