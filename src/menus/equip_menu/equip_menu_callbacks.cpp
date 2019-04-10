
#include "equip_menu_callbacks.h"
#include "../../impl.h"

using namespace EquipWidgetNames;

void registerEquipMenuListeners() {
    gContext.eventBus.addListener(INIT_EQUIP_MENU, (SrEventCallback)&initCharDataWidget);
    gContext.eventBus.addListener(INIT_EQUIP_MENU, (SrEventCallback)&initGearMateriaSlotWidget);
    gContext.eventBus.addListener(INIT_EQUIP_MENU, (SrEventCallback)&initGearDescWidget);
    gContext.eventBus.addListener(INIT_EQUIP_MENU, (SrEventCallback)&initStatDiffWidget);
    gContext.eventBus.addListener(INIT_EQUIP_MENU, (SrEventCallback)&initGearListWidget);

    gContext.eventBus.addListener(DRAW_EQUIP_MENU, (SrEventCallback)&handleChangeCharacter); 
    gContext.eventBus.addListener(DRAW_EQUIP_MENU, (SrEventCallback)&handleUpdateDescription);
    gContext.eventBus.addListener(DRAW_EQUIP_MENU, (SrEventCallback)&handleUpdateGearSlotsWidget);
    gContext.eventBus.addListener(DRAW_EQUIP_MENU, (SrEventCallback)&handleUpdateStatMenuWidget);

    gContext.eventBus.addListener(EQUIP_MENU_INPUT_OK, (SrEventCallback)&equipGearHandler);
    gContext.eventBus.addListener(EQUIP_MENU_INPUT_CANCEL, (SrEventCallback)&exitMenuListener);
    gContext.eventBus.addListener(EQUIP_MENU_INPUT_SQUARE, (SrEventCallback)&changeToMateriaMenu);
    gContext.eventBus.addListener(EQUIP_MENU_INPUT_TRIANGLE, (SrEventCallback)&handleUnequipAcc);
    gContext.eventBus.addListener(EQUIP_MENU_INPUT_L1, (SrEventCallback)&changeCharLeft);
    gContext.eventBus.addListener(EQUIP_MENU_INPUT_R1, (SrEventCallback)&changeCharRight);
}
