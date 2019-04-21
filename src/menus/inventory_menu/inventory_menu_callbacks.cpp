
#include "inventory_menu_callbacks.h"
#include "../menu.h"
#include "../../impl.h"

using namespace InventoryWidgetNames;


void initializeInventoryMenu() {
    CursorContext topBarContext = { 0, 0, 3, 1, 0, 0, 3, 1, 0, 0, 1, 0, 0, 0 };
    Cursor topBarCursor = { topBarContext, 13, 26, 0, 93 };
    CursorContext itemViewContext = { 0, 0, 1, 10, 0, 0, 1, gContext.inventory->current_capacity(), 0, 0, 0, 0, 0, 1 };
    Cursor itemViewCursor = { itemViewContext, 298, 109, 37, 0 };
    CursorContext charViewContext = { 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 0, 1, 0, 0 };
    Cursor charViewCursor = { charViewContext, 298, 109, 37, 0 };
    CursorContext keyItemsViewContext = { 0, 0, 2, 10, 0, 0, 2, 32, 0, 0, 2, 0, 0, 1 };
    Cursor keyItemsViewCursor = { keyItemsViewContext, 5, 129, 36, 293 };
    CursorContext arrangeViewContext = { 0, 0, 1, 8, 0, 0, 1, 8, 0, 0, 0, 1, 0, 0 };
    Cursor arrangeViewCursor = { arrangeViewContext, 0xBE, 0x2B, 26, 0 };
    CursorContext customSortContext = { 0, 0, 1, 10, 0, 0, 1, gContext.inventory->current_capacity(), 0, 0, 0, 0, 0, 1 };
    Cursor customSortViewCursor = { customSortContext, 298, 109, 37, 0 };

    Cursor cursorArray[6] = { topBarCursor, itemViewCursor, charViewCursor, keyItemsViewCursor, arrangeViewCursor, customSortViewCursor };
    auto inventoryMenu = createMenu(INIT_INVENTORY_MENU, 6, &(cursorArray[0]));
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
