#include "updaters.h"
#include "../impl.h"
#include "../inventories/inventory_utils.h"
#include "../menus/battle_menu/battle_menu_utils.h"

void gearViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }

    auto typedPtr = (CursorGridWidget*)self;
    auto gearType = gContext.gearViewData.getItemType();
    auto relativeItemID = gContext.gearViewData.get_resource(flatIndex).relative_item_id;
    const char* name = getNameFromRelativeID(relativeItemID, gearType);
    updateText(widget, name);

}

/*Update the current view of the inventory based on the relevant cursor*/
void inventoryViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }

    auto typedPtr = (CursorGridWidget*)self;
    auto itemID = gContext.inventory->get_resource(flatIndex).item_id;
    if (itemID != 0xFFFF) {
        enableWidget(widget);
        auto textColor = usableInInventoryMenu(itemID) ? COLOR_GRAY: COLOR_WHITE;
        const char* name = getNameFromItemID(itemID);
        updateText(widget, name);
        updateTextColor(widget, textColor);
    }
    else {
        disableWidget(widget);
    }
}

void inventoryViewQuantityUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }

    auto typedPtr = (CursorGridWidget*)self;
    auto itemID = gContext.inventory->get_resource(flatIndex).item_id;
    if (gContext.inventory->get_resource(flatIndex).item_id != 0xFFFF) {
        enableWidget(widget);

        auto numberColor = usableInInventoryMenu(itemID) ? COLOR_GRAY : COLOR_WHITE;
        auto itemQuantity = gContext.inventory->get_resource(flatIndex).quantity;
        updateNumber(widget, itemQuantity);
        updateNumberColor(widget, numberColor);
    }
    else {
        disableWidget(widget);
    }
}

void keyItemViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }

    auto typedPtr = (CursorGridWidget*)self;
    auto keyItemIndex = (KEY_ITEMS_INVENTORY_PTR)[(u8)(flatIndex)];
    if (keyItemIndex != 0xFFFF) {
        enableWidget(widget);
        const char* name = gContext.gameStrings.key_item_names.get_string(flatIndex);
        updateText(widget, name);
        updateTextColor(widget, COLOR_WHITE);
    }
    else {
        disableWidget(widget);
    }
}

void materiaNameViewUpdater(CollectionWidget* self, Widget*widget, u16 flatIndex) {
    srLogWrite("calling materia widget");
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    srLogWrite("updating materia widget");
    auto typedPtr = (CursorGridWidget*)self;
    auto materiaID = gContext.materiaInventory->get_resource(flatIndex).item_id;
    if (materiaID != 0xFFFF) {
        enableWidget(widget);
        const char* name = gContext.gameStrings.materia_names.get_string(materiaID);
        srLogWrite("displaying materia name %s", name);
        updateText(widget, name);
        updateTextColor(widget, COLOR_WHITE);
    }
    else {
        disableWidget(widget);
    }
}

/*This will update a "Sphere Widget once I figure out how to make one*/
void materiaSphereViewUpdater(CollectionWidget* self, Widget*widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto materiaID = gContext.materiaInventory->get_resource(flatIndex).item_id;
    srLogWrite("Displaying sphere for materia with ID %i", materiaID);
    if (materiaID != 0xFFFF) {
        enableWidget(widget);
        transformAsset(widget, 128, 32, 16, 16);
        auto materiaAssetType = getMateriaColorType(materiaID);
        updateAssetType(widget, materiaAssetType);
    }
    else {
        disableWidget(widget);
    }
}

/*Temporary function until we also provide infrastructure for extending the number of commands*/
void commandNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    srLogWrite("ATTEMPTING TO DISPLAY COMMAND NAME at index %i", flatIndex);
    auto typedPtr = (CursorGridWidget*)self;
    auto commands = PARTY_STRUCT_ARRAY[*MAT_MENU_PARTY_INDEX].enabledCommandArray;
    auto commandID = commands[flatIndex].commandID;
    if (commandID == 0xFF) {
        srLogWrite("DISABLING TEXT WIDGET FOR COMMAND NAME at index %i", flatIndex);
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    updateText(widget, gContext.gameStrings.command_names.get_string(commandID));
    updateTextColor(widget, COLOR_WHITE);
    srLogWrite("ENABLED TEXT WIDGET FOR COMMAND NAME at index %i", flatIndex);
}

void spellNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }

    auto typedPtr = (CursorGridWidget*)self;
    auto magics = gContext.party.get_resource(*MAT_MENU_PARTY_INDEX).actorMagics;
    if (magics[flatIndex].magicIndex == 0xFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    updateText(widget, gContext.attacks.get_element(std::string("MAG") + std::to_string(flatIndex)).attackName.str());
}

void summonNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto summons = gContext.party.get_resource(*MAT_MENU_PARTY_INDEX).actorSummons;
    if (summons[flatIndex].magicIndex == 0xFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    updateText(widget, gContext.attacks.get_element(std::string("SUM") + std::to_string(flatIndex)).attackName.str());
}

void eskillNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto eSkills = gContext.party.get_resource(*MAT_MENU_PARTY_INDEX).actorEnemySkills;
    if (eSkills[flatIndex].magicIndex == 0xFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    updateText(widget, gContext.attacks.get_element(std::string("ESK") + std::to_string(flatIndex)).attackName.str());
}

void battleSpellNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }

    auto typedPtr = (CursorGridWidget*)self;
    auto magics = gContext.party.get_resource(*BATTLE_ACTIVE_ACTOR_ID).actorMagics;
    if (magics[flatIndex].magicIndex == 0xFF) {
        disableWidget(getChild(widget, std::string("ARW")));
        disableWidget(getChild(widget, std::string("TXT")));
        return;
    }
    enableWidget(getChild(widget, std::string("TXT")));
    updateText(getChild(widget, std::string("TXT")), gContext.attacks.get_element(std::string("MAG") + std::to_string(flatIndex)).attackName.str());
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
    auto summons = gContext.party.get_resource(*BATTLE_ACTIVE_ACTOR_ID).actorSummons;
    if (summons[flatIndex].magicIndex == 0xFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    updateText(widget, gContext.attacks.get_element(std::string("SUM") + std::to_string(flatIndex)).attackName.str());
}

void battleEskillNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto eSkills = gContext.party.get_resource(*BATTLE_ACTIVE_ACTOR_ID).actorEnemySkills;
    if (eSkills[flatIndex].magicIndex == 0xFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    updateText(widget, gContext.attacks.get_element(std::string("ESK") + std::to_string(flatIndex)).attackName.str());
}

void battleInventoryRowUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto itemID = gContext.battleInventory->get_resource(flatIndex).item_id;
    if (itemID == 0xFFFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    auto textColor = (isUsableInBattle(itemID)) ? COLOR_WHITE : COLOR_GRAY;
    updateText(getChild(widget, std::string("TXT")), getNameFromItemID(itemID));
    updateTextColor(getChild(widget, std::string("TXT")), textColor);
    updateNumber(getChild(widget, std::string("AMT")), gContext.battleInventory->get_resource(itemID).quantity);
    updateNumberColor(getChild(widget, std::string("AMT")), textColor);
    updateItemIcon(getChild(widget, std::string("ICN")), gContext.itemTypeData.get_resource(itemID).itemIconType);
}
