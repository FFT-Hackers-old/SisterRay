
#include "equip_menu_callbacks.h"
#include "../menu.h"
#include <unordered_set>
#include "../../impl.h"

using namespace EquipWidgetNames;


void initializeEquipMenu() {
    CursorContext gearTypeSelection = { 0, 0, 1, 3, 0, 0, 1, 3, 0, 0, 0, 1, 0, 0 };
    Cursor gearTypeCursor = { gearTypeSelection, 207, 17, 36, 0 };
    CursorContext gearSelection = { 0, 0, 1, 8, 0, 0, 1, 16, 0, 0, 0, 0, 0, 1 };
    Cursor gearChoiceCursor = { gearSelection, 385, 197, 36, 0 };

    Cursor cursorArray[2] = { gearTypeCursor, gearChoiceCursor };
    auto equipMenu = createMenu(INIT_EQUIP_MENU, 2, &cursorArray[0]);
    gContext.menuWidgets.add_element(EQUIP_MENU_NAME, equipMenu);
    gContext.menuWidgets.initializeMenu(EQUIP_MENU_NAME, EQUIP_WIDGET_NAME);
}

void registerEquipMenuListeners() {
    const auto& modName = std::string("srFF7Base");
    const auto& contextKeys = std::unordered_set<SrEventContext>({ EQUIP_MENU_CONTEXT });

    gContext.eventBus.addListener(INIT_EQUIP_MENU, (SrEventCallback)&initCharDataWidget, modName);
    gContext.eventBus.addListener(INIT_EQUIP_MENU, (SrEventCallback)&initGearMateriaSlotWidget, modName);
    gContext.eventBus.addListener(INIT_EQUIP_MENU, (SrEventCallback)&initGearDescWidget, modName);
    gContext.eventBus.addListener(INIT_EQUIP_MENU, (SrEventCallback)&initStatDiffWidget, modName);
    gContext.eventBus.addListener(INIT_EQUIP_MENU, (SrEventCallback)&initGearListWidget, modName);

    gContext.eventBus.addListener(DRAW_EQUIP_MENU, (SrEventCallback)&handleChangeCharacter, modName); 
    gContext.eventBus.addListener(DRAW_EQUIP_MENU, (SrEventCallback)&handleUpdateDescription, modName);
    gContext.eventBus.addListener(DRAW_EQUIP_MENU, (SrEventCallback)&handleUpdateGearSlotsWidget, modName);
    gContext.eventBus.addListener(DRAW_EQUIP_MENU, (SrEventCallback)&handleUpdateStatMenuWidget, modName);
    gContext.eventBus.addListener(DRAW_EQUIP_MENU, (SrEventCallback)&enableListWidget, modName);

    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&equipGearHandler, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_OK, (SrEventCallback)&selectGearHandler, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&exitMenuListener, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_CANCEL, (SrEventCallback)&exitEquipViewListener, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_SQUARE, (SrEventCallback)&changeToMateriaMenu, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_TRIANGLE, (SrEventCallback)&handleUnequipAcc, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_L1, (SrEventCallback)&changeCharLeft, modName, contextKeys);
    gContext.eventBus.addListener(MENU_INPUT_R1, (SrEventCallback)&changeCharRight, modName, contextKeys);
}
