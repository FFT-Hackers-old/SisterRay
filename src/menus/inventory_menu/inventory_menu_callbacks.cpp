
#include "inventory_menu_callbacks.h"
#include "../menu.h"
#include "../../impl.h"

using namespace InventoryWidgetNames;


void initializeInventoryMenu() {
    CursorContext topBarCursor = { 0, 0, 3, 1, 0, 0, 3, 1, 0, 0, 1, 0, 0, 0 };
    CursorContext itemViewCursor = { 0, 0, 1, 10, 0, 0, 1, gContext.inventory->current_capacity(), 0, 0, 0, 0, 0, 1 };
    CursorContext charViewCursor = { 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 0, 1, 0, 0 };
    CursorContext keyItemsViewCursor = { 0, 0, 2, 10, 0, 0, 2, 32, 0, 0, 2, 0, 0, 1 };
    CursorContext arrangeViewCursor = { 0, 0, 1, 8, 0, 0, 1, 8, 0, 0, 0, 1, 0, 0 };
    CursorContext customSortCursor = { 0, 0, 1, 10, 0, 0, 1, gContext.inventory->current_capacity(), 0, 0, 0, 0, 0, 1 };
    CursorContext cursorContextArray[6] = { topBarCursor, itemViewCursor, charViewCursor, keyItemsViewCursor, arrangeViewCursor, customSortCursor };
    auto inventoryMenu = createMenu(INIT_INVENTORY_MENU, 6, &cursorContextArray[0]);
    gContext.menuWidgets.add_element("INVENTORY_MENU", inventoryMenu);
    gContext.menuWidgets.initializeMenu("INVENTORY_MENU", INVENTORY_MENU_NAME);
}

void registerInventoryMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({ INVENTORY_MENU_CONTEXT });

    gContext.eventBus.addListener(INIT_INVENTORY_MENU, (SrEventCallback)&initViewChoiceWidget, modName);
    gContext.eventBus.addListener(INIT_INVENTORY_MENU, (SrEventCallback)&initCharViewWidget, modName);
    gContext.eventBus.addListener(INIT_INVENTORY_MENU, (SrEventCallback)&initItemViewWidget, modName);
    gContext.eventBus.addListener(INIT_INVENTORY_MENU, (SrEventCallback)&keyItemsViewWidget, modName);
    gContext.eventBus.addListener(INIT_INVENTORY_MENU, (SrEventCallback)&itemDescriptionWidget, modName);
    gContext.eventBus.addListener(INIT_INVENTORY_MENU, (SrEventCallback)&arrangeTypeWidget, modName);

    gContext.eventBus.addListener(DRAW_INVENTORY_MENU, (SrEventCallback)&handleCustomSortActive, modName); 
    gContext.eventBus.addListener(DRAW_INVENTORY_MENU, (SrEventCallback)&handleKeyItemsViewActive, modName);
    gContext.eventBus.addListener(DRAW_INVENTORY_MENU, (SrEventCallback)&handlePartyViewActive, modName);
    gContext.eventBus.addListener(DRAW_INVENTORY_MENU, (SrEventCallback)&handleUpdateDescription, modName);
    gContext.eventBus.addListener(DRAW_INVENTORY_MENU, (SrEventCallback)&handleArrangeActive, modName);
    gContext.eventBus.addListener(DRAW_INVENTORY_MENU, (SrEventCallback)&handleMenuViewActive, modName);

    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&chooseViewHandler, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&selectItemHandler, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&executeSwapHandler, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&useTargetedItemHandler, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&arrangeItemsHandler, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&exitInventoryHandler, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&exitItemView, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&exitSwapHandler, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&exitTargetingHandler, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&exitArrangeHandler, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&exitKeyItemsHandler, modName, contextKeys);
}
