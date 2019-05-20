#include "updaters.h"
#include "../impl.h"
#include "../inventories/inventory_utils.h"

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
    if (self->collectionType != GridWidgetClass()) {
        return;
    }

    auto typedPtr = (CursorGridWidget*)self;
    auto materiaID = gContext.materiaInventory->get_resource(flatIndex).item_id;
    if (materiaID != 0xFFFF) {
        enableWidget(widget);
        const char* name = gContext.gameStrings.materia_names.get_string(flatIndex);
        updateText(widget, name);
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
    if (materiaID != 0xFFFF) {
        enableWidget(widget);
        auto materiaAssetType = getMateriaType(materiaID);
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
    auto typedPtr = (CursorGridWidget*)self;
    auto partyIndex = *MAT_MENU_PARTY_INDEX;
    auto commands = (PARTY_STRUCT_ARRAY)[partyIndex].enabledCommandArray;
    auto commandID = commands[flatIndex].commandID;
    if (commandID = 0xFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    updateText(widget, gContext.gameStrings.command_names.get_string(commandID));
}

void spellNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }

    auto typedPtr = (CursorGridWidget*)self;
    auto magics = gContext.party.get_resource(*MAT_MENU_PARTY_INDEX).actorMagics;
    if (magics[flatIndex].magicIndex = 0xFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    updateText(widget, gContext.attacks.get_element(std::string("MAG") + std::to_string(flatIndex)).attackName);
}

void summonNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto summons = gContext.party.get_resource(*MAT_MENU_PARTY_INDEX).actorSummons;
    if (summons[flatIndex].magicIndex = 0xFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    updateText(widget, gContext.attacks.get_element(std::string("SUM") + std::to_string(flatIndex)).attackName);
}

void eskillNameViewUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }
    auto typedPtr = (CursorGridWidget*)self;
    auto eSkills = gContext.party.get_resource(*MAT_MENU_PARTY_INDEX).actorEnemySkills;
    if (eSkills[flatIndex].magicIndex = 0xFF) {
        disableWidget(widget);
        return;
    }
    enableWidget(widget);
    updateText(widget, gContext.attacks.get_element(std::string("ESK") + std::to_string(flatIndex)).attackName);
}
