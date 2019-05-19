
#include "materia_menu_callbacks.h"
#include "../menu.h"
#include "../../impl.h"

using namespace MateriaWidgetNames;


void initializeMateriaMenu() {
    /*The Materia menu has 11 well-defined viewing states*/
    CursorContext checkArrangeContext = { 0, 0, 1, 2, 0, 0, 1, 2, 0, 0, 0, 1, 0, 0 };
    Cursor checkArrangeCursor = { checkArrangeContext, 240, 62, 51, 0 };
    CursorContext slotSelectContext = { 0, 0, 8, 2, 0, 0, 8, 2, 0, 0, 0, 0, 0, 0 };
    Cursor slotSelectCursor = { slotSelectContext, 317, 62, 51, 28 };
    CursorContext materiaInventoryContext = { 0, 0, 1, 10, 0, 0, 1, gContext.materiaInventory->current_capacity(), 0, 0, 0, 0, 0, 1 };
    Cursor materiaInventoryCursor = { materiaInventoryContext, 362, 214, 26, 0 };
    /*This is a view of the players command data. It must be set based on the active actor in an updater*/
    CursorContext checkCommandContext = { 0, 0, activePartyStructArray[0].commandRows, 0, 0,activePartyStructArray[0].commandRows, 4, 0, 0, 1, 1, 0, 0 };
    Cursor checkCommandCursor = { checkCommandContext, 0xE, 0xE7, 26, 13 };
    /*The following cursors are for views of available magics, summons and enemy skills*/
    CursorContext magicViewContext = { 0, 0, 3, 3, 0, 0, 3, 18, 0, 0, 2, 0, 0, 1 };
    Cursor magicViewContext = { magicViewContext, 0x2A, 0x167, 34, 130 };
    CursorContext summonViewContext = { 0, 0, 1, 3, 0, 0, 1, 16, 0, 0, 0, 0, 0, 1 };
    Cursor summonViewCursor = { summonViewContext, 0x2F + 50, 0x167, 34, 0 };
    CursorContext eSkillViewContext = { 0, 0, 1, 3, 0, 0, 1, 16, 0, 0, 0, 0, 0, 1 };
    Cursor eSkillViewCursor = { eSkillViewContext, 0x2F, 0x167, 34, 200 };
    /*Arrange cursor*/
    CursorContext arrangeViewContext = { 0, 0, 1, 4, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1 };
    Cursor arrangeViewCursor = { arrangeViewContext, 0x2F, 0xD9, 26, 0 };
    CursorContext trashViewContext = { 0, 0, 1, 10, 0, 0, 1, gContext.materiaInventory->current_capacity(), 0, 0, 0, 0, 0, 1 };
    Cursor arrangeViewCursor = { arrangeViewContext, 362, 214, 26, 0 };
    CursorContext destroyMateriaContext = { 0, 1, 1, 2, 0, 0, 1, 2, 0, 0, 0, 1, 0, 0 };
    Cursor destroyMateriaCursor = { destroyMateriaContext, 0x92 ,0x67, 26, 0 };

    Cursor cursorArray[11] = { checkArrangeCursor, slotSelectCursor, materiaInventoryCursor,
        checkCommandCursor, magicViewContext, summonViewCursor,
        eSkillViewCursor, arrangeViewCursor, arrangeViewCursor,
        destroyMateriaContext
    };
    auto materiaMenu = createMenu(INIT_MATERIA_MENU, 12, &(cursorArray[0]));
    gContext.menuWidgets.add_element("MATERIA_MENU", materiaMenu);
    gContext.menuWidgets.initializeMenu("MATERIA_MENU", INVENTORY_MENU_NAME);
}

void registerMateriaMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({ MATERIA_MENU_CONTEXT });

    gContext.eventBus.addListener(INIT_MATERIA_MENU, (SrEventCallback)&initViewChoiceWidget, modName);
    gContext.eventBus.addListener(INIT_MATERIA_MENU, (SrEventCallback)&initCharViewWidget, modName);
    gContext.eventBus.addListener(INIT_MATERIA_MENU, (SrEventCallback)&initItemViewWidget, modName);
    gContext.eventBus.addListener(INIT_MATERIA_MENU, (SrEventCallback)&keyItemsViewWidget, modName);
    gContext.eventBus.addListener(INIT_MATERIA_MENU, (SrEventCallback)&itemDescriptionWidget, modName);
    gContext.eventBus.addListener(INIT_MATERIA_MENU, (SrEventCallback)&arrangeTypeWidget, modName);

    gContext.eventBus.addListener(DRAW_MATERIA_MENU, (SrEventCallback)&handleCustomSortActive, modName); 
    gContext.eventBus.addListener(DRAW_MATERIA_MENU, (SrEventCallback)&handleKeyItemsViewActive, modName);
    gContext.eventBus.addListener(DRAW_MATERIA_MENU, (SrEventCallback)&handlePartyViewActive, modName);
    gContext.eventBus.addListener(DRAW_MATERIA_MENU, (SrEventCallback)&handleUpdateDescription, modName);
    gContext.eventBus.addListener(DRAW_MATERIA_MENU, (SrEventCallback)&handleArrangeActive, modName);
    gContext.eventBus.addListener(DRAW_MATERIA_MENU, (SrEventCallback)&handleMenuViewActive, modName);

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
