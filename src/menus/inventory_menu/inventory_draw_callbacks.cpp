#include "inventory_draw_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"

using namespace InventoryWidgetNames;

void handleCustomSortActive(const InventoryDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;

    auto normalWidget = getChild(getChild(menuWidget, ITEM_VIEW_WIDGET_NAME), NORMAL_ITEM_VIEW_NAME);
    auto customWidget = getChild(getChild(menuWidget, ITEM_VIEW_WIDGET_NAME), CUSTOM_ITEM_VIEW_NAME);
    switch (event->menuState) {
        case 0:
            disableWidget(customWidget);
            enableWidget(normalWidget);
            break;
        case 5:
            enableWidget(customWidget);
            disableWidget(normalWidget);
            break;
        default: {
        }
    }
}

void handleUpdateDescription(const InventoryDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    auto itemDescWidget = getChild(menuWidget, ITEM_DESC_WIDGET_NAME);
    const char* fetchedDescription;

    switch (event->menuState) {
        auto itemChoice = getStateCursor(event->menu, 1);
        auto flatInventoryIndex = itemChoice->baseRowIndex + itemChoice->relativeRowIndex;
        case 1: {
            if (gContext.inventory->get_resource(flatInventoryIndex).item_id != 0xFFFF) {
                fetchedDescription = getDescriptionFromID(gContext.inventory->get_resource(flatInventoryIndex).item_id);
                updateText(getChild(itemDescWidget, ITEM_DESCRIPTION), fetchedDescription);
            }
            break;
        }
        case 2: {
            if (gContext.inventory->get_resource(flatInventoryIndex).item_id != 0xFFFF) {
                fetchedDescription = getDescriptionFromID(gContext.inventory->get_resource(flatInventoryIndex).item_id);
                updateText(getChild(itemDescWidget, ITEM_DESCRIPTION), fetchedDescription);
            }
            break;
        }
        case 3: {
            auto keyItemChoice = getStateCursor(event->menu, 3);
            flatInventoryIndex = (keyItemChoice->maxColumnBound) * ((keyItemChoice->baseRowIndex) + (keyItemChoice->relativeRowIndex) + (keyItemChoice->relativeColumnIndex);
            if ((KEY_ITEMS_INVENTORY_PTR)[flatInventoryIndex] != 0xFFFF) {
                auto keyItemID = (KEY_ITEMS_INVENTORY_PTR)[flatInventoryIndex];
                fetchedDescription = gContext.game_strings.key_item_descriptions.get_string(keyItemID);
                updateText(getChild(itemDescWidget, ITEM_DESCRIPTION), fetchedDescription);
            }
            break;
        }
        case 5: {
            auto customSortChoice = getStateCursor(event->menu, 5);
            flatInventoryIndex = customSortChoice->baseRowIndex + customSortChoice->relativeRowIndex;
            if (gContext.inventory->get_resource(flatInventoryIndex).item_id != 0xFFFF) {
                fetchedDescription = getDescriptionFromID(gContext.inventory->get_resource(flatInventoryIndex).item_id);
                updateText(getChild(itemDescWidget, ITEM_DESCRIPTION), fetchedDescription);
            }
            break;
        }
        default:
            break;
    }
}

void handleKeyItemsViewActive(const InventoryDrawEvent* event){
    auto viewChoice = getStateCursor(event->menu, 0);
    auto keyItemsWidget = getChild(event->menu->menuWidget, KEY_ITEMS_VIEW_WIDGET_NAME);
    if (viewChoice->relativeColumnIndex == 2) {
        enableWidget(keyItemsWidget);
        return;
    }
    disableWidget(keyItemsWidget);
}

void handlePartyViewActive(const InventoryDrawEvent* event) {
    auto viewChoice = getStateCursor(event->menu, 0);
    auto portraitsWidget = getChild(event->menu->menuWidget, PARTY_VIEW_WIDGET_NAME);
    if (viewChoice->relativeColumnIndex != 2) {
        enableWidget(portraitsWidget);
        return;
    }
    disableWidget(portraitsWidget);
}
