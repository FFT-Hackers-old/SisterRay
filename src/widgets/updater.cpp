#include "updaters.h"
#include "../impl.h"

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
        auto textColor = usableInInventoryMenu(itemID) ? 0 : 7;
        const char* name = gContext.game_strings.key_item_names.get_string(flatIndex);
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
    if (gContext.inventory->get_resource(flatIndex).item_id != 0xFFFF) {
        enableWidget(widget);

        auto textColor = usableInInventoryMenu(itemID) ? 0 : 7;
        auto itemQuantity = gContext.inventory->get_resource(flatIndex).quantity;
        updateNumber(widget, itemQuantity);
        updateNumberColor(widget, textColor);
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
        const char* name = gContext.game_strings.key_item_names.get_string(flatIndex);
        updateText(widget, name);
    }
    else {
        disableWidget(widget);
    }
}
