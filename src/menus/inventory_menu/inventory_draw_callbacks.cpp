#include "inventory_draw_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"

using namespace InventoryWidgetNames;

void handleCustomSortActive(const MenuDrawEvent* event) {
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

void handleMenuViewActive(const MenuDrawEvent* event) {
    auto viewChoice = getStateCursor(event->menu, 0)->context;
    auto itemViewWidget = getChild(event->menu->menuWidget, ITEM_VIEW_WIDGET_NAME);
    if (viewChoice.relativeColumnIndex != 2) {
        enableWidget(itemViewWidget);
        return;
    }
    disableWidget(itemViewWidget);
}

void handleUpdateDescription(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    auto itemDescWidget = getChild(menuWidget, ITEM_DESC_WIDGET_NAME);
    const char* fetchedDescription;

    auto itemChoice = getStateCursor(event->menu, 1)->context;
    auto flatInventoryIndex = itemChoice.baseRowIndex + itemChoice.relativeRowIndex;
    switch (event->menuState) {
        case 1: {
            if (gContext.inventory->getResource(flatInventoryIndex).materiaID != 0xFFFF) {
                fetchedDescription = getDescriptionFromID(gContext.inventory->getResource(flatInventoryIndex).materiaID);
                updateText(getChild(itemDescWidget, ITEM_DESCRIPTION), fetchedDescription);
            }
            else {
                updateText(getChild(itemDescWidget, ITEM_DESCRIPTION), "\xFF");
            }
            break;
        }
        case 2: {
            if (gContext.inventory->getResource(flatInventoryIndex).materiaID != 0xFFFF) {
                fetchedDescription = getDescriptionFromID(gContext.inventory->getResource(flatInventoryIndex).materiaID);
                updateText(getChild(itemDescWidget, ITEM_DESCRIPTION), fetchedDescription);
            }
            else {
                updateText(getChild(itemDescWidget, ITEM_DESCRIPTION), "\xFF");
            }
            break;
        }
        case 3: {
            auto keyItemChoice = getStateCursor(event->menu, 3)->context;
            flatInventoryIndex = (keyItemChoice.maxColumnBound * (keyItemChoice.baseRowIndex + keyItemChoice.relativeRowIndex)) + keyItemChoice.relativeColumnIndex;
            if ((KEY_ITEMS_INVENTORY_PTR)[flatInventoryIndex] != 0xFFFF) {
                auto keyItemID = (KEY_ITEMS_INVENTORY_PTR)[flatInventoryIndex];
                fetchedDescription = gContext.gameStrings.key_item_descriptions.get_string(keyItemID);
                updateText(getChild(itemDescWidget, ITEM_DESCRIPTION), fetchedDescription);
            }
            else {
                updateText(getChild(itemDescWidget, ITEM_DESCRIPTION), "\xFF");
            }
            break;
        }
        case 5: {
            auto customSortChoice = getStateCursor(event->menu, 5)->context;
            flatInventoryIndex = customSortChoice.baseRowIndex + customSortChoice.relativeRowIndex;
            if (gContext.inventory->getResource(flatInventoryIndex).materiaID != 0xFFFF) {
                fetchedDescription = getDescriptionFromID(gContext.inventory->getResource(flatInventoryIndex).materiaID);
                updateText(getChild(itemDescWidget, ITEM_DESCRIPTION), fetchedDescription);
            }
            else {
                updateText(getChild(itemDescWidget, ITEM_DESCRIPTION), "\xFF");
            }
            break;
        }
        default:
            updateText(getChild(itemDescWidget, ITEM_DESCRIPTION), "\xFF");
            break;
    }
}

void handleKeyItemsViewActive(const MenuDrawEvent* event){
    auto viewChoice = getStateCursor(event->menu, 0)->context;
    auto keyItemsWidget = getChild(event->menu->menuWidget, KEY_ITEMS_VIEW_NAME);
    if (viewChoice.relativeColumnIndex == 2) {
        enableWidget(keyItemsWidget);
        return;
    }
    disableWidget(keyItemsWidget);
}

void handlePartyViewActive(const MenuDrawEvent* event) {
    auto viewChoice = getStateCursor(event->menu, 0)->context;
    auto portraitsWidget = getChild(event->menu->menuWidget, PARTY_VIEW_WIDGET_NAME);
    if (viewChoice.relativeColumnIndex != 2) {
        enableWidget(portraitsWidget);
        return;
    }
    disableWidget(portraitsWidget);
}

void handleArrangeActive(const MenuDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;
    if (event->menuState != 4) {
        disableWidget(getChild(menuWidget, ARRANGE_TYPE_WIDGET_NAME));
        return;
    }
    enableWidget(getChild(menuWidget, ARRANGE_TYPE_WIDGET_NAME));
}
