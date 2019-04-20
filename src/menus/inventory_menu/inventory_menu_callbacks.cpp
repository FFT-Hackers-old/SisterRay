
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
    auto inventoryMenu = createMenu(INIT_INVENTORY_MENU, 2, &cursorContextArray[0]);
    gContext.menuWidgets.add_element("INVENTORY_MENU", inventoryMenu);
    gContext.menuWidgets.initializeMenu("INVENTORY_MENU", INVENTORY_MENU_NAME);
}

void registerInventoryMenuListeners() {
    gContext.eventBus.addListener(INIT_INVENTORY_MENU, (SrEventCallback)&initCharDataWidget);
    gContext.eventBus.addListener(INIT_INVENTORY_MENU, (SrEventCallback)&initGearMateriaSlotWidget);
    gContext.eventBus.addListener(INIT_INVENTORY_MENU, (SrEventCallback)&initGearDescWidget);
    gContext.eventBus.addListener(INIT_INVENTORY_MENU, (SrEventCallback)&initStatDiffWidget);
    gContext.eventBus.addListener(INIT_INVENTORY_MENU, (SrEventCallback)&initGearListWidget);

    gContext.eventBus.addListener(DRAW_INVENTORY_MENU, (SrEventCallback)&handleChangeCharacter); 
    gContext.eventBus.addListener(DRAW_INVENTORY_MENU, (SrEventCallback)&handleUpdateDescription);
    gContext.eventBus.addListener(DRAW_INVENTORY_MENU, (SrEventCallback)&handleUpdateGearSlotsWidget);
    gContext.eventBus.addListener(DRAW_INVENTORY_MENU, (SrEventCallback)&handleUpdateStatMenuWidget);
    gContext.eventBus.addListener(DRAW_INVENTORY_MENU, (SrEventCallback)&enableListWidget);
}
